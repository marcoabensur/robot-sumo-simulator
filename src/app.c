#include "app.h"
#include "driving_service.h"
#include "distance_service.h"
#include "led_service.h"
#include "adc_service.h"
#include "button.h"
#include "buzzer_service.h"
#include "radio_service.h"
#include "bsp_eeprom.h"
#include "ble_service.h"

void APP_init(){

    distance_service_init();
    driving_init();
    led_stripe_init();
    adc_service_init();
    button_init();
    buzzer_service_init();
    radio_service_init();
    BSP_eeprom_init();
    ble_service_init();

}