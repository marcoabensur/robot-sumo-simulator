/***************************************************************************************************
 * INCLUDES
 **************************************************************************************************/

#include "qpc.h"    
#include "bsp.h"

#include "distance_service.h"
#include "bsp_gpio.h"


/***************************************************************************************************
 * LOCAL DEFINES
 **************************************************************************************************/

/***************************************************************************************************
 * LOCAL TYPEDEFS
 **************************************************************************************************/


/***************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 **************************************************************************************************/

static void sensor_data_interrupt(uint8_t sensor_num, io_level_t state);

/***************************************************************************************************
 * LOCAL VARIABLES
 **************************************************************************************************/

static uint16_t distance_sensor_mask = 0xffff;
static volatile bool dist_sensor_is_active[NUM_OF_DIST_SENSORS];

/* Adjust looking at  gpio_dist_sensor_pins in bsp_gpio.c*/
static uint8_t sensor_num_to_position[NUM_OF_DIST_SENSORS] = {
    DIST_SENSOR_DR, /* HARDWARE SILK DIST 1 */
    DIST_SENSOR_R,  /* HARDWARE SILK DIST 2 */
    DIST_SENSOR_FR, /* HARDWARE SILK DIST 3 */
    DIST_SENSOR_F,  /* HARDWARE SILK DIST 6 */
    DIST_SENSOR_FL, /* HARDWARE SILK DIST 7 */
    DIST_SENSOR_L,  /* HARDWARE SILK DIST 8 */
    DIST_SENSOR_DL, /* HARDWARE SILK DIST 9 */
};

/***************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/

/***************************************************************************************************
 * LOCAL FUNCTIONS
 **************************************************************************************************/

static void sensor_data_interrupt(uint8_t sensor_num, io_level_t state){

    if (sensor_num > (NUM_OF_DIST_SENSORS - 1)){
        return;
    }

    dist_sensor_t sensor_position = sensor_num_to_position[sensor_num];
    dist_sensor_is_active[sensor_position] = state;

    bool dist_sensor_enable = distance_sensor_mask & (1 << sensor_position);
    if (dist_sensor_enable){
        QEvt evt = {.sig = DIST_SENSOR_CHANGE_SIG};
        QHSM_DISPATCH(&AO_SumoHSM->super, &evt, SIMULATOR);
    }

    

}

/***************************************************************************************************
 * GLOBAL FUNCTIONS
 **************************************************************************************************/

void distance_service_init() {
    BSP_GPIO_Register_Distance_Callback(sensor_data_interrupt);
}



bool distance_is_active(dist_sensor_t position){

    if (position > NUM_OF_DIST_SENSORS){
        return 0;
    }

    bool dist_sensor_enable = distance_sensor_mask & (1 << position);

    return dist_sensor_is_active[position] & dist_sensor_enable;

}

void distance_service_set_mask(uint16_t mask) {
    distance_sensor_mask = mask;
}
