#include "ddl.h"
#include "gpio.h"

#include "base_types.h"

typedef void (*hc32L110GpioExtCbkPtr_t)(void);


//
void hc32L110GpioExtInit(void);

//
void hc32L110GpioExtCbRegister(uint8_t pin, hc32L110GpioExtCbkPtr_t callbackPtr);

