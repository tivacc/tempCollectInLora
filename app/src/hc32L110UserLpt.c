#include <stdio.h>
#include <stdint.h>

#include "timeService.h"
#include "hc32L110UserLpt.h"
#include "hc32L110UserUart.h"

void lptIntCallback(void)
{
	Lpt_Stop();
	if (TRUE == Lpt_GetIntFlag())
    {
        Lpt_ClearIntFlag();
    }
	
	timerIrqHandler();
}


uint16_t hc32L110LptMs2Cnt(uint16_t msValue)
{
	uint16_t cntValue = 0;
	uint16_t xtlFreq = 32768;
	uint32_t tempValue = xtlFreq*msValue;

	cntValue = (uint16_t)(tempValue/1000);

	return cntValue;	
}

uint16_t hc32L110LptCnt2Ms(uint16_t cntValue)
{
	uint16_t msValue = 0;
	uint16_t xtlFreq = 32768;
	uint32_t tempValue = cntValue*1000;
	
	msValue = (uint16_t)(tempValue/xtlFreq);
	return msValue;	
}


void hc32L110LptInit(void)
{
	timerListInit();

	stc_lpt_config_t stcConfig;
	DDL_ZERO_STRUCT(stcConfig);

	Clk_Enable(ClkXTL/*ClkRCL*/, TRUE);
	Clk_SetPeripheralGate(ClkPeripheralLpTim, TRUE);
    
    //INT ENABLE
    EnableNvic(LPTIM_IRQn, 3, TRUE);
    Lpt_EnableIrq();
    
    stcConfig.pfnLpTimCb = lptIntCallback;
    
    stcConfig.enGateP  = LptPositive;
    stcConfig.enGate   = LptGateDisable;
    stcConfig.enTckSel = LptXIL32K;//LptIRC32K;
    stcConfig.enTog    = LptTogDisable;
    stcConfig.enCT     = LptTimer;//LptCounter;
    stcConfig.enMD     = LptMode1;
        
    Lpt_Init(&stcConfig);
}


uint16_t  hc32L110LptGetCounter(void)
{
	uint16_t rtcCnt = 0;
	rtcCnt = Lpt_Cnt16Get();
	return rtcCnt;
}


void hc32L110LptStart(uint16_t timeValue)
{
	uint16_t timerCnt = 0;
	
	Lpt_Stop();  	
	
    timerCnt = 65535-hc32L110LptMs2Cnt(timeValue);
    Lpt_ARRSet(timerCnt);
    Lpt_Run();
}

void hc32L110LptStop(void)
{
	Lpt_Stop();
}


