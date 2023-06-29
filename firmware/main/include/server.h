#include <stdio.h>
#include <string.h>
#include "esp_http_server.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_netif.h"
#include <lwip/sockets.h>
#include <lwip/sys.h>
#include <lwip/api.h>
#include <lwip/netdb.h>

#include <math.h>

#include <sys/unistd.h>
#include "esp_spiffs.h"

#include "wifi.h"
#include "controller.h"

/**
* Inicializa o servidor. Eh o unico sistema que cria a task na inicializacao,
* porque a task do servidor eh criada internamente 
* @param task_prio Prioridade da task do servidor
* @return O handle do servidor
*/
httpd_handle_t server_init(int task_prio);

