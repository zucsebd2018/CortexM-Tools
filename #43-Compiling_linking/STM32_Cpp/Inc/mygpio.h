
#ifndef __MYGPIO_H__
#define __MYGPIO_H__

#include "stm32f1xx_hal.h"

class mygpio
{
private:
    GPIO_TypeDef *myGPIO;

public:
    mygpio(GPIO_TypeDef *myGPIO);
    void init(uint16_t Pin, uint32_t Mode, uint32_t Speed);
    void set(uint16_t Pin, GPIO_PinState state);
    GPIO_PinState get(uint16_t Pin);

};


#endif