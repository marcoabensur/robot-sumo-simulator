#ifndef BSP_BLE_H
#define BSP_BLE_H

#include <stdint.h>

#define BLE_RECEIVE_PACKET_SIZE     12
#define BLE_MAX_PACKET_SIZE         20


typedef void( * bsp_uart_ble_callback_t) (uint8_t * ble_data, uint8_t rcv_size);


void bsp_ble_init(void);
void bsp_ble_start(void);
void bsp_ble_stop(void);
void bsp_ble_transmit(uint8_t * data, uint8_t size);
void bsp_ble_register_callback(bsp_uart_ble_callback_t callback_function); 



#endif /* BSP_BLE_H */
