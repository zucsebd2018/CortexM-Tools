//
// Created by 范宏昌 on 2018/4/21.
//

#ifndef CMCUP_MAIN_H
#define CMCUP_MAIN_H
void RCC_Configuration(void);  //配置时钟
void GPIO_Configuration(void); //配置GPIO
void NVIC_Configuration(void); //配置中断
void TIM_Configuration(void);
void USART1_configuration(void);
void EXIT_Configuration(void);
void println(const char* string);
void print(const char* string);
void delay(unsigned int ms);
void delay_debounce();
void delay_block();
extern "C"{
void EXTI0_IRQHandler(void);
void EXTI4_IRQHandler(void);
void TIM2_IRQHandler(void);
}

#endif //CMCUP_MAIN_H
