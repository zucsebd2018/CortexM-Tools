#include "uart.h"
#include <string.h>
#include <misc.h>

//#define BUF_LEN 10

#define NUM_UART_PORT 4

#ifndef UART_TX_BUF_LEN1
#define UART_TX_BUF_LEN1 80
#endif

#ifndef UART_RX_BUF_LEN1
#define UART_RX_BUF_LEN1 80
#endif

#ifndef UART_RX_LOOP_BUF_LEN1
#define UART_RX_LOOP_BUF_LEN1 512
#endif

#ifndef UART_TX_BUF_LEN2
#define UART_TX_BUF_LEN2 128
#endif

#ifndef UART_RX_BUF_LEN2
#define UART_RX_BUF_LEN2 512
#endif

#ifndef UART_RX_LOOP_BUF_LEN2
#define UART_RX_LOOP_BUF_LEN2 1024
#endif

typedef struct _uartPortParam {
	//	configure
	int 			pinTx;
	int 			pinRx;
	USART_TypeDef*	pUSART;
	DMA_Channel_TypeDef *pTxDMA;
	DMA_Channel_TypeDef *pRxDMA;
	uint32_t 		TxDMAFlag;
	uint32_t 		RxDMAFlag;
	uint8_t 		TxDMAIRQn;
	uint8_t 		RxDMAIRQn;
	uint8_t 		USARTIRQn;
	//	buffer & flag
	char* 			txBuffer;
	char* 			rxBuffer;
	int 			txBufLen;
	int 			rxBufLen;
	int 			txPtr;
	int 			rxPtr;
	volatile int 	isTxDone;
	volatile int 	isRxDone;
	//	rx loop buffer
	char*			rxLoopBuf;
	int 			rxLoopBufLen;
	int 			rxLoopPutPtr;
	int 			rxLoopGetPtr;
} UARTPortParam;

static char __txBuffer1[UART_TX_BUF_LEN1];
static char __rxBuffer1[UART_RX_BUF_LEN1];
static char __rxLoopBuf1[UART_RX_LOOP_BUF_LEN1];
static char __txBuffer2[UART_TX_BUF_LEN2];
static char __rxBuffer2[UART_RX_BUF_LEN2];
static char __rxLoopBuf2[UART_RX_LOOP_BUF_LEN2];

static UARTPortParam __uartPortParam[NUM_UART_PORT] = {
	{0},
	{GPIO_Pin_9, GPIO_Pin_10, USART1, 
		DMA1_Channel4, DMA1_Channel5, DMA1_FLAG_TC4, DMA1_FLAG_TC5, 
		DMA1_Channel4_IRQn, DMA1_Channel5_IRQn, USART1_IRQn,
		__txBuffer1, __rxBuffer1, UART_TX_BUF_LEN1, UART_RX_BUF_LEN1, 
		0, 0, 1, 0,
		__rxLoopBuf1, UART_RX_LOOP_BUF_LEN1,0,0},
	{GPIO_Pin_2, GPIO_Pin_3,  USART2, 
		DMA1_Channel7, DMA1_Channel6, DMA1_FLAG_TC7, DMA1_FLAG_TC6, 
		DMA1_Channel7_IRQn, DMA1_Channel6_IRQn, USART2_IRQn,
		__txBuffer2, __rxBuffer2, UART_TX_BUF_LEN2, UART_RX_BUF_LEN2, 
		0, 0, 1, 0,
		__rxLoopBuf2, UART_RX_LOOP_BUF_LEN2,0,0},
};

static void __uart_output(int port);
static void __uart_rx_transfer(int port);

int uartInit(int port, int baud)
{
	int ret = 1;

	if ( port == 1 ) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1  | RCC_APB2Periph_AFIO, ENABLE);
	} else if ( port == 2 ) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	} else if ( port == 3 ) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	}

	//	init pins used by USART
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = __uartPortParam[port].pinTx;//GPIO_Pin_9;//serialConfig[port->port].pin_tx;//GPIO_Pin_9;				//¹Ü½Å9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//¸´ÓÃÍÆÍìÊä³ö
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//TX³õÊ¼»¯

	GPIO_InitStructure.GPIO_Pin = __uartPortParam[port].pinRx;//GPIO_Pin_10;				//¹Ü½Å10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	//¸¡¿ÕÊäÈë
	GPIO_Init(GPIOA, &GPIO_InitStructure);     				//RX³õÊ¼»¯

	//	init USART
	USART_InitTypeDef USART_InitStructure; 					//¶¨Òå´®¿Ú³õÊ¼»¯½á¹¹Ìå
	USART_InitStructure.USART_BaudRate = baud;										//²¨ÌØÂÊ9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//8Î»Êý¾Ý
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//1¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No ; 							//ÎÞÐ£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //½ûÓÃRTSCTSÓ²¼þÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//Ê¹ÄÜ·¢ËÍ½ÓÊÕ

	USART_ClockInitTypeDef USART_ClockInitStructure;
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	USART_ClockInit(USART1, &USART_ClockInitStructure);		//×îºóÊý¾ÝÎ»µÄÊ±ÖÓÂö³å²»Êä³öµ½SCLKÒý½Å

	USART_Init(__uartPortParam[port].pUSART, &USART_InitStructure);	//³õÊ¼»¯´®¿Ú1
	/* Enable USART1 DMA Rx and TX request */
