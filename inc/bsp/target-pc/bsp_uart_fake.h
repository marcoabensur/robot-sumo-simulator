#ifndef BSP_UART_FAKE_H
#define BSP_UART_FAKE_H

#include <stdint.h>

void HAL_UART_Fake_UartData(uint8_t uart_num, int16_t* data);
void HAL_UART_Fake_UartError(uint8_t uart_num);

#endif /* BSP_UART_FAKE_H */
