#include <stdio.h>
#include <stdint.h>

#include "hc32L110UserRtc.h"

static uint32_t hc32L110RtcGetCalendarValue( stc_rtc_time_t rtcValue );

static void RtcCycCb(void)
{
}
static void RtcAlarmCb(void)
{
}


void hc32L110RtcInit(void)
{
	stc_rtc_config_t stcRtcConfig; 
    stc_rtc_irq_cb_t stcIrqCb;
    stc_rtc_time_t  stcTime;
    stc_rtc_alarmset_t stcAlarm;
    stc_rtc_cyc_sel_t   stcCycSel;

	 DDL_ZERO_STRUCT(stcRtcConfig);
	 DDL_ZERO_STRUCT(stcIrqCb);
	 DDL_ZERO_STRUCT(stcTime);
	 DDL_ZERO_STRUCT(stcAlarm);
	 DDL_ZERO_STRUCT(stcCycSel);    
    
    Clk_SetPeripheralGate(ClkPeripheralRtc,TRUE);
    
    stcRtcConfig.enClkSel = RtcClk32768;
    stcRtcConfig.enAmpmSel = Rtc24h;
      
    stcTime.u8Year = 0x17;
    stcTime.u8Month = 0x06;
    stcTime.u8Day = 0x07;
    stcTime.u8Hour = 0x12;
    stcTime.u8Minute = 0x59;
    stcTime.u8Second = 0x59;
    stcTime.u8DayOfWeek = Rtc_CalWeek(&stcTime.u8Day);
    stcRtcConfig.pstcTimeDate = &stcTime;
    
    stcIrqCb.pfnAlarmIrqCb = RtcAlarmCb;
    stcIrqCb.pfnTimerIrqCb = RtcCycCb;
    stcRtcConfig.pstcIrqCb = &stcIrqCb;
    stcRtcConfig.bTouchNvic = FALSE;
    
    Rtc_DisableFunc(RtcCount);
    Rtc_Init(&stcRtcConfig);

    Rtc_EnableFunc(RtcCount);
}


uint32_t hc32L110RtcGetCounter(void)
{
	stc_rtc_time_t  stcReadTime;
	Rtc_ReadDateTime(&stcReadTime);	

	return hc32L110RtcGetCalendarValue(stcReadTime);
}

uint32_t hc32L110RtcElapse(uint32_t startTime)
{
	stc_rtc_time_t  stcReadTime;
	Rtc_ReadDateTime(&stcReadTime);	
	return (hc32L110RtcGetCalendarValue(stcReadTime) - startTime);
}


static uint32_t hc32L110RtcGetCalendarValue( stc_rtc_time_t rtcValue )
{
    uint32_t calendarValue = 0; 
	calendarValue = rtcValue.u8Month*30*86400+rtcValue.u8Day*86400+rtcValue.u8Hour*3600+rtcValue.u8Minute*60+rtcValue.u8Second;

    return( calendarValue );
}


