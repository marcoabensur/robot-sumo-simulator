#include <stdio.h>  
#include <stdbool.h>

#include "qpc.h"  
#include "bsp.h"
#include "bsp_ws2812.h"

#ifdef Q_SPY
#include "qs_defines.h"
#endif

#define WS2812_MAX_LED_AMOUNT 16


uint8_t led_stripe[WS2812_MAX_LED_AMOUNT][3]; 

void BSP_ws2812_init(){
    printf("BSP_WS2812 init \r\n");

}


void BSP_ws2812_set(uint8_t num, uint8_t r, uint8_t g, uint8_t b) {

    printf("WS2812 Num %d RGB = %x,%x,%x\n", num, r, g, b); 

     if (num >= WS2812_MAX_LED_AMOUNT) {
        printf("num >= WS2812_MAX_LED_AMOUNT\r\n");
        return;
    }

    led_stripe[num][0] = r;
    led_stripe[num][1] = g;
    led_stripe[num][2] = b;


}

void BSP_ws2812_send() {

    printf("WS2812 Send\n"); 

    for (int i = 0; i < WS2812_MAX_LED_AMOUNT; i++)
    {  
        QS_BEGIN_ID(SIMULATOR, AO_SumoHSM->prio);
        QS_U8(1, QS_LED_STRIPE_ID);
        QS_U8(1, i); // Index
        QS_U8(1, led_stripe[i][0]); // R
        QS_U8(1, led_stripe[i][1]); // G
        QS_U8(1, led_stripe[i][2]); // B
        QS_END()
    }
    

    
}
