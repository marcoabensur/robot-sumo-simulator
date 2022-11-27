/***************************************************************************************************
 * INCLUDES
 **************************************************************************************************/

#include "qpc.h"    
#include "bsp.h"

#include "button.h"
#include "bsp_gpio.h"


static void button_interrupt(){
    QEvt evt = {.sig = BUTTON_SIG};
    QHSM_DISPATCH(&AO_SumoHSM->super, &evt, SIMULATOR); 
}


void button_init() {
    BSP_GPIO_Register_Button_Callback(button_interrupt);
}



