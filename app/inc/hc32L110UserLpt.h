#ifndef _USER_LPT_H_
#define _USER_LPT_H_

#include <stdio.h>
#include <stdint.h>

#include "ddl.h"
#include "gpio.h"
#include "lpt.h"


uint16_t hc32L110LptMs2Cnt(uint16_t msValue);

uint16_t hc32L110LptCnt2Ms(uint16_t cntValue);

void hc32L110LptInit(void);

uint16_t  hc32L110LptGetCounter(void);

void hc32L110LptStart(uint16_t timeValue);

void hc32L110LptStop(void);

#endif
