# 尝试C++的编译连接

>尝试在Cube库的基础上做一个GPIO类，分析所产生的二进制文件是否急剧膨胀

## 编写相关类

利用Hal的相关库函数我们可以写一个简单地GPIO类。我这里实现了初始化，get和set功能。

```cpp
//mygpio.h
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
```

```cpp
//mygpio.c
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
```

然后main.c中的相关部分即可：

```cpp
//init
mygpio test(GPIOA);
test.init(GPIO_PIN_2,GPIO_MODE_OUTPUT_PP,GPIO_SPEED_FREQ_HIGH);
```

```cpp
//blink
HAL_Delay(1000);
if(test.get(GPIO_PIN_2) == GPIO_PIN_RESET){
    test.set(GPIO_PIN_2, GPIO_PIN_SET);
}else if(test.get(GPIO_PIN_2) == GPIO_PIN_SET){
    test.set(GPIO_PIN_2, GPIO_PIN_RESET);
}
```

要注意的是保存位.c即可，不用保存为cpp

## 修改Makefile文件

首先就是在Source中加入我们的文件：

```Makefile
C_SOURCES =  \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c \
Src/main.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c \
Src/stm32f1xx_hal_msp.c \
/Src/system_stm32f1xx.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c \
Src/stm32f1xx_it.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c \
Src/mygpio.c
```

