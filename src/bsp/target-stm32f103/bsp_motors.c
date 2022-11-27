/**
 ***************************************************************************************************
 * @brief   brief
 *
 * @author  author
 **************************************************************************************************/
/***************************************************************************************************
 * INCLUDES
 **************************************************************************************************/
#include <stdint.h>

#include "bsp_motors.h"
#include "tim.h"
#include "gpio.h"
#include "utils.h"


/***************************************************************************************************
 * LOCAL DEFINES
 **************************************************************************************************/
#define MAX_SPEED 100
#define COUNTER_PERIOD_MAX (1000)


/***************************************************************************************************
 * LOCAL TYPEDEFS
 **************************************************************************************************/
typedef struct pwm {
    TIM_HandleTypeDef*    htim;
    uint32_t              channel;
} pwm_t;

typedef struct motor {
    pwm_t red_pwm;
    pwm_t black_pwm;
} motor_t;

/***************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 **************************************************************************************************/

/***************************************************************************************************
 * LOCAL VARIABLES
 **************************************************************************************************/
static motor_t left_motor = {
    .black_pwm = {
        .htim = &htim1,
        .channel = TIM_CHANNEL_3,
    },
    .red_pwm = {
        .htim = &htim1,
        .channel = TIM_CHANNEL_4,
    },
};

static motor_t right_motor = {
    .black_pwm = {
        .htim = &htim1,
        .channel = TIM_CHANNEL_1,
    },
    .red_pwm = {
        .htim = &htim1,
        .channel = TIM_CHANNEL_2,
    },
};
/***************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/

/***************************************************************************************************
 * LOCAL FUNCTIONS
 **************************************************************************************************/
static void pwm_set(pwm_t* pwm, uint32_t value) {
    __HAL_TIM_SET_COMPARE(pwm->htim, pwm->channel, value);
}

static void set_motor_pwm(motor_t* motor, int32_t speed){
   if (speed < 0) {
        pwm_set(&motor->red_pwm, -speed);
        pwm_set(&motor->black_pwm, 0);
    } else {
        pwm_set(&motor->red_pwm, 0);
        pwm_set(&motor->black_pwm, speed);
    }
}

/***************************************************************************************************
 * GLOBAL FUNCTIONS
 **************************************************************************************************/



void BSP_motorsInit(void){
    MX_TIM1_Init(); // Motors Timer
    
    /* Motor Right */
    HAL_TIM_PWM_Start(right_motor.black_pwm.htim, right_motor.black_pwm.channel);
    HAL_TIM_PWM_Start(right_motor.red_pwm.htim, right_motor.red_pwm.channel);

    pwm_set(&right_motor.black_pwm, 0);
    pwm_set(&right_motor.red_pwm, 0);


     /* Motor Left */
    HAL_TIM_PWM_Start(left_motor.black_pwm.htim, left_motor.black_pwm.channel);
    HAL_TIM_PWM_Start(left_motor.red_pwm.htim, left_motor.red_pwm.channel);

    pwm_set(&left_motor.black_pwm, 0);
    pwm_set(&left_motor.red_pwm, 0);

}

void BSP_motors(int16_t vel_left, int16_t vel_right) {

    vel_left = constrain(vel_left, -(MAX_SPEED - 1), (MAX_SPEED - 1));
    vel_right = constrain(vel_right, -(MAX_SPEED - 1), (MAX_SPEED - 1));

    // deadzone
    if (abs(vel_left) < 10) {
        vel_left = 0;
    }

    if (abs(vel_right) < 10) {
        vel_right = 0;
    }

    vel_left = map(vel_left, -MAX_SPEED, MAX_SPEED, -COUNTER_PERIOD_MAX, COUNTER_PERIOD_MAX);
    vel_right = map(vel_right, -MAX_SPEED, MAX_SPEED, -COUNTER_PERIOD_MAX, COUNTER_PERIOD_MAX);

    set_motor_pwm(&left_motor, vel_left);
    set_motor_pwm(&right_motor, vel_right);


}