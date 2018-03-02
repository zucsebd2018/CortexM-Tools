#include "usr_conf.h"
#include "stdio.h"

void delay_ms(u16 time){
	u16 i= 0;
	while(time--){
		i = 12000;
		while(i--){}
	}
}

void USART1_IRQHandler(){
	u8 RX_data;
	
	if(USART_GetFlagStatus(USART1,USART_FLAG_PE)!=RESET){
		USART_ReceiveData(USART1);
		USART_ClearFlag(USART1,USART_FLAG_PE);
	}
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)!=RESET){
		USART_ReceiveData(USART1);
		USART_ClearFlag(USART1,USART_FLAG_ORE);
	}
	if(USART_GetFlagStatus(USART1,USART_FLAG_FE)!=RESET){
		USART_ReceiveData(USART1);
		USART_ClearFlag(USART1,USART_FLAG_FE);
	}
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET){
		USART_ClearFlag(USART1,USART_FLAG_RXNE);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		RX_data = USART_ReceiveData(USART1);
		//while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET){}

		//debug info
		//GPIO_WriteBit(GPIOA,GPIO_Pin_3,(BitAction)(Bit_SET-GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_3)));
		
		USART_ITConfig(USART1,USART_IT_TXE,ENABLE);
		USART_SendData(USART1,RX_data);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET){}
		USART_ITConfig(USART1,USART_IT_TXE,DISABLE);
	}
	
}
void sendChar(char c){
	USART_SendData(USART1,c);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
}
void sendString(char *s){
	while((*s) != '\0'){
		if(*s == '\n'){
			sendChar(13);
			sendChar(10);
		}else{
			sendChar(*s);
		}
		s++;
	}
}

int main()
{
	//Config
	RCC_Configuration();
	GPIO_Configuration();
	USART_Configuration();
	NVIC_Configuration();

	//Start Info
	sendString("Hello World\n");

	//Blink the LED
	int flag = 0;
	while(1){
		if(!flag){
			GPIO_WriteBit(GPIOA,GPIO_Pin_2,Bit_SET);
			delay_ms(500);
			flag = 1;
		}else{
			GPIO_WriteBit(GPIOA,GPIO_Pin_2,Bit_RESET);
			delay_ms(500);
			flag = 0;
		}
	}
	return 0;
}
