#include <stdio.h>
#include <stdint.h>

#include "hc32L110UserSpi.h"


#define  HC32L110_SPI_PORT		2			
#define  HC32L110_SPI_PIN_CS	3		
#define  HC32L110_SPI_PIN_CLK	6
#define  HC32L110_SPI_PIN_MOSI	4	
#define  HC32L110_SPI_PIN_MISO 	5



//
static void hc32L110SpiEnable(void);

static void hc32L110SpiDisable(void);

static uint8_t spiReadData(void);

static void spiWriteData(uint8_t data);



/* Implementation of the functions */

void hc32L110SpiIoInit(void)
{
	Gpio_InitIO(HC32L110_SPI_PORT, HC32L110_SPI_PIN_CS, GpioDirOut);
	Gpio_InitIO(HC32L110_SPI_PORT, HC32L110_SPI_PIN_CLK, GpioDirOut);
	Gpio_InitIO(HC32L110_SPI_PORT, HC32L110_SPI_PIN_MOSI, GpioDirOut);
	Gpio_InitIO(HC32L110_SPI_PORT, HC32L110_SPI_PIN_MISO, GpioDirIn);

	//cs & clk output high level
	Gpio_SetIO(HC32L110_SPI_PORT, HC32L110_SPI_PIN_CS, TRUE);
	Gpio_SetIO(HC32L110_SPI_PORT, HC32L110_SPI_PIN_CLK, FALSE);
}


//enanble spi
static void hc32L110SpiEnable(void)
{
	Gpio_SetIO(HC32L110_SPI_PORT, HC32L110_SPI_PIN_CS, FALSE);
}


//disable spi
static void hc32L110SpiDisable(void)
{
	Gpio_SetIO(HC32L110_SPI_PORT, HC32L110_SPI_PIN_CS, TRUE);
}

static uint8_t spiReadData(void)
{
	uint8_t spiTempData = 0;
	for(uint8_t i=0;i<8;i++)
	{		
		if(Gpio_GetIO(HC32L110_SPI_PORT,HC32L110_SPI_PIN_MISO))
		{
			spiTempData |= 0x80>>i;
		}
		Gpio_SetIO(HC32L110_SPI_PORT, HC32L110_SPI_PIN_CLK, TRUE);
		__asm("NOP");__asm("NOP");__asm("NOP");__asm("NOP");__asm("NOP");
		Gpio_SetIO(HC32L110_SPI_PORT, HC32L110_SPI_PIN_CLK, FALSE);
		__asm("NOP");__asm("NOP");__asm("NOP");__asm("NOP");__asm("NOP");
	}
	
	return spiTempData;
}

static void spiWriteData(uint8_t data)
{	
	for(uint8_t i=0;i<8;i++)
	{		
		if(data&(0x80>>i))
		{
			Gpio_SetIO(HC32L110_SPI_PORT, HC32L110_SPI_PIN_MOSI, TRUE);
		}
		else
		{
			Gpio_SetIO(HC32L110_SPI_PORT, HC32L110_SPI_PIN_MOSI, FALSE);
		}
		Gpio_SetIO(HC32L110_SPI_PORT, HC32L110_SPI_PIN_CLK, TRUE);
		__asm("NOP");__asm("NOP");__asm("NOP");__asm("NOP");__asm("NOP");
		Gpio_SetIO(HC32L110_SPI_PORT, HC32L110_SPI_PIN_CLK, FALSE);
		__asm("NOP");__asm("NOP");__asm("NOP");__asm("NOP");__asm("NOP");
	}
}


//spi rcv a byte
void hc32L110SpiRead(uint8_t addr, uint8_t *data)
{
	hc32L110SpiEnable();
	spiWriteData(addr & 0x7F);
	data[0] = spiReadData();
	hc32L110SpiDisable();
}


//spi send a byte
void hc32L110SpiWrite(uint8_t addr,uint8_t data)
{
	hc32L110SpiEnable();
	spiWriteData(addr | 0x80);
	spiWriteData(data);
	hc32L110SpiDisable();
}


//
void hc32L110SpiReadBuffer(uint8_t addr,uint8_t *buffer,uint8_t readSize)
{
	hc32L110SpiEnable();
	spiWriteData(addr & 0x7F);
	for(uint8_t i=0;i<readSize;i++)
	{
		buffer[i] = spiReadData();
	}
	hc32L110SpiDisable();
}


void hc32L110SpiWriteBuffer(uint8_t addr,uint8_t *buffer,uint8_t writeSize)
{
	hc32L110SpiEnable();
	spiWriteData(addr | 0x80);
	for(uint8_t i=0;i<writeSize;i++)
	{
		spiWriteData(buffer[i]);
	}
	hc32L110SpiDisable();
}

void hc32L110SpiReadFifo( uint8_t *buffer, uint8_t readSize )
{
	hc32L110SpiReadBuffer( 0, buffer, readSize );
}


void hc32L110SpiWriteFifo( uint8_t *buffer, uint8_t writeSize )
{
	hc32L110SpiWriteBuffer( 0, buffer, writeSize );
}
