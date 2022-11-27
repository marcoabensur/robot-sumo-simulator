#ifndef LED_SERVICE_H
#define LED_SERVICE_H

#include <stdint.h>

typedef struct color {
    uint8_t R;
    uint8_t B;
    uint8_t G;
} color_t;

extern color_t color_purple;
extern color_t color_red;
extern color_t color_blue;

void board_led_toggle();
void board_led_on();
void board_led_off();

void led_stripe_init();
void led_stripe_send();

void led_stripe_set(uint8_t idx, color_t color);
void led_stripe_set_range(uint8_t from, uint8_t to, color_t color);
void led_stripe_set_all(color_t color);

void led_stripe_reset();
void led_stripe_send();

void led_stripe_set_strategy_color(uint8_t strategy_num);
void led_stripe_set_pre_strategy_color(uint8_t pre_strategy_num);


#endif /* LED_SERVICE_H */
