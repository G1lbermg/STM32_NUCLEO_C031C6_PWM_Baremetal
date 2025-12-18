#ifndef TMR3_PWM_BSP_H
#define TMR3_PWM_BSP_H

#include "stm32c0xx.h"
#include <stdint.h>


void initPWM_Tim3Ch1(void);
uint8_t setDutyCycle_Tim3Ch1(uint16_t percentage);

/*
void brightenLED(void);
void dimLED(void);
*/

#endif
