#include <stdio.h>
#include <stdint.h>

#include "timeService.h"

//
uint8_t timerCbCnt = 0;
uint8_t timerCbEventTotal = 0;
timeOutCallBack timerCallBackFunc[10];

timer_t *timerListHeader;

#pragma optimize= none
static void timerProcess(void)
{
	timer_t *readyTimer = NULL;
	timer_t *currentTimer = timerListHeader;

	//calculate the will start timer
	readyTimer = timerListHeader;
	while(currentTimer)
	{
		if((currentTimer->timeOut)<(readyTimer->timeOut))
		{
			readyTimer = currentTimer;
		}
		
		currentTimer = currentTimer->pNext;
	}

	if(readyTimer)
	{
		(*readyTimer).timerState = 1;
		hc32L110LptStart(readyTimer->timeOut);
	}
}

/*
#pragma optimize= none
void timerCallBackenQueue(timeOutCallBack pCallBack)
{
	timerCbEventTotal += 1;
	timerCallBackFunc[timerCbCnt] = pCallBack;
	timerCbCnt = (timerCbCnt+1)%10;
}

#pragma optimize= none
timeOutCallBack timerCallBackdeQueue(void)
{
	uint8_t timerEventStart = 0;

	if(timerCbEventTotal)
	{
		if(timerCbEventTotal<timerCbCnt)
		{
			timerEventStart = timerCbCnt - timerCbEventTotal;
		}
		else if(timerCbEventTotal == timerCbCnt)
		{
			timerEventStart =0;
		}
		else
		{
			timerEventStart =10 + timerCbCnt - timerCbEventTotal;
		}
		
		timerCbEventTotal -= 1;
		return timerCallBackFunc[timerEventStart];
	}
	else
	{
		return NULL;
	}
}*/

#pragma optimize= none
void timerListInit(void)
{
	timerListHeader = NULL;
}

#pragma optimize= none
void timerInit(timer_t *toStartTimer,timeOutCallBack callBack)
{
	toStartTimer->timerState = 0;
	toStartTimer->fcallBack = callBack;
	toStartTimer->pNext = NULL;
}

#pragma optimize= none
bool timerStart(timer_t *toStartTimer,uint16_t timeOutValue)
{
	uint8_t timerHasExit = 0;
	uint16_t curTimerCount = 0;
	uint16_t timerElapesed = 0;

	timer_t *currentTimer = NULL;
	timer_t *nextTimer = NULL;
  
	toStartTimer->timeOut = timeOutValue;
	toStartTimer->timerState  = 0;
  	toStartTimer->pNext = NULL;

	//if timer has been run ,calculate the time has been elapsed
	currentTimer = timerListHeader;
	while(currentTimer)
	{		
		if(currentTimer->timerState == 1)
		{
			curTimerCount = hc32L110LptGetCounter();
			
			(*currentTimer).timerState = 0;
			timerElapesed = (*currentTimer).timeOut - hc32L110LptCnt2Ms(65535 - curTimerCount);
		}

		currentTimer = currentTimer->pNext;
	}
	
	//update the new time of the timer
	currentTimer = timerListHeader;
	while(currentTimer)
	{
		(*currentTimer).timeOut = (*currentTimer).timeOut - timerElapesed;

		currentTimer = currentTimer->pNext;
	}
	
	//currentTimer = toStartTimer;
	if(timerListHeader == NULL)
	{	
		timerListHeader = toStartTimer;
	}
	else
	{
		nextTimer = timerListHeader;
		//only one timer in the timer list 
		if(nextTimer->pNext == NULL)
		{
			if(nextTimer->fcallBack == toStartTimer->fcallBack)
			{
				timerHasExit = 1;

				//pNext not change
				nextTimer->fcallBack = toStartTimer->fcallBack;
				nextTimer->timeOut = toStartTimer->timeOut;
				nextTimer->timerState = toStartTimer->timerState;
			}
		}
		else
		{		
			while(nextTimer->pNext)
			{	
				if(nextTimer->fcallBack == toStartTimer->fcallBack)
				{
					timerHasExit = 1;
					
					//if the timer is exit,recover the same timer,pNext not change
					nextTimer->fcallBack = toStartTimer->fcallBack;
					nextTimer->timeOut = toStartTimer->timeOut;
					nextTimer->timerState = toStartTimer->timerState;
					break;
				}
				else
				{
					nextTimer = nextTimer->pNext;
				}		
			}
		}

		if(timerHasExit == 0)  //this timer is a new timer
		{
			(*nextTimer).pNext = toStartTimer;
		}
	}

	delay100us(10);//because the read processing of timer count will cost some time,so the start of the timer would be delayed 
	timerProcess();
	return true;
}

#pragma optimize= none
void timerStop(timer_t *toStopTimer)
{
	timer_t *previousTimer = NULL;
	timer_t *currentTimer = timerListHeader;
	while(currentTimer)
	{
		if(currentTimer->fcallBack == toStopTimer->fcallBack)		//find the target
		{		
			if(previousTimer)						//the target is not the first timer
			{
				if(currentTimer->pNext)				//the target timer is in the middle of list
				{
					(*previousTimer).pNext = (*currentTimer).pNext;
				}
				else												//the target timer is at the end of list
				{
					(*previousTimer).pNext = NULL;
				}
			}
			else													//the target is the first timer
			{
				if(currentTimer->pNext)								//the started timer more than one
				{
					timerListHeader = currentTimer->pNext;			//set new timer list header
				}
				else												//the started timer only one
				{
					timerListHeader = NULL;							//timer list empty
				}
			}

			break;
		}
		else
		{
			previousTimer = currentTimer;
			currentTimer = currentTimer->pNext;
		}
	}

	timerProcess();
}


void timerIrqHandler(void)
{
	uint8_t timerState = 0;
	uint16_t timeOutValue = 0;
	timeOutCallBack fcallBack;
	
  timer_t *tempTimer = NULL;
	timer_t *timeOutTimer = timerListHeader;

	while(timeOutTimer)
	{
		timerState = (*timeOutTimer).timerState;
		if(timerState)
		{
			timeOutValue = (*timeOutTimer).timeOut;
			fcallBack = (*timeOutTimer).fcallBack;
       
      //calculate the new time will be time
      tempTimer = timerListHeader;
      while(tempTimer)
      {
        if((*tempTimer).fcallBack != fcallBack)
        {
          (*tempTimer).timeOut = (*tempTimer).timeOut - timeOutValue;
        }
        tempTimer = tempTimer->pNext;
      }
      
			timerStop(timeOutTimer);

			fcallBack();
			//timerCallBackenQueue(fcallBack);
			break;
		}
		else
		{
			timeOutTimer = timeOutTimer->pNext;
		}
	}
}






