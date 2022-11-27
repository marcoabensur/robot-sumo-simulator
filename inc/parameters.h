#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stdint.h>

typedef struct sumo_parameters {

    uint8_t  current_strategy;
    uint8_t  current_pre_strategy;

    uint16_t enabled_distance_sensors;
    uint8_t  enabled_line_sensors;

    uint8_t  star_speed;
    uint8_t  max_speed;
    
    uint8_t  reverse_speed; 
    uint16_t reverse_time_ms;
    uint8_t  turn_speed;

    uint16_t turn_180_time_ms;

    uint16_t step_wait_time_ms;
    uint16_t step_advance_time_ms;
    

} sumo_parameters_t;

void parameters_report(sumo_parameters_t params, uint8_t config_num);
void parameters_init(sumo_parameters_t *params);
void parameters_update_from_ble(sumo_parameters_t *params, uint8_t * last_data);


#endif /* PARAMETERS_H */
