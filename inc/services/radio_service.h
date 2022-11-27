#ifndef RADIO_SERVICE_H
#define RADIO_SERVICE_H

#include <stdint.h>

#define NUM_OF_RADIO_CHANNELS 8

typedef enum radio_channel 
{
    RADIO_CH1,
    RADIO_CH2,
    RADIO_CH3,
    RADIO_CH4,
    RADIO_CH5,
    RADIO_CH6,
    RADIO_CH7,
    RADIO_CH8,
} radio_channel_t;


void radio_service_init(void);
void radio_service_enable(void);
void radio_service_disable(void);
int8_t radio_service_get_channel(radio_channel_t ch);

#endif /* RADIO_SERVICE_H */
