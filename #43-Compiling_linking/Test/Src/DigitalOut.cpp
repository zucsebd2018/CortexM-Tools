#include "DigitalOut.h"

DigitalOut::DigitalOut(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin):GPIOx(GPIOx), GPIO_Pin(GPIO_Pin)
{
    //Init
    GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

DigitalOut::DigitalOut(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t GPIO_Mode, uint32_t GPIO_Speed)
{
    //Init
    GPIO_InitTypeDef GPIO_InitStruct;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_Mode;
    GPIO_InitStruct.Speed = GPIO_Speed;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

DigitalOut::~DigitalOut()
{

}

void DigitalOut::write(int value)
{
    if (value == 0)
    {
        HAL_GPIO_WritePin(this->GPIOx, this->GPIO_Pin, GPIO_PIN_RESET);
        this->state = 0;
    }
    else if (value == 1)
    {
        HAL_GPIO_WritePin(this->GPIOx, this->GPIO_Pin, GPIO_PIN_SET);
        this->state = 1;
    }
}

int DigitalOut::read()
{
    return this->state;
}

DigitalOut::operator int ()
{
    return this->read();
}

DigitalOut& DigitalOut::operator= (int value)
{
    if (value == 0)
    {
        HAL_GPIO_WritePin(this->GPIOx, this->GPIO_Pin, GPIO_PIN_RESET);
        this->state = 0;
    }
    else if (value == 1)
    {
        HAL_GPIO_WritePin(this->GPIOx, this->GPIO_Pin, GPIO_PIN_SET);
        this->state = 1;
    }
    return *this;
}



