#include "DigitalIn.h"

DigitalIn::DigitalIn(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t GPIO_Pull):GPIOx(GPIOx), GPIO_Pin(GPIO_Pin)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Pull = GPIO_Pull;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

DigitalIn::~DigitalIn()
{

}

int DigitalIn::read()
{
    GPIO_PinState state = HAL_GPIO_ReadPin(this->GPIOx, this->GPIO_Pin);
    if (state == GPIO_PIN_SET)
        return 1;
    else
        return 0;
}

DigitalIn::operator int ()
{
    return this->read();
}
