#ifndef DRIVING_SERVICE_H
#define DRIVING_SERVICE_H

#include <stdint.h>

void driving_init();

void driving_enable();

void driving_disable();

void drive(int16_t left, int16_t right);

#endif /* DRIVING_SERVICE_H */
