#ifndef BSP_UART_CRSF_H
#define BSP_UART_CRSF_H

#include <stdint.h>

#define RADIO_CRSF_MIN_VALUE  0
#define RADIO_CRSF_MAX_VALUE  2047


typedef void( * bsp_uart_crsf_callback_t) (uint16_t * ch_data, uint8_t ch_amount);

void bsp_uart_crsf_init(); 
void bsp_uart_crsf_start();
void bsp_uart_crsf_stop();

void bsp_uart_crsf_register_callback(bsp_uart_crsf_callback_t callback_function); 


#endif /* BSP_UART_CRSF_H */
