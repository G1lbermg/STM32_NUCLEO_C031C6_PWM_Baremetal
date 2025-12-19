
#ifndef TIMER3_BSP_H
#define TIMER3_BSP_H

#include "stm32c0xx.h"
#include "stdint.h"
#include "error_codes.h"

/*****************Code to use the timer as a counter *****************************/
ErrorCode_t initCounter_Tmr3(uint32_t targetFreqHz);
ErrorCode_t elapsedTicks_Tmr3(uint32_t *milliSeconds);
ErrorCode_t delayTicks_Tmr3(uint32_t milliSeconds);

/*****************Code to use the timer to drive a PWM signal*****************************/

//--------Timer 3 Channel 1---------
void initPWM_Tim3Ch1(void);
uint8_t setDutyCycle_Tim3Ch1(uint16_t percentage);

#endif
