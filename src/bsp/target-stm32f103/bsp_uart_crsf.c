#ifdef RADIO_MODE_UART_CRSF
/***************************************************************************************************
 * INCLUDES
 **************************************************************************************************/
#include <stdbool.h>
#include "usart.h"
#include "bsp_uart.h"
#include "bsp_uart_crsf.h"
#include "radio_crsf.h"

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
static uint8_t rx_data[1];
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

    UNUSED(arg);

    crsf_packet_ret_t ret = crsf_parse_byte(rx_data[0]);

    if (ret == CRSF_PACKET_COMPLETE){
	    crsf_get_rc_data(rc_channels, RADIO_CRSF_CHANNELS);
        external_callback(rc_channels, RADIO_CRSF_CHANNELS);
    }
    
	HAL_UART_Receive_DMA(&huart4, rx_data, 1);


}


void uart_error_callback(void *arg){
    UNUSED(arg);

	HAL_UART_Receive_DMA(&huart4, rx_data, 1);

}

/***************************************************************************************************
 * GLOBAL FUNCTIONS
 **************************************************************************************************/

void bsp_uart_crsf_init() {

    MX_UART4_Init();
    BSP_UART_Register_Callback(UART_NUM_4, uart_callback);
    BSP_UART_Register_Error_Callback(UART_NUM_4, uart_error_callback);
}

void bsp_uart_crsf_start() {
    HAL_UART_Receive_DMA(&huart4, rx_data, 1);

}

void bsp_uart_crsf_stop() {
    HAL_UART_DMAStop(&huart4);

}


void bsp_uart_crsf_register_callback(bsp_uart_crsf_callback_t callback_function){
    external_callback = callback_function;
}
#endif