/***************************************************************************************************
 * INCLUDES
 **************************************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include "bsp_uart_radio.h"
#include "utils.h"
#include "bsp_uart.h"

#ifdef RADIO_MODE_UART
/***************************************************************************************************
 * LOCAL DEFINES
 **************************************************************************************************/
#define RADIO_UART_CHANNELS  9
static uint16_t channels[RADIO_UART_CHANNELS] = {0};


/***************************************************************************************************
 * LOCAL TYPEDEFS
 **************************************************************************************************/

/***************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 **************************************************************************************************/

/***************************************************************************************************
 * LOCAL VARIABLES
 **************************************************************************************************/
static bsp_uart_radio_callback_t external_callback;
static void uart_callback(void *arg);

static bool stop_uart;


/***************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/

/***************************************************************************************************
 * LOCAL FUNCTIONS
 **************************************************************************************************/

static void uart_callback(void *arg) {

    int16_t data[4];

    data[0] = *(((int16_t*) (arg)) + 0); 
    data[1] = *(((int16_t*) (arg)) + 1); 
    data[2] = *(((int16_t*) (arg)) + 2);
    data[3] = *(((int16_t*) (arg)) + 3);

    // Fake implementation of uart radio service
    if (data[0] == 0){
        channels[0] = map(data[1], 0, 255, RX_RADIO_MIN_VALUE, RX_RADIO_MAX_VALUE);
        channels[1] = map(data[2], 0, 255, RX_RADIO_MIN_VALUE, RX_RADIO_MAX_VALUE);
    } else if (data[0] == 1){
        channels[2] = map(data[1], 0, 255, RX_RADIO_MIN_VALUE, RX_RADIO_MAX_VALUE);
        channels[3] = map(data[2], 0, 255, RX_RADIO_MIN_VALUE, RX_RADIO_MAX_VALUE);
        channels[5] = map(data[3], 0, 255, RX_RADIO_MIN_VALUE, RX_RADIO_MAX_VALUE);
        if (!stop_uart){
            external_callback(channels, RADIO_UART_CHANNELS);
        }
    
    }
    
}



/***************************************************************************************************
 * GLOBAL FUNCTIONS
 **************************************************************************************************/


void bsp_uart_radio_init() {
    printf("UART RADIO INIT \r\n");
    BSP_UART_Register_Callback(UART_NUM_4, uart_callback);
    stop_uart = true;
}

void bsp_uart_radio_start() {
    stop_uart = false;
}

void bsp_uart_radio_stop() {
    stop_uart = true;
}


void bsp_uart_radio_register_callback(bsp_uart_radio_callback_t callback_function){
    external_callback = callback_function;
}

#endif