
#ifndef TIMER3_BSP_H
#define TIMER3_BSP_H

#include "stm32c0xx.h"
#include "stdint.h"
#include "error_codes.h"

/*****************Code to use the timer as a counter *****************************/
void startCounter_Tmr3(void);
ErrorCode_t initCounter_Tmr3(uint32_t targetFreqHz);
ErrorCode_t elapsedTicks_Tmr3(uint32_t *ticks);
ErrorCode_t delayTicks_Tmr3(uint32_t ticks);

/*****************Code to use the timer to drive a PWM signal*****************************/

//--------Timer 3 Channel 1---------
ErrorCode_t initPWM_Tim3Ch1(void);
ErrorCode_t setDutyCycle_Tim3Ch1(uint16_t percentage);

#endif
