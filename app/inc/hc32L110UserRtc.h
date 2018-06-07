#ifndef _HC32l110_RTC_H
#define _HC32l110_RTC_H


#include <stdio.h>
#include <stdint.h>

#include "ddl.h"
#include "gpio.h"
#include "rtc.h"


void hc32L110RtcInit(void);

uint32_t hc32L110RtcGetCounter(void);

uint32_t hc32L110RtcElapse(uint32_t startTime);


#endif

