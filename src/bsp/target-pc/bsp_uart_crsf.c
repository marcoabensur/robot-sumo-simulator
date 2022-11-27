/***************************************************************************************************
 * INCLUDES
 **************************************************************************************************/
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "bsp_uart.h"
#include "bsp_uart_crsf.h"
#include "utils.h"

#ifdef RADIO_MODE_UART_CRSF
/***************************************************************************************************
 * LOCAL DEFINES
 **************************************************************************************************/
#define RADIO_CRSF_CHANNELS  8
#define RADIO_CRSF_MAX_CHANNELS  16
/***************************************************************************************************
 * LOCAL TYPEDEFS
 **************************************************************************************************/

/***************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 **************************************************************************************************/

/***************************************************************************************************
 * LOCAL VARIABLES
 **************************************************************************************************/

static bool stop_uart;
static void uart_callback(void *arg);
static void uart_error_callback(void *arg);
uint16_t rc_channels[RADIO_CRSF_MAX_CHANNELS];
static bsp_uart_crsf_callback_t external_callback;


/***************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/

/***************************************************************************************************
 * LOCAL FUNCTIONS
 **************************************************************************************************/

static void uart_callback(void *arg) {

    int16_t data[3];

    data[0] = *(((int16_t*) (arg)) + 0); 
    data[1] = *(((int16_t*) (arg)) + 1); 
    data[2] = *(((int16_t*) (arg)) + 2);

    // Fake implementation of uart radio service
    if (data[0] == 0){
        rc_channels[0] = map(data[1], 0, 255, RADIO_CRSF_MIN_VALUE, RADIO_CRSF_MAX_VALUE);
        rc_channels[1] = map(data[2], 0, 255, RADIO_CRSF_MIN_VALUE, RADIO_CRSF_MAX_VALUE);
    } else if (data[0] == 1){
        rc_channels[2] = map(data[1], 0, 255, RADIO_CRSF_MIN_VALUE, RADIO_CRSF_MAX_VALUE);
        rc_channels[3] = map(data[2], 0, 255, RADIO_CRSF_MIN_VALUE, RADIO_CRSF_MAX_VALUE);
        if (!stop_uart){
            external_callback(rc_channels, RADIO_CRSF_CHANNELS);
        }
    
    }
    
}


void uart_error_callback(void *arg){
    (void)(arg); // Unused Parameter
}

/***************************************************************************************************
 * GLOBAL FUNCTIONS
 **************************************************************************************************/

void bsp_uart_crsf_init() {
    printf("UART RADIO CRSF INIT \r\n");
    BSP_UART_Register_Callback(UART_NUM_4, uart_callback);
    BSP_UART_Register_Error_Callback(UART_NUM_4, uart_error_callback);
    stop_uart = true;

}

void bsp_uart_crsf_start() {
    stop_uart = false;
}

void bsp_uart_crsf_stop() {
    stop_uart = true;
}


void bsp_uart_crsf_register_callback(bsp_uart_crsf_callback_t callback_function){
    external_callback = callback_function;
}

#endif