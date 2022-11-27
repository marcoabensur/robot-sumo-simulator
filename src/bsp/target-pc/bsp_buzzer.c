#include <stdio.h>  
#include "qpc.h"  
#include "bsp.h"

#include "bsp_buzzer.h"

#ifdef Q_SPY
#include "qs_defines.h"

typedef enum {
   QS_BUZZER_START,
   QS_BUZZER_DUTY_CYCLE,
   QS_BUZZER_FREQUENCY,
} buzzer_codes_t;
#endif

void BSP_buzzerInit(void){
    printf("Buzzer init \r\n"); 
}


void BSP_buzzerStart(){
    printf("Buzzer Start \r\n"); 
    QS_BEGIN_ID(SIMULATOR, AO_SumoHSM->prio)
       QS_U8(1, QS_BUZZER_ID);
       QS_U8(1, QS_BUZZER_START);
       QS_U8(1, 1);
       QS_U8(1, 0);
    QS_END()

}

void BSP_buzzerStop(){
    printf("Buzzer Stop \r\n"); 
    QS_BEGIN_ID(SIMULATOR, AO_SumoHSM->prio)
        QS_U8(1, QS_BUZZER_ID);
        QS_U8(1, QS_BUZZER_START);
        QS_U8(1, 0);
        QS_U8(1, 0);
    QS_END()

}

void BSP_buzzerSetPwmDutyCycle(uint8_t duty_cycle){
    printf("Buzzer Duty Cycle %d\r\n", duty_cycle); 
    QS_BEGIN_ID(SIMULATOR, AO_SumoHSM->prio)
        QS_U8(1, QS_BUZZER_ID);
        QS_U8(1, QS_BUZZER_DUTY_CYCLE);
        QS_U8(1, duty_cycle);
        QS_U8(1, 0);
    QS_END()

}

void BSP_buzzerSetFrequency(uint16_t frequency_hz){
    printf("Buzzer frequency_hz %d\r\n", frequency_hz); 

    QS_BEGIN_ID(SIMULATOR, AO_SumoHSM->prio)
        QS_U8(1, QS_BUZZER_ID);
        QS_U8(1, QS_BUZZER_FREQUENCY);
        QS_U8(1, (uint8_t)(frequency_hz >> 8));
        QS_U8(1, (uint8_t)(frequency_hz & 0xff));
    QS_END()
}