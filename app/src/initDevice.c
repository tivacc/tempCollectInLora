#include <stdio.h>
#include <stdint.h>
#include "HC32L110.h"
#include "clk.h"
#include "gpio.h"
#include "uart.h"
#include "lpt.h"
#include "lpm.h"

#include "initDevice.h"

#define  HC32L110_LED_PORT		0		
#define  HC32L110_LED_PIN_RED	3

//local function
static void hc32L110ClkInit(void);
static void hc32L110GpioInit(void);


/* Implementation of the functions */
static void hc32L110ClkInit(void)
{
	stc_clk_config_t stcClkCfg;

	stcClkCfg.enClkSrc = ClkRCH;
    stcClkCfg.enHClkDiv = ClkDiv1;
    stcClkCfg.enPClkDiv = ClkDiv1;
    Clk_Init(&stcClkCfg); 
}


//board GPIO
static void hc32L110GpioInit(void)
{
	Clk_SetPeripheralGate(ClkPeripheralGpio, TRUE);
	
	Gpio_InitIO(HC32L110_LED_PORT, HC32L110_LED_PIN_RED, GpioDirOut);
	Gpio_SetIO(HC32L110_LED_PORT,HC32L110_LED_PIN_RED,TRUE);
}


void hc32L110LedOn(void)
{
	Gpio_SetIO(HC32L110_LED_PORT,HC32L110_LED_PIN_RED,FALSE);
}

void hc32L110LedOff(void)
{
	Gpio_SetIO(HC32L110_LED_PORT,HC32L110_LED_PIN_RED,TRUE);
}

void hc32L110LedToggle(void)
{
	Gpio_SetIO(HC32L110_LED_PORT,HC32L110_LED_PIN_RED,!Gpio_GetIO(HC32L110_LED_PORT,HC32L110_LED_PIN_RED));
}



void initDevice(void)
{
	//system clock
	hc32L110ClkInit();

	//led io
	hc32L110GpioInit();

	//spi init
	hc32L110SpiIoInit();

	//lpt init
	hc32L110LptInit();

	//uart init
	hc32L110UartInit();

	//rtc init
	hc32L110RtcInit();

	//interrupu init
	hc32L110GpioExtInit();
}

