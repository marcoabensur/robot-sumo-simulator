#ifndef DISTANCE_SERVICE_H
#define DISTANCE_SERVICE_H

#include <stdint.h>
#include <stdbool.h>

#define NUM_OF_DIST_SENSORS 7

typedef enum dist_sensor 
{
    DIST_SENSOR_L,
    DIST_SENSOR_FL,
    DIST_SENSOR_DL,
    DIST_SENSOR_F,
    DIST_SENSOR_DR,
    DIST_SENSOR_FR,
    DIST_SENSOR_R,
} dist_sensor_t;



void distance_service_init(void);
bool distance_is_active(dist_sensor_t position);
void distance_service_set_mask(uint16_t mask);

#endif /* DISTANCE_SERVICE_H */
