/***************************************************************************************************
 * INCLUDES
 **************************************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "bsp_uart.h"
#include "bsp_ble.h"
#include "utils.h"

#ifdef Q_SPY
#include "qs_defines.h"
#include "bsp.h"
#endif
/***************************************************************************************************
 * LOCAL DEFINES
 **************************************************************************************************/

/***************************************************************************************************
 * LOCAL TYPEDEFS
 **************************************************************************************************/


/***************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 **************************************************************************************************/

/***************************************************************************************************
 * LOCAL VARIABLES
 **************************************************************************************************/
static void uart_callback(void *arg);
static void uart_error_callback(void *arg);
static bsp_uart_ble_callback_t external_callback;
static bool ble_running;


/***************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/

/***************************************************************************************************
 * LOCAL FUNCTIONS
 **************************************************************************************************/
static void uart_callback(void *arg) {

    if (!ble_running){
        return;
    }

    (void)arg;
    uint8_t ble_dma_data[BLE_RECEIVE_PACKET_SIZE];


    printf("BLE Callback = ");

    for (size_t i = 0; i < sizeof(ble_dma_data); i++) {
        ble_dma_data[i] = *(((uint8_t*) (arg)) + i); 
        printf("0x%02x, ", ble_dma_data[i]);
    }

    printf("\r\n");
            

    if (external_callback != NULL){
        external_callback(ble_dma_data, BLE_RECEIVE_PACKET_SIZE);
    }
    
}


void uart_error_callback(void *arg){
    (void)arg;
}

/***************************************************************************************************
 * GLOBAL FUNCTIONS
 **************************************************************************************************/


void bsp_ble_init(){
    printf("BLE Service Init\r\n");
    BSP_UART_Register_Callback(UART_NUM_3, uart_callback);
    BSP_UART_Register_Error_Callback(UART_NUM_3, uart_error_callback);
    ble_running = false;

}

void bsp_ble_start(void) {
    if (!ble_running) {
        ble_running = true;
    }

}

void bsp_ble_stop(void){
    ble_running = false;
}

void bsp_ble_transmit(uint8_t * data, uint8_t size) {

    uint8_t size_to_send = min(size, BLE_MAX_PACKET_SIZE);

    printf("BLE Transmit Size = %d; Data:", size_to_send);
    for (int i = 0; i < size_to_send; i++)
    {
        printf("0x%02x, ", data[i]);
    }
    printf("\r\n");
    

    QS_BEGIN_ID(SIMULATOR, AO_SumoHSM->prio)
       QS_U8(1, QS_BLE_ID); 
       QS_MEM(data, size_to_send);
    QS_END()

}

void bsp_ble_register_callback(bsp_uart_ble_callback_t callback_function){
    external_callback = callback_function;
}