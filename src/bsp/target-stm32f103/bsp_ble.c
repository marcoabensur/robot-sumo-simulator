/***************************************************************************************************
 * INCLUDES
 **************************************************************************************************/
#include <stdbool.h>
#include "usart.h"
#include "bsp_uart.h"
#include "bsp_ble.h"
#include "utils.h"
/***************************************************************************************************
 * LOCAL DEFINES
 **************************************************************************************************/

/***************************************************************************************************
 * LOCAL TYPEDEFS
 **************************************************************************************************/

#define UART_BLE_TIMEOUT    1000

/***************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 **************************************************************************************************/

/***************************************************************************************************
 * LOCAL VARIABLES
 **************************************************************************************************/
static void uart_callback(void *arg);
static void uart_error_callback(void *arg);

static uint8_t ble_dma_data[BLE_RECEIVE_PACKET_SIZE];
static bsp_uart_ble_callback_t external_callback;
static bool ble_running;

/***************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/

/***************************************************************************************************
 * LOCAL FUNCTIONS
 **************************************************************************************************/
static void uart_callback(void *arg) {
    UNUSED(arg);
    if (external_callback != NULL){
        external_callback(ble_dma_data, BLE_RECEIVE_PACKET_SIZE);
    }
    
}


void uart_error_callback(void *arg){
    UNUSED(arg);

}
/***************************************************************************************************
 * GLOBAL FUNCTIONS
 **************************************************************************************************/


void bsp_ble_init(){

    MX_USART3_UART_Init();
    BSP_UART_Register_Callback(UART_NUM_3, uart_callback);
    BSP_UART_Register_Error_Callback(UART_NUM_3, uart_error_callback);

    ble_running = false;
}

void bsp_ble_start(void) {
    if (!ble_running) {
        ble_running = true;
        HAL_UART_Receive_DMA(&huart3, ble_dma_data, sizeof(ble_dma_data));
    }

}

void bsp_ble_stop(void){
    ble_running = false;
    HAL_UART_DMAStop(&huart3);
}



void bsp_ble_transmit(uint8_t * data, uint8_t size) {

    uint8_t size_to_send = min(size, BLE_MAX_PACKET_SIZE);
    HAL_UART_Transmit(&huart3, data, size_to_send, UART_BLE_TIMEOUT);

}

void bsp_ble_register_callback(bsp_uart_ble_callback_t callback_function){
    external_callback = callback_function;
}