//	USART_DMACmd(USART1, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);
	USART_DMACmd(__uartPortParam[port].pUSART, USART_DMAReq_Tx, ENABLE);
	//	enable uart DMA receive
	USART_DMACmd(__uartPortParam[port].pUSART, USART_DMAReq_Rx, ENABLE);

	USART_Cmd(__uartPortParam[port].pUSART, ENABLE);

	//	TX DMA init
	DMA_Cmd(__uartPortParam[port].pTxDMA, DISABLE);
	DMA_DeInit(__uartPortParam[port].pTxDMA);

	DMA_InitTypeDef DMA_InitStructure;
	/* USARTy TX DMA1 Channel (triggered by USARTy Tx event) Config */
	// DMA_DeInit(DMA1_Channel4);
	// DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);//0x40013804;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(__uartPortParam[port].pUSART->DR);//0x40013804;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)__uartPortParam[port].txBuffer;//0;//(uint32_t)line;//TxBuffer1;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 0;	//	to be specified at beginning of the tranport
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(__uartPortParam[port].pTxDMA, &DMA_InitStructure);
	DMA_ClearFlag(__uartPortParam[port].TxDMAFlag);
	DMA_ITConfig(__uartPortParam[port].pTxDMA, DMA_IT_TC, ENABLE);

	//=NVIC_Configuration==============================================================================//
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	//	2 preemption, 8 subpriority
	//	0 for timers, 1 for usarts

	/* Enable the DMA Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = __uartPortParam[port].TxDMAIRQn; //DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;	// lowest
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// /* Configure the NVIC Preemption Priority Bits */  
 	// NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
 	/* Enable the USART1 Interrupt */
 	NVIC_InitStructure.NVIC_IRQChannel = __uartPortParam[port].USARTIRQn; //USART1_IRQn;
 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//	highest
 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 	NVIC_Init(&NVIC_InitStructure);

 	//	for RX DMA and Int
 	// USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
 	USART_ITConfig(__uartPortParam[port].pUSART, USART_IT_IDLE, ENABLE);

	/*--- UART_Rx_DMA_Channel DMA Config ---*/
	DMA_Cmd(__uartPortParam[port].pRxDMA, DISABLE);										// disable DMA first
	DMA_DeInit(__uartPortParam[port].pRxDMA);												// restore to default vales
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&__uartPortParam[port].pUSART->DR);		// uart data register
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)__uartPortParam[port].rxBuffer;			// uart rx buffer
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						// peripheral as source
	DMA_InitStructure.DMA_BufferSize = 0;									// length to receive
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		// no increament on peripheral address
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					// increament on buffer
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	// periphereal: 8-bit
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			// buffer: 8-bit
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							// single byte mode
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;					// priority
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;							// no memory-memory mode
	DMA_Init(__uartPortParam[port].pRxDMA, &DMA_InitStructure);				// set the configure
	//DMA_ClearFlag(DMA1_FLAG_GL5);											// clear DMA flags
	DMA_ClearFlag(__uartPortParam[port].RxDMAFlag);	

	//=NVIC_Configuration==============================================================================//
	// NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure the NVIC Preemption Priority Bits */
	// NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* Enable the DMA Rx Complete Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = __uartPortParam[port].RxDMAIRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	return ret;
}

