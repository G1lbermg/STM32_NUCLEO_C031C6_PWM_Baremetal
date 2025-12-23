#ifndef ADC_CH0_BSP_H
#define ADC_CH0_BSP_H

#include "error_codes.h"
#include "stm32c0xx.h"
#include "stm32c0xx_ll_utils.h"
#include <stdint.h>

extern volatile uint16_t dataADC1;
extern volatile uint8_t flagADC1Data;

ErrorCode_t initADC_Ch0(void);
ErrorCode_t runADC_Ch0(void);

#endif
