#ifndef BSP_UART_H
#define BSP_UART_H

#define MAX_UART_NUM    5

typedef void( * bsp_uart_callback_t) (void *args);

typedef enum {

    UART_NUM_1,
    UART_NUM_2,
    UART_NUM_3,
    UART_NUM_4,
    UART_NUM_5,

} uart_num_t;


void BSP_UART_Register_Callback(uart_num_t uart_num, bsp_uart_callback_t callback_function);
void BSP_UART_Register_Error_Callback(uart_num_t uart_num, bsp_uart_callback_t callback_function);



#endif /* BSP_UART_H */
