#include "controller.h"
#include "helpers.h"

// Internal definitions ---------------------------------------------------------------------------

#define END_SWITCH_PIN 13
#define INPUT_PINS_FLAG (1ULL << END_SWITCH_PIN)

#define DIRECTION_PIN 5
#define STEP_PIN 4
#define DONE_MOVING_PIN 15
#define OUTPUT_PINS_FLAG   ( (1ULL << DIRECTION_PIN) \
                           | (1ULL << DONE_MOVING_PIN) \
                           | (1ULL << STEP_PIN) )

// Period = 400 us, pps = 2500
#define STEP_HALF_PERIOD_US 200

#define CONTROLLER_TAG "CONTROLLER"

#define DIR_PULL 0
#define DIR_PUSH 1

#define MULTISAMPLING 16

// ------------------------------------------------------------------------------------------------

static int target = 0;
static int real_position = 0;
static bool done_moving = 0;

static bool finding_zero = false;
static bool step_pulse_value = false;

// ------------------------------------------------------------------------------------------------

void step_to_find_zero() {
    if (step_pulse_value == 1) {
        if (gpio_get_level(END_SWITCH_PIN) == 1) {
            target = 0;
            real_position = 0;
            finding_zero = false;
            hw_timer_enable(false);
        }

        step_pulse_value = 0;
    } else {
        gpio_set_level(DIRECTION_PIN, DIR_PUSH);
        step_pulse_value = 1;
    }

    gpio_set_level(STEP_PIN, step_pulse_value);
}

void try_step(void* arg) {
    if (finding_zero) {
        step_to_find_zero();
        return;
    }

    if (step_pulse_value == 1) {
        if (target == real_position) {
            hw_timer_enable(false);
            done_moving = true;
            gpio_set_level(DONE_MOVING_PIN, 1);
            return;
        }

        step_pulse_value = 0;
    } else {
        if (target > real_position) {
            gpio_set_level(DIRECTION_PIN, DIR_PULL);
            real_position ++;
        } else {
            gpio_set_level(DIRECTION_PIN, DIR_PUSH);
            real_position --;
        }

        step_pulse_value = 1;
    }

    gpio_set_level(STEP_PIN, step_pulse_value);
}

// Buttons ---------------------------------------
void buttons_init() {
    adc_config_t adc_config;
    adc_config.mode = ADC_READ_TOUT_MODE;
    adc_config.clk_div = 16;
    ESP_ERROR_CHECK(adc_init(&adc_config));
}

unsigned int read_button() {
    unsigned int mean = 0;
    unsigned int raw_reading = 0;

    for (int i=0; i<MULTISAMPLING; i++) {
        adc_read(&raw_reading);
        mean += raw_reading;
    }
    mean /= MULTISAMPLING;

    return mean;
}

void act_button(unsigned int reading) {
    // Expected values: 0, 152, 290, 417, 535, 643, 743, 836
    if (reading < 75) {
        set_target(real_position - 1600 * 50);       // -50mm
    }
    else if (reading < 225) {
        set_target(real_position - 1600 * 5);        // -5mm
    }
    else if (reading < 350) {
        set_target(real_position - 1600);            // -1mm
    }
    else if (reading < 475) {
        set_target(real_position);                   // stop
    }
    else if (reading < 590) {
        set_target(real_position + 1600);            // +1mm
    }
    else if (reading < 690) {
        set_target(real_position + 1600 * 5);        // -5mm
    }
    else if (reading < 790) {
        set_target(real_position + 1600 * 50);        // +50mm
    }
}

// Public ----------------------------------------------------------------------

void set_target(unsigned int _target) {
    if (target == _target) {
        return;
    }

    target = _target;

    done_moving = false;
    gpio_set_level(DONE_MOVING_PIN, 0);
    hw_timer_enable(true);
}

void reset_zero() {
    finding_zero = true;
    hw_timer_enable(true);
}

ControllerInfo get_info() {
    ControllerInfo info;
    info.state = finding_zero ? 2 : !done_moving;
    info.real_position = real_position;
    info.target_position = target;

    return info;
}

void poll_buttons() {
    while (1) {
        // printf("%u\n", read_button());
        act_button(read_button());
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void controller_init()
{
    gpio_config_t gpio_cfg;                                // Init output and stability pins
    gpio_cfg.intr_type = GPIO_INTR_DISABLE;
    gpio_cfg.mode = GPIO_MODE_OUTPUT;
    gpio_cfg.pin_bit_mask = OUTPUT_PINS_FLAG;
    gpio_cfg.pull_down_en = 0;
    gpio_cfg.pull_up_en = 0;
    gpio_config(&gpio_cfg);

    gpio_set_level(DIRECTION_PIN, 0);
    gpio_set_level(STEP_PIN, 0);
    gpio_set_level(DONE_MOVING_PIN, 0);


    gpio_config_t inputs_cfg;                                // Inputs interrupt
    inputs_cfg.intr_type = GPIO_INTR_DISABLE;
    inputs_cfg.pin_bit_mask = INPUT_PINS_FLAG;
    inputs_cfg.mode = GPIO_MODE_INPUT;
    inputs_cfg.pull_up_en = 0;
    inputs_cfg.pull_down_en = 0;
    gpio_config(&inputs_cfg);

    // Step timer -------------------------------------------------------------

    hw_timer_init(try_step, NULL);
    hw_timer_set_clkdiv(TIMER_CLKDIV_1);
    hw_timer_set_intr_type(TIMER_EDGE_INT);
    hw_timer_set_reload(true);
    hw_timer_set_load_data(STEP_HALF_PERIOD_US * (TIMER_BASE_CLK / 1000000));    // TODO check
    hw_timer_enable(false);

    // buttons ------------------
    buttons_init();

    ESP_LOGI(CONTROLLER_TAG, "Initiated the controller");

}