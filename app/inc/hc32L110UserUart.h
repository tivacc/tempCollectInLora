#ifndef _USER_UART_H_
#define _USER_UART_H_

#include "ddl.h"
#include "uart.h"
#include "bt.h"
#include "gpio.h"

void hc32L110UartInit(void);

void hc32L110UartSend(uint8_t dataLen, uint8_t *dataBuffer);

#endif
