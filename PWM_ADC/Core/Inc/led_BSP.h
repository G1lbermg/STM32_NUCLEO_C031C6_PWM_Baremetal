#ifndef LED_BSP_H
#define LED_BSP_H

#include "stm32c0xx.h"
#include "error_codes.h"
#include <stdint.h>

typedef struct {
    GPIO_TypeDef *port;
    uint32_t setMask;     // BSRR mask to turn on
    uint32_t resetMask;   // BSRR mask to turn off
} LED_t;

ErrorCode_t initLED(LED_t *led,GPIO_TypeDef *port, uint8_t pinNum);
ErrorCode_t turnOnLED(LED_t *led);
ErrorCode_t turnOffLED(LED_t *led);


#endif
