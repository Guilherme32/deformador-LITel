/* Deformador do LITel
* O controle e interface do sistema foram desenvolvidos sobre um esp8266, e
* aqui esta o codigo do seu firmware. Mais informacoes do sistema podem ser
* obtidas no github: https://github.com/Guilherme32/deformador-LITel
*/


#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"

#include "wifi.h"
#include "server.h"
#include "storage_spiffs.h"
#include "controller.h"
#include "serial_comm.h"


void app_main()
{
    // Some system info print -------------------------------------------------
    print_header();
    ESP_LOGI("STARTUP", "Starting up the system");

    // Modules init -----------------------------------------------------------
    wifi_init();
    spiffs_init();
    server_init(10);
    controller_init();
    serial_comm_init();

    // Linking modules --------------------------------------------------------
    add_command(netinfo_command, netinfo_command_help);

    // Starting modules tasks -------------------------------------------------
    xTaskCreate(wifi_led_task,
        "wifi_led_task",
        2*configMINIMAL_STACK_SIZE,
        NULL,
        10,
        NULL);

    xTaskCreate(serial_comm_task,
        "serial_task",
        2*configMINIMAL_STACK_SIZE,
        NULL,
        10,
        NULL);

    xTaskCreate(poll_buttons,
        "poll_buttons",
        2*configMINIMAL_STACK_SIZE,
        NULL,
        10,
        NULL);

}
