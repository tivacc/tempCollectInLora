/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMESERVER_H__
#define __TIMESERVER_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "hc32L110UserLpt.h"


 typedef void (*timeOutCallBack)(void);

 typedef struct timer_tag {
 	uint8_t timerState;
 	uint32_t timeOut;
 	timeOutCallBack fcallBack;
 	struct timer_tag *pNext;
 }timer_t,*ptimer_t;

 typedef struct timer_list_tag {
 	timer_t *timer;
 	struct timer_list_tag *pNext;
 }timer_list_t;


 //extern void timerCallBackenQueue(timeOutCallBack pCallBack);
 //extern timeOutCallBack timerCallBackdeQueue(void);
 extern void timerListInit(void);
 extern void timerInit(timer_t *newTimer,timeOutCallBack callBack);
 extern bool timerStart(timer_t *toStartTimer,uint16_t timeOutValue);
 extern void timerStop(timer_t *toStopTimer);
 extern void timerIrqHandler(void);


#ifdef __cplusplus
}
#endif

#endif /* __TIMESERVER_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
