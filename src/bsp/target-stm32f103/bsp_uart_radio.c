#ifdef RADIO_MODE_UART

/***************************************************************************************************
 * INCLUDES
 **************************************************************************************************/

#include <stdbool.h>
#include "usart.h"
#include "bsp_uart.h"
#include "bsp_uart_radio.h"
/***************************************************************************************************
 * LOCAL DEFINES
 **************************************************************************************************/
#define RXDATA_SIZE 25
#define RADIO_UART_CHANNELS  9
#define RX_RADIO_FIRST_BYTE 0x0F
#define RX_RADIO_LAST_BYTE  0x00

/***************************************************************************************************
 * LOCAL TYPEDEFS
 **************************************************************************************************/

/***************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 **************************************************************************************************/

/***************************************************************************************************
 * LOCAL VARIABLES
 **************************************************************************************************/
static uint8_t rxdata[RXDATA_SIZE] = {0};
static uint16_t channels[RADIO_UART_CHANNELS] = {0};
static bsp_uart_radio_callback_t external_callback;
static void uart_callback(void *arg);

static bool stop_uart;
// static bool last_data_broken;


/***************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/

/***************************************************************************************************
 * LOCAL FUNCTIONS
 **************************************************************************************************/

static void uart_callback(void *arg) {
    UNUSED(arg);

    do {
        // invalid packet; Ignore packet
        if ((rxdata[0] != RX_RADIO_FIRST_BYTE) || (rxdata[RXDATA_SIZE - 1] != RX_RADIO_LAST_BYTE)) {
            for (int i = 0; i < RADIO_UART_CHANNELS; i++) {
                channels[i] = (RX_RADIO_MAX_VALUE/2);
            }
            break;
        }

        channels[0] = ((rxdata[1] | rxdata[2] << 8) & 0x07FF);
        channels[1] = ((rxdata[2] >> 3 | rxdata[3] << 5) & 0x07FF);
        channels[2] = ((rxdata[3] >> 6 | rxdata[4] << 2 | rxdata[5] << 10) & 0x07FF);
        channels[3] = ((rxdata[5] >> 1 | rxdata[6] << 7) & 0x07FF);
        channels[4] = ((rxdata[6] >> 4 | rxdata[7] << 4) & 0x07FF);
        channels[5] = ((rxdata[7] >> 7 | rxdata[8] << 1 | rxdata[9] << 9) & 0x07FF);
        channels[6] = ((rxdata[9] >> 2 | rxdata[10] << 6) & 0x07FF);
        channels[7] = ((rxdata[10] >> 5 | rxdata[11] << 3) & 0x07FF);
        channels[8] = ((rxdata[12] | rxdata[13] << 8) & 0x07FF);
        
    } while (0);
    
    external_callback(channels, RADIO_UART_CHANNELS);

    // Continuous receive
    if (!stop_uart){
        HAL_UART_Receive_IT(&huart3, rxdata, RXDATA_SIZE);
    }

}


/***************************************************************************************************
 * GLOBAL FUNCTIONS
 **************************************************************************************************/


void bsp_uart_radio_init() {

    MX_UART4_Init();
    BSP_UART_Register_Callback(UART_NUM_4, uart_callback);

}

void bsp_uart_radio_start() {
    stop_uart = false;
    HAL_UART_Receive_IT(&huart4, rxdata, RXDATA_SIZE);
}

void bsp_uart_radio_stop() {
    stop_uart = true;
}


void bsp_uart_radio_register_callback(bsp_uart_radio_callback_t callback_function){
    external_callback = callback_function;
}


#endif