#include "driving_service.h"
#include "bsp_motors.h"
#include "bsp_gpio.h"
#include "bsp_gpio_mapping.h"
#include "utils.h"

void driving_init(){
    BSP_motorsInit();
}

void driving_enable(){
    BSP_GPIO_Write_Pin(GPIO_MOT_EN_PORT, GPIO_MOT_EN_PIN, IO_LOW);
}

void driving_disable(){
    BSP_GPIO_Write_Pin(GPIO_MOT_EN_PORT, GPIO_MOT_EN_PIN, IO_HIGH);
}

void drive(int16_t left, int16_t right){

    left = constrain(left, -99, 99);
    right = constrain(right, -99, 99);

    if (abs(left) < 10) {
        left = 0;
    }

    if (abs(right) < 10) {
        right = 0;
    }

    BSP_motors(left, right);

}