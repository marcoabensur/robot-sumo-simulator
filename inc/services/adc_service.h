#ifndef ADC_SERVICE_H
#define ADC_SERVICE_H

#include <stdint.h>
#include <stdbool.h>

#define NUM_OF_LINE_SENSORS 4

// Position of sensor on ADC DMA buffer
typedef enum line_sensor 
{
    LINE_FR, 
    LINE_FL,
    LINE_BR,      
    LINE_BL,
} line_sensor_t;


void adc_service_init(void);
bool adc_line_is_white(line_sensor_t position);
bool adc_get_low_battery(void);
double adc_get_battery_mv(void);
void adc_line_set_mask(uint8_t mask);

#endif /* ADC_SERVICE_H */
