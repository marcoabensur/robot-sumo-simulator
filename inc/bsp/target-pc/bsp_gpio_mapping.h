#ifndef BSP_GPIO_MAPPING_H
#define BSP_GPIO_MAPPING_H

#include "bsp_gpio.h"

#define BOARD_NUM_DIST_SENSORS  7
#define BOARD_NUM_OF_PPMS       4


// Button
#define GPIO_BUTTON_PIN          IO_PIN_15
#define GPIO_BUTTON_PORT         IO_PORTA

// MOT EN
#define GPIO_MOT_EN_PIN          IO_PIN_0
#define GPIO_MOT_EN_PORT         IO_PORTB

// Start Module
#define GPIO_START_MODULE_PIN    IO_PIN_2
#define GPIO_START_MODULE_PORT   IO_PORTB

// Distance Sensors
#define GPIO_DIST_SENSOR_1_PIN   IO_PIN_1     
#define GPIO_DIST_SENSOR_1_PORT  IO_PORTA      

#define GPIO_DIST_SENSOR_2_PIN   IO_PIN_2      
#define GPIO_DIST_SENSOR_2_PORT  IO_PORTA      

#define GPIO_DIST_SENSOR_3_PIN   IO_PIN_3      
#define GPIO_DIST_SENSOR_3_PORT  IO_PORTA      

// #define GPIO_DIST_SENSOR_4_PIN   IO_PIN_4      
// #define GPIO_DIST_SENSOR_4_PORT  IO_PORTA      

// #define GPIO_DIST_SENSOR_5_PIN   IO_PIN_5      
// #define GPIO_DIST_SENSOR_5_PORT  IO_PORTA      

#define GPIO_DIST_SENSOR_6_PIN   IO_PIN_4      
#define GPIO_DIST_SENSOR_6_PORT  IO_PORTA      

#define GPIO_DIST_SENSOR_7_PIN   IO_PIN_5      
#define GPIO_DIST_SENSOR_7_PORT  IO_PORTA      

#define GPIO_DIST_SENSOR_8_PIN   IO_PIN_6      
#define GPIO_DIST_SENSOR_8_PORT  IO_PORTA      

#define GPIO_DIST_SENSOR_9_PIN   IO_PIN_7      
#define GPIO_DIST_SENSOR_9_PORT  IO_PORTA      


#endif /* BSP_GPIO_MAPPING_H */