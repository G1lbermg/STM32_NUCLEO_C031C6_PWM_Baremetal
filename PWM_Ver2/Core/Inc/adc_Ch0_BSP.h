#ifndef ADC_CH0_BSP_H
#define ADC_CH0_BSP_H

#include "stm32c0xx.h"
#include <stdint.h>
#include "stm32c0xx_ll_utils.h"

extern volatile uint16_t dataADC1;
extern volatile uint8_t flagADC1Data;

void initADC_Ch0(void);
void runADC_Ch0(void);

//void runADC_Ch0(void);

#endif
