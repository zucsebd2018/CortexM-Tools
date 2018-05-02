#include <stm32f10x.h>
#include "main.h"
//#define DEBUG
/*
 * A0: Senser
 * A4: Mode select
 * A9,A10: Serial ports
 */

/*
 * Support Four mode:
 * SPEED: current speed
 * MAX  : maximum speed
 * AVG  : average speed
 * TOTAL: total mileage
 */
#ifdef DEBUG
#define debug(string) println(string)
#else
#define debug(string) while(0){}
#endif
enum mode_t {
    SPEED = 0, MAX = 1, AVG = 2, TOTAL = 3
};
int mode = 0;
double total = 0;
double radius = 0.33;
uint64_t time = 0;
uint64_t last = 0;
uint32_t maxspeed = 0;


void System_Sleep(void)
{
        SCB->SCR |= 0X00;
#if defined ( __CC_ARM   )
        __force_stores();
#endif
        /* Request Wait For Interrupt */
        __WFI();
}

int main()
{
        RCC_Configuration();
        GPIO_Configuration();
        USART1_configuration();// now println is available
        TIM_Configuration();
        EXIT_Configuration();
        NVIC_Configuration();
        
        println("hello!");
        int i = 0;
        System_Sleep();
        while (1) {
                System_Sleep();
        }
}

void itoa_kmh(uint64_t num, char *string)
{
        string[0] = (num / 100) % 10 + '0';
        string[1] = (num / 10) % 10 + '0';
        string[2] = '.';
        string[3] = num % 10 + '0';
        string[4] = 'k', string[5] = 'm', string[6] = '/', string[7] = 'h';
        string[8] = 0;
}

void EXTI0_IRQHandler()
{
        debug("EXTI0_IRQHandler");
        uint32_t now = time;
        if (last) {
                if (last > now)last = 0;
                else {
                        uint64_t delta = now - last;
                        double speed = 2 * 3.14159 * radius * 1000 / delta;
                        uint32_t curspeed = speed * 36;
                        maxspeed = curspeed > maxspeed ? curspeed : maxspeed;
                        total += 2 * 3.14159 * radius * 1000;
                        char num_s[9];
                        switch (mode) {
                        case SPEED: {
                                itoa_kmh(curspeed, num_s);
                                print("Current speed:");
                                println(num_s);
                                break;
                        }
                        case MAX: {
                                itoa_kmh(maxspeed, num_s);
                                print("Maximum speed:");
                                println(num_s);
                                break;
                        }
                        case AVG: {
                                itoa_kmh(total / time * 36, num_s);
                                print("Average speed:");
                                println(num_s);
                                break;
                        }
                        case TOTAL: {
                                itoa_kmh(total / 100000, num_s);
                                num_s[6] = 0;
                                print("Total:");
                                println(num_s);
                                break;
                        }
                        default:break;
                        }
                        last = now;
                }
        } else {
                last = now;
        }
        delay_debounce();
        EXTI_ClearITPendingBit(EXTI_Line0);
}

void EXTI4_IRQHandler()
{
        mode = (mode + 1) % 4;
        delay_debounce();
        EXTI_ClearITPendingBit(EXTI_Line4);
}

void TIM2_IRQHandler()
{
        time++;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

void println(const char *string)
{
        uint16_t da = 0xa;
        uint16_t dd = 0xd;
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        while (*string) {
                USART_SendData(USART1, (uint8_t) (*string));
                string++;
                while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        }
        USART_SendData(USART1, da);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        USART_SendData(USART1, dd);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void print(const char *string)
{
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        while (*string) {
                USART_SendData(USART1, (uint8_t) (*string));
                string++;
                while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        }
}

void USART1_configuration(void)
{
        USART_InitTypeDef USART_InitStructure;
        USART_InitStructure.USART_BaudRate = 9600;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
        
        /* Configure USART1 */
        USART_Init(USART1, &USART_InitStructure);
        
        /* Enable the USART1 */
        USART_Cmd(USART1, ENABLE);
}

void RCC_Configuration(void)
{
        /* Enable USART1, GPIOA, GPIOx and AFIO clocks */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void NVIC_Configuration(void)
{
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        NVIC_InitTypeDef NVIC_InitStruct;
        NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
        NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
        NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
        NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
        debug("NVIC_Init_s");
        NVIC_Init(&NVIC_InitStruct);
        debug("NVIC_Init_e");
        
        NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn; //使能外部中断所在的通道
        NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02; //抢占优先级 2，
        NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x02; //子优先级 2
        NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
        NVIC_Init(&NVIC_InitStruct); //根据结构体信息进行优先级初始化
        
        NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn; //使能外部中断所在的通道
        NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02; //抢占优先级 2，
        NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x02; //子优先级 2
        NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
        NVIC_Init(&NVIC_InitStruct); //根据结构体信息进行优先级初始化
        
}

void TIM_Configuration(void)
{
        
        TIM_DeInit(TIM2);
        TIM_TimeBaseInitTypeDef TIM_TimeBaseInitstruct;
        TIM_TimeBaseInitstruct.TIM_Prescaler = 7200 - 1;//0.1 us
        TIM_TimeBaseInitstruct.TIM_Period = 10 - 1;//1 ms
        TIM_TimeBaseInitstruct.TIM_ClockDivision = TIM_CKD_DIV1;
        TIM_TimeBaseInitstruct.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseInitstruct.TIM_RepetitionCounter = 0;
        
        TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitstruct);
        
        TIM_ARRPreloadConfig(TIM2, ENABLE);
        
        TIM_ClearFlag(TIM2, TIM_FLAG_Update);
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
        
        TIM_Cmd(TIM2, ENABLE);
        
}

void GPIO_Configuration(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;          //USART1 TX
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //复用推挽输出
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);      //A端口
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;          //USART1 RX
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //复用开漏输入
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void EXIT_Configuration(void)
{
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
        
        EXTI_InitTypeDef EXTI_InitStructure;//定义初始化结构体
        EXTI_InitStructure.EXTI_Line = EXTI_Line0; //中断线的标号 取值范围为EXTI_Line0~EXTI_Line15
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式，可选值为中断 EXTI_Mode_Interrupt 和事件 EXTI_Mode_Event。
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//触发方式，可以是下降沿触发 EXTI_Trigger_Falling，上升沿触发 EXTI_Trigger_Rising，或者任意电平（上升沿和下降沿）触发EXTI_Trigger_Rising_Falling
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        
        EXTI_Init(&EXTI_InitStructure);
        
        EXTI_InitStructure.EXTI_Line = EXTI_Line4; //中断线的标号 取值范围为EXTI_Line0~EXTI_Line15
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式，可选值为中断 EXTI_Mode_Interrupt 和事件 EXTI_Mode_Event。
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//触发方式，可以是下降沿触发 EXTI_Trigger_Falling，上升沿触发 EXTI_Trigger_Rising，或者任意电平（上升沿和下降沿）触发EXTI_Trigger_Rising_Falling
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        
        EXTI_Init(&EXTI_InitStructure);//根据结构体信息进行初始化
}

void delay_debounce()
{
        uint32_t t = 0x000FFFFF;
        while (t--);
}