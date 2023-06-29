#include <stdlib.h>
#include <string.h>
#include "esp_log.h"
#include "esp_attr.h"
#include "driver/gpio.h"
#include "esp_timer.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"

#include "driver/hw_timer.h"


#ifndef CONTROLLER_IMPORT

typedef struct ControllerInfo {
    int state;        // 0 stopped, 1 moving, 2 finding zero
    unsigned int real_position;
    unsigned int target_position;
} ControllerInfo;

#define CONTROLLER_IMPORT

#endif


void controller_init();

void set_target(unsigned int target);
void reset_zero();
ControllerInfo get_info();

void poll_buttons();


