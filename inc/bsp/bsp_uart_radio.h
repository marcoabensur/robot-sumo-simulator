#ifndef BSP_UART_RADIO_H
#define BSP_UART_RADIO_H

#include <stdint.h>

#define RX_RADIO_MIN_VALUE  0
#define RX_RADIO_MAX_VALUE  2047


typedef void( * bsp_uart_radio_callback_t) (uint16_t * ch_data, uint8_t ch_amount);

void bsp_uart_radio_init(); 
void bsp_uart_radio_start();
void bsp_uart_radio_stop();

void bsp_uart_radio_register_callback(bsp_uart_radio_callback_t callback_function); 


#endif /* BSP_UART_RADIO_H */
