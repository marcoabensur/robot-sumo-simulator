#ifndef BSP_GPIO_MAPPING_H
#define BSP_GPIO_MAPPING_H

#include "bsp_gpio.h"

#define BOARD_NUM_DIST_SENSORS  7
#define BOARD_NUM_OF_PPMS       4

// Button
#define GPIO_BUTTON_PIN          IO_PIN_15
#define GPIO_BUTTON_PORT         IO_PORTC

// MOT EN
#define GPIO_MOT_EN_PIN          IO_PIN_12
#define GPIO_MOT_EN_PORT         IO_PORTA

// Start Module
#define GPIO_START_MODULE_PIN    IO_PIN_2
#define GPIO_START_MODULE_PORT   IO_PORTA


// PPMS 
#define GPIO_PPM_1_PIN           IO_PIN_6
#define GPIO_PPM_1_PORT          IO_PORTC

#define GPIO_PPM_2_PIN           IO_PIN_7
#define GPIO_PPM_2_PORT          IO_PORTC

#define GPIO_PPM_3_PIN           IO_PIN_8
#define GPIO_PPM_3_PORT          IO_PORTC

#define GPIO_PPM_4_PIN           IO_PIN_9
#define GPIO_PPM_4_PORT          IO_PORTC


// Distance Sensors
#define GPIO_DIST_SENSOR_1_PIN   IO_PIN_0      
#define GPIO_DIST_SENSOR_1_PORT  IO_PORTB      

#define GPIO_DIST_SENSOR_2_PIN   IO_PIN_14     
#define GPIO_DIST_SENSOR_2_PORT  IO_PORTC      

#define GPIO_DIST_SENSOR_3_PIN   IO_PIN_13      
#define GPIO_DIST_SENSOR_3_PORT  IO_PORTC      

// #define GPIO_DIST_SENSOR_4_PIN   IO_PIN_10      
// #define GPIO_DIST_SENSOR_4_PORT  IO_PORTC      

// #define GPIO_DIST_SENSOR_5_PIN   IO_PIN_11      
// #define GPIO_DIST_SENSOR_5_PORT  IO_PORTC      

#define GPIO_DIST_SENSOR_6_PIN   IO_PIN_12      
#define GPIO_DIST_SENSOR_6_PORT  IO_PORTC      

#define GPIO_DIST_SENSOR_7_PIN   IO_PIN_2      
#define GPIO_DIST_SENSOR_7_PORT  IO_PORTD      

#define GPIO_DIST_SENSOR_8_PIN   IO_PIN_4      
#define GPIO_DIST_SENSOR_8_PORT  IO_PORTB      

#define GPIO_DIST_SENSOR_9_PIN   IO_PIN_5      
#define GPIO_DIST_SENSOR_9_PORT  IO_PORTB      


#endif /* BSP_GPIO_MAPPING_H */