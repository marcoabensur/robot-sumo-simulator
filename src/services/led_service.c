/***************************************************************************************************
 * INCLUDES
 **************************************************************************************************/

#include "led_service.h"
#include "bsp_led.h"
#include "bsp_ws2812.h"
#include "utils.h"

/***************************************************************************************************
 * LOCAL DEFINES
 **************************************************************************************************/
#define LED_STRIPE_NUM  16
/***************************************************************************************************
 * LOCAL TYPEDEFS
 **************************************************************************************************/

/***************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 **************************************************************************************************/
static void led_stripe_prepare(uint8_t idx, color_t color);
/***************************************************************************************************
 * LOCAL VARIABLES
 **************************************************************************************************/

/***************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/
color_t color_purple = {
    .R = 0x94,
    .G = 0x00,
    .B = 0xD3,
};

color_t color_red = {
    .R = 0xff,
    .G = 0x00,
    .B = 0x00,
};

color_t color_blue = {
    .R = 0x00,
    .G = 0x00,
    .B = 0xff,
};

/***************************************************************************************************
 * LOCAL FUNCTIONS
 **************************************************************************************************/

static void led_stripe_prepare(uint8_t idx, color_t color){
    BSP_ws2812_set(idx, color.R, color.G, color.B);
}

/***************************************************************************************************
 * GLOBAL FUNCTIONS
 **************************************************************************************************/
void board_led_toggle(){
    BSP_ledToggle();
}

void board_led_on(){
    BSP_ledOn();
}

void board_led_off(){
    BSP_ledOff();
}

// Led Stripe

void led_stripe_init(){
    BSP_ws2812_init();
    led_stripe_reset();
}

void led_stripe_send(){
    BSP_ws2812_send();
}


void led_stripe_set(uint8_t idx, color_t color){
    led_stripe_prepare(idx, color);
    led_stripe_send();
}


void led_stripe_set_range(uint8_t from, uint8_t to, color_t color){

    to = min(to, LED_STRIPE_NUM);

    for (uint8_t i = from; i < to; i++) {
        led_stripe_prepare(i, color);
    }

    led_stripe_send();
}


void led_stripe_set_all(color_t color){
    led_stripe_set_range(0, LED_STRIPE_NUM, color);
}

void led_stripe_reset(){
    color_t color_black = {
        .R = 0x00,
        .G = 0x00,
        .B = 0x00,
    };

    led_stripe_set_all(color_black);
}


void led_stripe_set_strategy_color(uint8_t strategy_num){

    color_t color;

    switch (strategy_num) {
        case 0:
            color.R = 0xff;
            color.G = 0x00;
            color.B = 0x00;
            break;
        case 1:
            color.R = 0x00;
            color.G = 0xff;
            color.B = 0x00;
            break;
        case 2:
            color.R = 0x00;
            color.G = 0x00;
            color.B = 0xff;
            break;
        
        default:
            color.R = 0x00;
            color.G = 0x00;
            color.B = 0x00;
            break;
    }

    led_stripe_set_range(0, (LED_STRIPE_NUM / 2), color);

}

void led_stripe_set_pre_strategy_color(uint8_t pre_strategy_num){

    color_t color;


    switch (pre_strategy_num) {
        case 0:
            color.R = 0xff;
            color.G = 0x00;
            color.B = 0x00;
            break;
        case 1:
            color.R = 0x00;
            color.G = 0xff;
            color.B = 0x00;
            break;
        case 2:
            color.R = 0x00;
            color.G = 0x00;
            color.B = 0xff;
            break;
        
        case 3:
            color.R = 0x00;
            color.G = 0xff;
            color.B = 0xff;
            break;

        default:
            color.R = 0x00;
            color.G = 0x00;
            color.B = 0x00;
            break;
    }

    led_stripe_set_range(8, LED_STRIPE_NUM, color);

}
