#include <stdlib.h>
#include "bsp_uart.h"
#include "bsp_uart_fake.h"

static bsp_uart_callback_t uart_custom_callbacks[MAX_UART_NUM];
static bsp_uart_callback_t uart_custom_error_callbacks[MAX_UART_NUM];


void HAL_UART_Fake_UartData(uint8_t uart_num, int16_t* data) {

    if (uart_num == UART_NUM_3){
        if (uart_custom_callbacks[UART_NUM_3] != NULL){
            uart_custom_callbacks[UART_NUM_3](data);
        }
    } else if (uart_num == UART_NUM_4){
        if (uart_custom_callbacks[UART_NUM_4] != NULL){
            uart_custom_callbacks[UART_NUM_4](data);
        }
    }


}

void HAL_UART_Fake_UartError(uint8_t uart_num){
	if (uart_num == UART_NUM_3){
        if (uart_custom_error_callbacks[UART_NUM_3] != NULL){
            uart_custom_error_callbacks[UART_NUM_3](NULL);
        }
    } else if (uart_num == UART_NUM_4){
        if (uart_custom_error_callbacks[UART_NUM_4] != NULL){
            uart_custom_error_callbacks[UART_NUM_4](NULL);
        }
    }
}




void BSP_UART_Register_Callback(uart_num_t uart_num, bsp_uart_callback_t callback_function){
    
    if (uart_num >= MAX_UART_NUM){
        return;
    }

    uart_custom_callbacks[uart_num] = callback_function;
}

void BSP_UART_Register_Error_Callback(uart_num_t uart_num, bsp_uart_callback_t callback_function){
    
    if (uart_num >= MAX_UART_NUM){
        return;
    }

    uart_custom_error_callbacks[uart_num] = callback_function;
}