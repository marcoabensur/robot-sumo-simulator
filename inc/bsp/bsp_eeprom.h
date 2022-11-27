#ifndef BSP_EEPROM_H
#define BSP_EEPROM_H

#include <stdint.h>


#define EEPROM_NB_OF_VAR    ((uint8_t)0x02)

typedef enum eeprom_result {

    EEPROM_OK,
    EEPROM_ERROR,
    EEPROM_DATA_NOT_FOUND,

} eeprom_result_t;


typedef enum eeprom_addresses {

    EN_DIST_SENSOR_ADDR,
    EN_LINE_SENSOR_ADDR,

    STAR_SPEED_ADDR,
    MAX_SPEED_ADDR,

    REVERSE_SPEED_ADDR,
    REVERSE_TIME_MS_ADDR,

    TURN_SPEED_ADDR,
    TURN_180_TIME_ADDR,

    STEP_WAIT_TIME_MS_ADDR,
    STEP_ADVANCE_TIME_MS_ADDR,

} eeprom_addresses_t;


eeprom_result_t BSP_eeprom_init();
eeprom_result_t BSP_eeprom_read(uint16_t address, uint16_t* data);
eeprom_result_t BSP_eeprom_write(uint16_t address, uint16_t data);


#endif /* BSP_EEPROM_H */
