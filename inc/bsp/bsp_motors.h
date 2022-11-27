#ifndef BSP_MOTORS_H
#define BSP_MOTORS_H

#include <stdint.h>


void BSP_motorsInit(void);
void BSP_motors(int16_t vel_left, int16_t vel_right);


#endif /* BSP_MOTORS_H */
