#ifndef BSP_BUZZER_H
#define BSP_BUZZER_H

#include <stdint.h>


void BSP_buzzerInit(void);
void BSP_buzzerStart();
void BSP_buzzerStop();
void BSP_buzzerSetPwmDutyCycle(uint8_t duty_cycle);
void BSP_buzzerSetFrequency(uint16_t frequency_hz);



#endif /* BSP_BUZZER_H */
