#include <stdio.h>
#include <stdint.h>

#include "hc32L110UserUart.h"


//macro define
#define UART_RX_MAX 	64

#define  HC32L110_UART_RX_PORT	0
#define  HC32L110_UART_RX_PIN	  1
#define  HC32L110_UART_TX_PORT 	0
#define  HC32L110_UART_TX_PIN 	2



//global varialbe
uint8_t u8RxCnt=0;
uint32_t uartRxBuffer[UART_RX_MAX];


void txIntCallback(void)
{

}

void rxIntCallback(void)
{
	uartRxBuffer[u8RxCnt]=Uart_ReceiveData(UARTCH0);
	u8RxCnt++;
}

void errIntCallback(void)
{
  
}


//data & cmd uart init
void hc32L110UartInit(void)
{
	uint16_t timer;
	stc_uart_config_t stcUartConfig;
	stc_uart_irq_cb_t stcUartIrqCb;
	stc_uart_baud_config_t stcUartBaud;
  stc_bt_config_t stcBtConfig;

  DDL_ZERO_STRUCT(stcUartConfig);
  DDL_ZERO_STRUCT(stcUartIrqCb);
  DDL_ZERO_STRUCT(stcUartBaud);
  DDL_ZERO_STRUCT(stcBtConfig);
	
	Gpio_InitIOExt(HC32L110_UART_TX_PORT,HC32L110_UART_TX_PIN,GpioDirOut,TRUE,FALSE,FALSE,FALSE);   
  Gpio_InitIOExt(HC32L110_UART_RX_PORT,HC32L110_UART_RX_PIN,GpioDirOut,TRUE,FALSE,FALSE,FALSE); 

  Gpio_SetIO(HC32L110_UART_TX_PORT,HC32L110_UART_TX_PIN,TRUE);
  Gpio_SetIO(HC32L110_UART_RX_PORT,HC32L110_UART_RX_PIN,TRUE);

  Gpio_SetFunc_UART0RX_P01();
  Gpio_SetFunc_UART0TX_P02();

	Clk_SetPeripheralGate(ClkPeripheralBt,TRUE);
	Clk_SetPeripheralGate(ClkPeripheralUart0,TRUE);

	stcUartIrqCb.pfnRxIrqCb = rxIntCallback;
  stcUartIrqCb.pfnTxIrqCb = txIntCallback;
  stcUartIrqCb.pfnRxErrIrqCb = errIntCallback;
  
  stcUartConfig.pstcIrqCb = &stcUartIrqCb;
  stcUartConfig.bTouchNvic = TRUE;
  stcUartConfig.enRunMode = UartMode1;

  stcUartBaud.bDbaud = 0u;
  stcUartBaud.u32Baud = 9600u;
  stcUartBaud.u8Mode = UartMode1;
  timer=Uart_SetBaudRate(UARTCH0,Clk_GetPClkFreq(),&stcUartBaud);
  
  stcBtConfig.enMD = BtMode2;
  stcBtConfig.enCT = BtTimer;
  Bt_Init(TIM0, &stcBtConfig);
  Bt_ARRSet(TIM0,timer);
  Bt_Cnt16Set(TIM0,timer);
  Bt_Run(TIM0);

  Uart_Init(UARTCH0, &stcUartConfig);
  Uart_EnableIrq(UARTCH0,UartRxIrq);
  Uart_ClrStatus(UARTCH0,UartRxFull);
  Uart_EnableFunc(UARTCH0,UartRx);
}


//
void hc32L110UartSend(uint8_t dataLen, uint8_t *dataBuffer)
{
	for(uint8_t i=0;i<dataLen;i++)
	{
		Uart_SendData(UARTCH0, dataBuffer[i]);	
	}
}


