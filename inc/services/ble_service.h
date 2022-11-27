#ifndef BLE_SERVICE_H
#define BLE_SERVICE_H

#include <stdint.h>
#include "bsp_ble.h"


typedef enum  {
    BLE_REQUEST_DATA,
    BLE_UPDATE_PARAMETERS,
    BLE_CHANGE_STATE,
} ble_data_header_t;

typedef union{
    uint8_t _raw[BLE_RECEIVE_PACKET_SIZE];

    struct {
        uint8_t _header;
        uint8_t enabledDistanceSensors;
        uint8_t enabledLineSensors;
        uint8_t reverseSpeed;
        uint8_t reverseTimeMs;
        uint8_t turnSpeed;
        uint8_t turnTimeMs;
        uint8_t stepWaitTimeMs;
        uint8_t preStrategy;
        uint8_t strategy;
        uint8_t maxMotorSpeed;
        uint8_t _chk;
    };

} ble_rcv_packet_t;

void ble_service_init(void);

void ble_service_send_data(uint8_t * data, uint8_t size);
void ble_service_last_packet(ble_rcv_packet_t * data);
ble_data_header_t ble_service_last_packet_type(void);

#endif /* BLE_SERVICE_H */