void uartPrint(int port, const char* line)
{
	while ( *line ) {
		while ( !__uartPortParam[port].isTxDone ) {
		}
		int hasCRLF = 0;
		while ( *line && __uartPortParam[port].txPtr < __uartPortParam[port].txBufLen ) {
			__uartPortParam[port].txBuffer[__uartPortParam[port].txPtr] = *line;
			if ( *line == '\n' || *line == '\r' ) {
				hasCRLF = 1;
			}
			__uartPortParam[port].txPtr++;
			line++;
		}
		if ( hasCRLF || __uartPortParam[port].txPtr == __uartPortParam[port].txBufLen ) {
			// __uartPortParam[port].txBuffer[__uartPortParam[port].txPtr-1] = '$';
			__uart_output(port);
		}
	}
}

void uartPrintln(int port, const char* line)
{
	uartPrint(port, line);
	uartPrint(port, "\r\n");
}

void uartPrintInt(int port, int i)
{
	char buf[12] = {0};	//	-2^31-->-2147483648-->11-byte
	char neg=0;
	int loc = sizeof(buf)-2;
	//	start from right to left
	if ( i<0 ) {
		neg = 1;
		i = -i;
	}
	do {
		buf[loc--] = '0' + i%10;
		i = i/10;
	} while ( i>0);
	if ( neg ) {
		buf[loc] = '-';
	} else {
		loc ++;
	}
	uartPrint(port, buf+loc);
}

void uartFlush(int port)
{
	if ( __uartPortParam[port].isTxDone && __uartPortParam[port].txPtr ) {
		__uart_output(port);
	}
}

void uartRxBegin(int port)
{
	DMA_Cmd(__uartPortParam[port].pRxDMA, DISABLE);
	DMA_ClearFlag(__uartPortParam[port].RxDMAFlag);
	// DMA1_Channel5->CNDTR = __uartPortParam[port].rxBufLen;
	DMA_SetCurrDataCounter(__uartPortParam[port].pRxDMA, __uartPortParam[port].rxBufLen);
	__uartPortParam[port].isRxDone = 0;
	// enable DMA channel5 for uart1 rx
	DMA_Cmd(__uartPortParam[port].pRxDMA, ENABLE);
}

int uartRxIsDone(int port)
{
	if ( DMA_GetFlagStatus(__uartPortParam[port].RxDMAFlag) != RESET ) {
		return 1;
	} else {
		return __uartPortParam[port].isRxDone;
	}
}

int uartRxLength(int port)
{
	return __uartPortParam[port].rxBufLen - DMA_GetCurrDataCounter(__uartPortParam[port].pRxDMA);
}

char* uartRxRead(int port)
{
	return __uartPortParam[port].rxBuffer;
}

int uartPeek(int port)
{
	int ret = EOF;
	if ( __uartPortParam[port].rxLoopGetPtr != __uartPortParam[port].rxLoopPutPtr ) {
		ret = __uartPortParam[port].rxLoopBuf[__uartPortParam[port].rxLoopGetPtr];
	} 
	return ret;
}

int uartGetchar(int port)
{
	int ret = EOF;
	if ( __uartPortParam[port].rxLoopGetPtr != __uartPortParam[port].rxLoopPutPtr ) {
		ret = __uartPortParam[port].rxLoopBuf[__uartPortParam[port].rxLoopGetPtr];
		__uartPortParam[port].rxLoopGetPtr=(__uartPortParam[port].rxLoopGetPtr+1)%__uartPortParam[port].rxLoopBufLen;

	} 
	return ret;
}

int uartHasLine(int port)
{
	int ret = 0;
	char* buf = __uartPortParam[port].rxLoopBuf;
	int ptr = __uartPortParam[port].rxLoopGetPtr;
	int putptr = __uartPortParam[port].rxLoopPutPtr;
	int buflen = __uartPortParam[port].rxLoopBufLen;
	while ( ptr != putptr ) {
		if ( buf[ptr] == 0x0D || buf[ptr] == 0x0A ) {
			ret = 1;
			break;
		} 
		ptr++;
		ptr %= buflen;
	} 
	return ret;
}

char* uartReadLine(int port, char* line, int len)
{
	char* buf = __uartPortParam[port].rxLoopBuf;
	int getptr = __uartPortParam[port].rxLoopGetPtr;
	int putptr = __uartPortParam[port].rxLoopPutPtr;
	int buflen = __uartPortParam[port].rxLoopBufLen;
	int i=0;
	int hasCRLF=0;
	len--;
	for ( i=0; i<len && getptr!=putptr; i++ ) {
		line[i] = buf[getptr++];
		getptr %= buflen;
		if ( line[i] == 0x0D || line[i] == 0x0A ) {
			if ( hasCRLF ) {
				i--;
				break;
			} else {
				hasCRLF = 1;
			}
		} else if ( hasCRLF ) {
			getptr--;
			if ( getptr<0 ) {
				getptr = buflen-1;
			}
			break;
		}
	}
	line[i] = 0;
	__uartPortParam[port].rxLoopGetPtr = getptr;
	return line;
}

