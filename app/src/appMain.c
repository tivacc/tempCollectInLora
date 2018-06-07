/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>

#include "initDevice.h"

#include "lora.h"
#include "radio.h"
#include "lpm.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
#define ENABLE_FAST_WAKEUP
#define LORAWAN_ADR_ON                  1
#define JOINREQ_NBTRIALS                3

/* Private macro -------------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
//static void 	LoraTxData(lora_AppData_t *AppData, FunctionalState *IsTxConfirmed);
//static void 	LoraRxData(lora_AppData_t *AppData);

/* Private variables ---------------------------------------------------------*/

/**
 * Initialises the Lora Parameters
 */
LoRaParam_t LoRaParamInit = {TX_ON_TIMER/*TX_ON_EVENT*/,
                                    5000,
                                    CLASS_A,
                                    LORAWAN_ADR_ON,
                                    DR_0,
                                    LORAWAN_PUBLIC_NETWORK,
                                    JOINREQ_NBTRIALS};

timer_t testTimer1;
timer_t testTimer2;
void testTimer1CallBack(void);
void testTimer2CallBack(void);

uint32_t  testCnt = 0; 
/* Implementation of the functions */
void main(void)
{
  //Lpm Cfg
  stc_lpm_config_t stcLpmCfg;  
  stcLpmCfg.enSEVONPEND   = SevPndDisable;
  stcLpmCfg.enSLEEPDEEP   = SlpDpEnable;
  stcLpmCfg.enSLEEPONEXIT = SlpExtDisable;
  Lpm_Config(&stcLpmCfg);

  initDevice();
  Radio.IoInit();

  lora_Init(NULL, &LoRaParamInit);
  
  //timerInit(&testTimer1,testTimer1CallBack);
  //timerInit(&testTimer2,testTimer2CallBack);
  //timerStart(&testTimer1,500);
  //timerStart(&testTimer2,1700);
  
  while(1) {     
    lora_fsm();
    //testCnt = hc32L110RtcGetCounter();
    //Lpm_GotoLpmMode();
  }
}

void testTimer1CallBack(void)
{
  testCnt++;
  timerStart(&testTimer1,500);
  
  hc32L110LedToggle();
  hc32L110UartSend(11,"test timer1");
}

void testTimer2CallBack(void)
{
  timerStart(&testTimer2,1700);
  hc32L110UartSend(11,"test timer2");
}

