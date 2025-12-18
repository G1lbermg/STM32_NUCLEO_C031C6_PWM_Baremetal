#ifndef UART_BSP_H
#define UART_BSP_H

#include "stm32c0xx.h"


void initUART(void);
void printMsg(char *message);
void printMsgNL(char *message);
void transmitByte(const char byte);

#endif
