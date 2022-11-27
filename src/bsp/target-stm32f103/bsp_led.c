#include <stdbool.h>
#include "bsp_led.h"
#include "main.h"
#include "gpio.h"


void BSP_ledInit(void) {
}


void BSP_ledOff(void) { 
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
}


void BSP_ledOn(void)  { 
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
}

void BSP_ledToggle(void)  {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
}

