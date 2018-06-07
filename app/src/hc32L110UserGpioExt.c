#include <stdio.h>
#include <stdint.h>

#include "hc32L110UserGpioExt.h"

typedef struct {
    uint8_t u8Port;
    uint8_t u8Pin;
}stc_gpio_list_t;


hc32L110GpioExtCbkPtr_t gpioCallbacks[16] = { 0 };

//
void hc32L110GpioExtDispatcher(uint32_t iflags);

const stc_gpio_list_t gGpiolist[] =
{
    /*{ 3, 4 }, { 3, 5 }, { 3, 3 }, { 3, 2 }*/
    { 3, 5 }, { 3, 4 },
};

//#pragma optimize = none
void Gpio_IRQHandler(uint8_t u8Param)
{
    uint8_t irqPort = u8Param;
    *((uint32_t *)((uint32_t)&M0P_GPIO->P0ICLR + u8Param * 0x40)) = 0;
    if(Gpio_GetIO(irqPort,5))
    {
    	hc32L110GpioExtDispatcher(1<<5);
    }
    else if(Gpio_GetIO(irqPort,4))
    {
    	hc32L110GpioExtDispatcher(1<<4);
    }
    /*
    else if(Gpio_GetIO(u8Param,3))
    {
    	hc32L110GpioExtDispatcher(1<<3);
    }
    else if(Gpio_GetIO(u8Param,2))
    {
    	hc32L110GpioExtDispatcher(1<<2);
    }
    else
    {
    	;
    }*/
}

//
void hc32L110GpioExtInit(void)
{
	for (uint8_t i = 0; i < ARRAY_SZ(gGpiolist); i++)
    {
        Gpio_InitIOExt(gGpiolist[i].u8Port,
                       gGpiolist[i].u8Pin,
                       GpioDirIn, TRUE, FALSE, FALSE, 0);

        Gpio_EnableIrq(gGpiolist[i].u8Port,
                       gGpiolist[i].u8Pin, GpioIrqRising);
    }
    
    EnableNvic(PORT3_IRQn, DDL_IRQ_LEVEL_DEFAULT, TRUE);
}


//
void hc32L110GpioExtCbRegister(uint8_t pin, hc32L110GpioExtCbkPtr_t callbackPtr)
{
	gpioCallbacks[pin] = callbackPtr;
}


#pragma optimize = none
void hc32L110GpioExtDispatcher(uint32_t iflags)
{
	 uint32_t irqIdx;
	 for(irqIdx = 0;irqIdx<32;irqIdx++)
	 {
		 if(iflags&1<<irqIdx)
		 {
			 gpioCallbacks[irqIdx]();
		 }
	 }
}



