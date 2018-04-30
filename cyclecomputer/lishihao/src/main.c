#include <stdio.h>

#include "gpio.h"
#include "systick.h"
#include "stm32f10x_conf.h"

#define METER_PER_R 2
#define NUM_MODE 2
#define DISPLAY_PERIOD 3

IOPin *a2,*a3;
MySystick *st;
uint32_t rotation,mode,speed,avg_speed;
uint32_t cur_time,pre_time,pre_rotation;
uint32_t display;
char buffer[100];
void var_init();
void rcc_init();
void gpio_init();
void nvic_init();
void exti_init();
void uart_init();
void tim_init();

int main(){
	IOPin ta2(PORT_A,2,INPUT,PULL_UP);
	ta2.init();
	IOPin ta3(PORT_A,3,INPUT,PULL_UP);
	ta3.init();
	a2=&ta2;
	a3=&ta3;
	MySystick tst;
	tst.enable();
	st=&tst;

	var_init();
	rcc_init();
	nvic_init();
	exti_init();
	uart_init();
	tim_init();
	while(1);
}

void var_init(){
	rotation=mode=speed=avg_speed=0;
	cur_time=pre_time=pre_rotation=0;
	display = DISPLAY_PERIOD;
	buffer[0] = 0;
}

void rcc_init(){
	ErrorStatus HSEStartUpStatus;
	//将RCC寄存器重新设置为默认值
	RCC_DeInit();
	//打开外部高速时钟晶振
	RCC_HSEConfig(RCC_HSE_ON);
	//等待外部高速时钟晶振工作
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus == SUCCESS)
	{
	//设置AHB时钟(HCLK)为系统时钟
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	//设置高速AHB时钟(APB2)为HCLK时钟
	RCC_PCLK2Config(RCC_HCLK_Div1);
	//设置低速AHB时钟(APB1)为HCLK的2分频
	RCC_PCLK1Config(RCC_HCLK_Div2);
	//设置FLASH代码延时
	FLASH_SetLatency(FLASH_Latency_2);
	//使能预取指缓存
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	//设置PLL时钟，为HSE的4倍频 8MHz * 4 = 32MHz
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_4);
	//使能PLL
	RCC_PLLCmd(ENABLE);
	//等待PLL准备就绪
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	//设置PLL为系统时钟源
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	//判断PLL是否是系统时钟
	while(RCC_GetSYSCLKSource() != 0x08);
	}
	//开启串口 GPIO AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	//开始TIM2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
}


void nvic_init(){
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	//TIM2 NVIC
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//UART NVIC
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//EXIT2
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//EXIT3
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void exti_init(){
	//挂上2号中断线 使能，下降沿触发
	a2->exti_config();
	EXTI->IMR|=1<<2;
	EXTI->FTSR|=1<<2;
	//挂上3号中断线
	a3->exti_config();
	EXTI->IMR|=1<<3;
	EXTI->FTSR|=1<<3;
}

void uart_init(){
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//校验
	USART_InitStructure.USART_HardwareFlowControl
	=USART_HardwareFlowControl_None; //硬件流控制无
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //发送与接受两种方式
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE); //USART1使能
	//清除第一个标志位
	USART_ClearFlag(USART1,USART_FLAG_TC);
}

void tim_init(){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	//重新将Timer设置为缺省值
	TIM_DeInit(TIM2);
	//采用内部时钟给TIM2提供时钟源
	TIM_InternalClockConfig(TIM2);
	//预分频系数为36000-1，这样计数器时钟为32MHz/16000 = 2kHz
	TIM_TimeBaseStructure.TIM_Prescaler = 16000 - 1;
	//设置时钟分割
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//设置计数器模式为向上计数模式
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//设置计数溢出大小，每计2000个数就产生一个更新事件
	TIM_TimeBaseStructure.TIM_Period = 2000 - 1;
	//将配置应用到TIM2中
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	//清除溢出中断标志
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	//禁止ARR预装载缓冲器
	TIM_ARRPreloadConfig(TIM2, DISABLE);
	//开启TIM2的中断
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	//启用TIM2
	TIM_Cmd(TIM2,ENABLE);
}

extern "C"{
//定时器中断，时间++ 和 显示
void TIM2_IRQHandler(void) {
	uint32_t i;
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
		cur_time++;
		display--;
		if(display==0){
			sprintf(buffer,"distance %dm\tspeed %dm/s\t",rotation*METER_PER_R,speed);
			for( i=0;buffer[i];i++){
              USART_SendData(USART1,buffer[i]);
              //等待数据发送完毕
              while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
       		}
			if(mode==0){
				sprintf(buffer,"using time %ds\n",cur_time);
			}
			else{
				sprintf(buffer,"avg_speed %dm/s\n",avg_speed);
			}
			for( i=0;buffer[i];i++){
              USART_SendData(USART1,buffer[i]);
              //等待数据发送完毕
              while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
       		}
       		display = DISPLAY_PERIOD;
		}
	}
}

//传感器的中断
void EXTI2_IRQHandler(void){
	st->delay(10);//去抖
	if(a2->read()==LOW){
		rotation++;
		//精度为1s
		if(cur_time>pre_time){
			speed = (rotation - pre_rotation) * METER_PER_R / (cur_time-pre_time);
			pre_time = cur_time;
			pre_rotation = rotation;
		}
		if(cur_time>0){
			avg_speed = rotation*METER_PER_R/cur_time;
		}
	}
	//清除中断标志
	EXTI->PR=1<<2;
}
//按键中断
void EXTI3_IRQHandler(void){
	st->delay(10);//去抖
	if(a3->read()==LOW){
		mode++;
		mode = mode % NUM_MODE;
	}
	//清除中断标志
	EXTI->PR=1<<3;
}
}