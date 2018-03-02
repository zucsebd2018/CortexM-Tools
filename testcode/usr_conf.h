#ifndef __USR_CONF__
#define __USR_CONF__

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "system_stm32f10x.h"
#include "misc.h"

void RCC_Configuration();
void GPIO_Configuration();
void USART_Configuration();
void NVIC_Configuration();

#endif