// -- internal functions -----------------

static void __uart_output(int port)
{
	DMA_Cmd(__uartPortParam[port].pTxDMA, DISABLE);	
	// DMA1_Channel4->CNDTR = __uartPortParam[port].txPtr;
	DMA_SetCurrDataCounter(__uartPortParam[port].pTxDMA, __uartPortParam[port].txPtr);
	__uartPortParam[port].isTxDone = 0;
	DMA_Cmd(__uartPortParam[port].pTxDMA, ENABLE);
}

static void __uart_rx_transfer(int port)
{
	int len = uartRxLength(port);
	int getptr = __uartPortParam[port].rxLoopGetPtr;
	int putptr = __uartPortParam[port].rxLoopPutPtr;
	char* buf = __uartPortParam[port].rxLoopBuf;
	char* raw = __uartPortParam[port].rxBuffer;
	int buflen = __uartPortParam[port].rxLoopBufLen;
	for ( int i=0; i<len; i++ ) {
		buf[putptr++] = raw[i];
		putptr %= buflen;
		if ( putptr == getptr ) {
			getptr++;
			getptr %= buflen;
		}
	}
	__uartPortParam[port].rxLoopGetPtr = getptr;
	__uartPortParam[port].rxLoopPutPtr = putptr;
	uartRxBegin(port);
}

// DMA TX Finish int
void DMA1_Channel4_IRQHandler(void)
{
	if ( DMA_GetFlagStatus(DMA1_FLAG_TC4) == SET ) {
		DMA_ClearFlag(DMA1_FLAG_TC4);
		DMA_Cmd(DMA1_Channel4, DISABLE);
		__uartPortParam[1].isTxDone = 1;
		__uartPortParam[1].txPtr = 0;
	}
}

void DMA1_Channel7_IRQHandler(void)
{
	if ( DMA_GetFlagStatus(DMA1_FLAG_TC7) == SET ) {
		DMA_ClearFlag(DMA1_FLAG_TC7);
		DMA_Cmd(DMA1_Channel7, DISABLE);
		__uartPortParam[2].isTxDone = 1;
		__uartPortParam[2].txPtr = 0;
	}
}

//	DMA RX 1 Finish int
void DMA1_Channel5_IRQHandler(void)
{
	if ( DMA_GetFlagStatus(DMA1_FLAG_TC5) == SET ) {
		DMA_ClearFlag(DMA1_FLAG_TC5);
		DMA_Cmd(DMA1_Channel5, DISABLE);
		__uartPortParam[1].isRxDone = 1;
		__uart_rx_transfer(1);
	}
}

//	DMA RX 2 Finish int
void DMA1_Channel6_IRQHandler(void)
{
	if ( DMA_GetFlagStatus(DMA1_FLAG_TC6) == SET ) {
		DMA_ClearFlag(DMA1_FLAG_TC6);
		DMA_Cmd(DMA1_Channel6, DISABLE);
		__uartPortParam[2].isRxDone = 1;
		__uart_rx_transfer(2);
	}
}

// USART RX int
void USART1_IRQHandler(void)
{
	// static int cnt =0;
	
 // 	if ( USART_GetITStatus(USART1, USART_IT_RXNE) != RESET ) {
 //    	/* Read one byte from the receive data register */
 // 		__rxBuffer[__rxPtr] = USART_ReceiveData(USART1);
 // 		if ( __rxBuffer[__rxPtr] == '.' ) {
 //    		__rxBuffer[__rxPtr+1] = '\0';
 //    		__rxPtr = 0;
 //    		rx_flag = 1;
 //    	} else 
 //    		__rxPtr++;
 //    	//USART_ITConfig(USARTy, USART_IT_RXNE, DISABLE);    		
	// }
	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) {
		int t = USART1->SR;
		t = USART1->DR;
		DMA_Cmd(DMA1_Channel5, DISABLE);
		__uartPortParam[1].isRxDone = 2;
		__uart_rx_transfer(1);
	}
}

void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) {
		int t = USART2->SR;
		t = USART2->DR;
		DMA_Cmd(DMA1_Channel6, DISABLE);
		__uartPortParam[2].isRxDone = 2;
		__uart_rx_transfer(2);
	}
}
