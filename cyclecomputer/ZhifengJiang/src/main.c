#include "gpio.h"
#include "systick.h"
#include "stm32f10x_conf.h"
#include <stdio.h>

#define DISPLAY_WINDOW 3
#define MODE_COUNT 4
#define CIRCLE_DISTANCE 2

enum modes{TIME = 0, DISTANCE, CURRENT_SPEED, AVERAGE_SPEED};

unsigned int current_time;
unsigned int last_time;
unsigned int mode;

unsigned int circle;
unsigned int distance, last_distance;
double current_speed;
double average_speed;

MySystick *systick;
IOPin *sensor;
IOPin *button;

void RCC_Config();
void TIM_Config();
void EXTI_Config();
void USART_Config();
void NVIC_Config();
void Data_Init();

int main(){

	IOPin a2(PORT_A, 2, INPUT, PULL_UP);
	IOPin a3(PORT_A, 3, INPUT, PULL_UP);
	MySystick st;

	a2.init();
	a3.init();
	st.enable();
	sensor = &a2;
	button = &a3;
	systick = &st;

	RCC_Config();
	TIM_Config();
	EXTI_Config();
	USART_Config();
	NVIC_Config();
	Data_Init();

	while (1);
}

// Configuration of Real-Time Clock Control
void RCC_Config() {
	ErrorStatus HSEStartUpStatus;

	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	
	if(HSEStartUpStatus == SUCCESS) {
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK2Config(RCC_HCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div2);
		
		FLASH_SetLatency(FLASH_Latency_2);
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_4);
		RCC_PLLCmd(ENABLE);

		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
		while(RCC_GetSYSCLKSource() != 0x08);
	}
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
}

// Configuration of Timer
void TIM_Config(){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	TIM_DeInit(TIM2);
	TIM_InternalClockConfig(TIM2);

	TIM_TimeBaseStructure.TIM_Prescaler = 16000 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 2000 - 1;

	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ARRPreloadConfig(TIM2, DISABLE);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
}

// Configuration of External Interrupt
void EXTI_Config(){
	
	sensor->exti_config();
	EXTI->IMR |= 1 << 2;
	EXTI->FTSR |= 1 << 2;
	
	button->exti_config();
	EXTI->IMR |= 1 << 3;
	EXTI->FTSR |= 1 << 3;
}

// Configuration of Universal Synchronous and Asynchronous Receiver-Transmitter
void USART_Config() {
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1,USART_FLAG_TC);
}

// Configuration of Nested Vectored Interrupt Controller
void NVIC_Config() {
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

// data initializer
void Data_Init() {
	circle 			= 0;
	distance 		= 0;
	last_distance 	= 0;
	current_speed 	= 0.0;
	average_speed 	= 0.0;
	mode 			= 0;
	current_time 	= 0;
	last_time		= 0;
}

// timer inturrupt handler
extern "C"
void TIM2_IRQHandler(){
	unsigned int i;
	char buf[512];
	buf[0] = 0;

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
		current_time++;

		if (current_time % DISPLAY_WINDOW == 0) {
			switch (mode) {
			case TIME:
				sprintf(buf, "Time(sec): %d\n", current_time);
				break;
			case DISTANCE:
				sprintf(buf, "Distance(m): %d\n", distance);
				break;
			case CURRENT_SPEED:
				sprintf(buf, "Current speed(km/h): %d.%d\n", (int)current_speed, (int)((current_speed - (int)current_speed) * 100));
				break;
			case AVERAGE_SPEED:
			default:
				sprintf(buf, "Average speed(km/h): %d.%d\n", (int)average_speed, (int)((average_speed - (int)average_speed) * 100));
				break;
			}

			i = 0;
			while (buf[i]) {
				USART_SendData(USART1, buf[i++]);
				while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
			}
		}
	}
}

// sensor interrupt handler
extern "C"
void EXTI2_IRQHandler() {
	systick->delay(10);

	if (sensor->read() == LOW) {
		circle++;
		distance 			= circle * CIRCLE_DISTANCE;

		if (current_time > last_time && current_time > 0) {
			current_speed	= (double)(distance - last_distance) / (current_time - last_time);
			average_speed 	= (double)distance / current_time;
		}

		last_time 			= current_time;
		last_distance 		= distance;
	}

	EXTI->PR = 1 << 2;
}

// button interrupt handler
extern "C"
void EXTI3_IRQHandler() {
	systick->delay(10);

	if (button->read() == LOW)
		mode = (mode + 1) % MODE_COUNT;

	EXTI->PR = 1 << 3;
}