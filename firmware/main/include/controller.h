/**
* Componente de controle da flecha atraves de um motor de passo. O motor
* utilizado eh um NEMA 17, com 200 passos por volta. O driver foi conectado
* para o funcionamento com microstepping de 1/32. A rotacao do motor eh
* transferida para um movimento linear conectando o eixo em um fuso com passo
* de 2mm
*/


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
    int real_position;
    int target_position;
} ControllerInfo;

#define CONTROLLER_IMPORT

#endif


/**
* Inicializa o controlador. Deve ser executado antes de qualquer outra funcao
*/
void controller_init();

/**
* Coloca o alvo na posicao desejada. O sistema possui um zero intero que eh
* inicializado como a posicao atual quando o sistema eh energizado / resetado.
* O zero pode ser recalculado com a funcao 'reset_zero'. Pela montagem fisica
* do sistema, um valor inteiro de 1 representa um movimento de 0.625um
*/

void set_target(unsigned int target);
/**
* Reseta o zero. O programa entra em um estado em que ignora comandos e ativa o
* motor na direcao de aliviar a carga ate que encontre o sensor de fim de
* curso. Entao o programa marca tal ponto como o novo zero e volta a ouvir
* comandos
*/

void reset_zero();
/**
* Pega as informacoes do controlador. Essas sao definidas no struct retornado.
* As informacoes sao:
*   - state: O estado atual de funcionamento. 0 (parado), 1 (em movimento),
*     2 (procurando o zero);
*   - real_position: A posicao atual no fuso
*   - target_position: A posicao alvo atual
*
* ** O sistema nao detecta escorregamento. Se a carga for muito alta, o motor
* ira escorregar e perder passos. Nessa situracao a posicao real nao sera mais
* precisa
*/
ControllerInfo get_info();

/**
* Task que fica fazendo o polling dos botoes de comando. A estrategia de
* polling foi escolhida porque os botoes estao todos conectados em uma mesma
* entrada analogica, portanto nao seria possivel fazer a deteccao por
* interrupcoes
*/
void poll_buttons();


