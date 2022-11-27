#ifndef BUZZER_SERVICE_H
#define BUZZER_SERVICE_H

#include <stdint.h>

void buzzer_service_init();
void buzzer_set_intensity(uint8_t volume);
void buzzer_set_frequency(uint16_t frequency_hz);
void buzzer_start();
void buzzer_stop();
void buzzer_toggle();

#endif /* BUZZER_SERVICE_H */
