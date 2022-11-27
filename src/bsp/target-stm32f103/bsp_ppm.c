/***************************************************************************************************
 * INCLUDES
 **************************************************************************************************/

#ifdef RADIO_MODE_PPM

#include "tim.h"
#include "gpio.h"
#include "bsp_gpio_mapping.h"
#include "bsp_gpio.h"
#include "bsp_ppm.h"

/***************************************************************************************************
 * LOCAL DEFINES
 **************************************************************************************************/

#define RADIO_FAILSAFE_MAX_TIMER_OVERFLOW 10

#define PPM_TIMER (&htim7)
/***************************************************************************************************
 * LOCAL TYPEDEFS
 **************************************************************************************************/


typedef struct ppm_input {
    uint16_t           value;
    uint16_t           _tmp_count;
    uint32_t           _fail_count;
} ppm_input_t;
/***************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 **************************************************************************************************/

/***************************************************************************************************
 * LOCAL VARIABLES
 **************************************************************************************************/
static volatile ppm_input_t ppms[BOARD_NUM_OF_PPMS];
static bsp_ppm_callback_t external_callback;
/***************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/

/***************************************************************************************************
 * LOCAL FUNCTIONS
 **************************************************************************************************/


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    for (size_t i = 0; i < BOARD_NUM_OF_PPMS; i++) {
        if (htim->Instance == PPM_TIMER->Instance) {
            ppms[i]._fail_count++;

            if (ppms[i]._fail_count > RADIO_FAILSAFE_MAX_TIMER_OVERFLOW) {
                ppms[i].value = PPM_STOPPED_VALUE_MS;
            }
        }
    }
}

static void ppm_exti_callback(uint8_t ppm_num, io_level_t state) {
    if (state == IO_HIGH) {
        ppms[ppm_num]._tmp_count = __HAL_TIM_GET_COUNTER(PPM_TIMER);
    } else {
        ppms[ppm_num].value = __HAL_TIM_GET_COUNTER(PPM_TIMER) - ppms[ppm_num]._tmp_count;
        ppms[ppm_num]._fail_count = 0;
        external_callback(ppm_num, ppms[ppm_num].value);
    }

}

/***************************************************************************************************
 * GLOBAL FUNCTIONS
 **************************************************************************************************/


void bsp_ppm_init(){

    for (int i = 0; i < BOARD_NUM_OF_PPMS; i++) {
        ppms[i].value = PPM_STOPPED_VALUE_MS;
        ppms[i]._fail_count = 0;
        ppms[i]._tmp_count = 0;
    }

    BSP_GPIO_Register_PPM_Callback(ppm_exti_callback);
    
    MX_TIM7_Init();

    bsp_ppm_start();
    
}

void bsp_ppm_start(){
    HAL_TIM_Base_Start_IT(&htim7);
    __HAL_TIM_SET_COUNTER(&htim7, 0);
}

void bsp_ppm_stop(){
    HAL_TIM_Base_Stop_IT(&htim7);
}

void bsp_ppm_register_callback(bsp_ppm_callback_t callback_function){
    external_callback = callback_function;
}

#endif