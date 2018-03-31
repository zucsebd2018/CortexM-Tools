#include "mygpio.h"
#include "stm32f1xx_hal.h"

mygpio::mygpio(GPIO_TypeDef *myGPIO){
    this->myGPIO = myGPIO;
}

void mygpio::init(uint16_t Pin, uint32_t Mode, uint32_t Speed){
    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_GPIOA_CLK_ENABLE();

    HAL_GPIO_WritePin(this->myGPIO, Pin, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = Pin;
    GPIO_InitStruct.Mode = Mode;
    GPIO_InitStruct.Speed = Speed;
    HAL_GPIO_Init(this->myGPIO, &GPIO_InitStruct);

}

void mygpio::set(uint16_t Pin, GPIO_PinState state){
    HAL_GPIO_WritePin(this->myGPIO, Pin, state);
}

GPIO_PinState mygpio::get(uint16_t Pin){
    GPIO_PinState state = HAL_GPIO_ReadPin(this->myGPIO,Pin);
    return state;
}