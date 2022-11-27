#include <stdint.h>
#include "bsp_buzzer.h"
#include "bsp_gpio.h"
#include "tim.h"


#define CPU_FREQUENCY  (72000000UL)
#define COUNTER_PERIOD (100)


#ifdef STM32F103xG
void BSP_buzzerInit(){
    MX_TIM12_Init();
}


void BSP_buzzerStart(){
    
    HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);

}

void BSP_buzzerStop(){
    
    HAL_TIM_PWM_Stop(&htim12, TIM_CHANNEL_1);

}

void BSP_buzzerSetPwmDutyCycle(uint8_t duty_cycle){
    
    __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, duty_cycle);
    
}

void BSP_buzzerSetFrequency(uint16_t frequency_hz){
    // frequency_hz = (CPU_FREQUENCY/ COUNTER_PERIOD * (preescaler_value + 1)) ->
    
    uint16_t preescaler_value = ((CPU_FREQUENCY / COUNTER_PERIOD) / frequency_hz) - 1;
    __HAL_TIM_SET_PRESCALER(&htim12, preescaler_value);
}



#elif defined (STM32F103xE)
void BSP_buzzerInit(){
    MX_TIM3_Init();
}


void BSP_buzzerStart(){
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
}

void BSP_buzzerStop(){
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
    BSP_GPIO_Write_Pin(IO_PORTB, IO_PIN_14, IO_LOW);
}

void BSP_buzzerSetPwmDutyCycle(uint8_t duty_cycle){
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, duty_cycle);
}

void BSP_buzzerSetFrequency(uint16_t frequency_hz){
    uint16_t preescaler_value = ((CPU_FREQUENCY / COUNTER_PERIOD) / frequency_hz) - 1;
    __HAL_TIM_SET_PRESCALER(&htim3, preescaler_value);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

    if (htim->Instance == TIM3){
        BSP_GPIO_Toggle_Pin(IO_PORTB, IO_PIN_14);
    }

}

#endif