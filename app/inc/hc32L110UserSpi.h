#ifndef _USER_SPI_H_
#define _USER_SPI_H_


#include "ddl.h"
#include "gpio.h"


void hc32L110SpiIoInit(void);


void hc32L110SpiRead(uint8_t addr, uint8_t *data);


//spi send a byte
void hc32L110SpiWrite(uint8_t addr,uint8_t data);


//
void hc32L110SpiReadBuffer(uint8_t addr,uint8_t *buffer,uint8_t readSize);


void hc32L110SpiWriteBuffer(uint8_t addr,uint8_t *buffer,uint8_t writeSize);


void hc32L110SpiReadFifo( uint8_t *buffer, uint8_t readSize );


void hc32L110SpiWriteFifo( uint8_t *buffer, uint8_t writeSize );


#endif

