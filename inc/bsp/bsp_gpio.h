#ifndef BSP_GPIO_H
#define BSP_GPIO_H

#include <stdint.h>
#include <stdbool.h>


typedef enum io_level {

    IO_LOW,
    IO_HIGH

} io_level_t;

typedef enum io_port {

    IO_PORTA,
    IO_PORTB,
    IO_PORTC,
    IO_PORTD

} io_port_t;

typedef enum io_pin {

    IO_PIN_0,
    IO_PIN_1,
    IO_PIN_2,
    IO_PIN_3,
    IO_PIN_4,
    IO_PIN_5,
    IO_PIN_6,
    IO_PIN_7,
    IO_PIN_8,
    IO_PIN_9,
    IO_PIN_10,
    IO_PIN_11,
    IO_PIN_12,
    IO_PIN_13,
    IO_PIN_14,
    IO_PIN_15

} io_pin_t;

typedef void( * bsp_gpio_dist_callback_t) (uint8_t sensor_num, io_level_t state);
typedef void( * bsp_gpio_ppm_callback_t) (uint8_t ppm_num, io_level_t state);
typedef void( * bsp_button_callback_t) (void);


io_level_t BSP_GPIO_Read_Pin(io_port_t port, io_pin_t gpio_pin);
void BSP_GPIO_Write_Pin(io_port_t port, io_pin_t gpio_pin, io_level_t level);
void BSP_GPIO_Toggle_Pin(io_port_t port, io_pin_t gpio_pin);

void BSP_GPIO_Register_Distance_Callback(bsp_gpio_dist_callback_t callback_function);
void BSP_GPIO_Register_Button_Callback(bsp_button_callback_t callback_function);
void BSP_GPIO_Register_PPM_Callback(bsp_gpio_ppm_callback_t callback_function);

#endif /* BSP_GPIO_H */
