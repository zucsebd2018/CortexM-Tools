#include "uart.h"
#include <string.h>
#include <misc.h>

//#define BUF_LEN 10

#define NUM_UART_PORT 4

//define from std peripheral
#define CR1_UE_Set                ((uint16_t)0x2000)  /*!< USART Enable Mask */
#define CR1_UE_Reset              ((uint16_t)0xDFFF)  /*!< USART Disable Mask */

#define IS_RCC_APB2_PERIPH(PERIPH) ((((PERIPH) & 0xFFC00002) == 0x00) && ((PERIPH) != 0x00))
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))
#define IS_GPIO_ALL_PERIPH(PERIPH) (((PERIPH) == GPIOA) || \
                                    ((PERIPH) == GPIOB) || \
                                    ((PERIPH) == GPIOC) || \
                                    ((PERIPH) == GPIOD) || \
                                    ((PERIPH) == GPIOE) || \
                                    ((PERIPH) == GPIOF) || \
                                    ((PERIPH) == GPIOG))
#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_Mode_AIN) || ((MODE) == GPIO_Mode_IN_FLOATING) || \
                            ((MODE) == GPIO_Mode_IPD) || ((MODE) == GPIO_Mode_IPU) || \
                            ((MODE) == GPIO_Mode_Out_OD) || ((MODE) == GPIO_Mode_Out_PP) || \
                            ((MODE) == GPIO_Mode_AF_OD) || ((MODE) == GPIO_Mode_AF_PP))
#define IS_GPIO_PIN(PIN) ((((PIN) & (uint16_t)0x00) == 0x00) && ((PIN) != (uint16_t)0x00))
#define IS_USART_ALL_PERIPH(PERIPH) (((PERIPH) == USART1) || \
                                     ((PERIPH) == USART2) || \
                                     ((PERIPH) == USART3) || \
                                     ((PERIPH) == UART4) || \
                                     ((PERIPH) == UART5))

#define IS_USART_123_PERIPH(PERIPH) (((PERIPH) == USART1) || \
                                     ((PERIPH) == USART2) || \
                                     ((PERIPH) == USART3))

#define IS_USART_1234_PERIPH(PERIPH) (((PERIPH) == USART1) || \
                                      ((PERIPH) == USART2) || \
                                      ((PERIPH) == USART3) || \
                                      ((PERIPH) == UART4))
#define USART_WordLength_8b                  ((uint16_t)0x0000)
#define USART_WordLength_9b                  ((uint16_t)0x1000)
                                    
#define IS_USART_WORD_LENGTH(LENGTH) (((LENGTH) == USART_WordLength_8b) || \
                                      ((LENGTH) == USART_WordLength_9b))
#define USART_StopBits_1                     ((uint16_t)0x0000)
#define USART_StopBits_0_5                   ((uint16_t)0x1000)
#define USART_StopBits_2                     ((uint16_t)0x2000)
#define USART_StopBits_1_5                   ((uint16_t)0x3000)
#define IS_USART_STOPBITS(STOPBITS) (((STOPBITS) == USART_StopBits_1) || \
                                     ((STOPBITS) == USART_StopBits_0_5) || \
                                     ((STOPBITS) == USART_StopBits_2) || \
                                     ((STOPBITS) == USART_StopBits_1_5))
#define USART_Parity_No                      ((uint16_t)0x0000)
#define USART_Parity_Even                    ((uint16_t)0x0400)
#define USART_Parity_Odd                     ((uint16_t)0x0600) 
#define IS_USART_PARITY(PARITY) (((PARITY) == USART_Parity_No) || \
                                 ((PARITY) == USART_Parity_Even) || \
                                 ((PARITY) == USART_Parity_Odd))
#define USART_Mode_Rx                        ((uint16_t)0x0004)
#define USART_Mode_Tx                        ((uint16_t)0x0008)
#define IS_USART_MODE(MODE) ((((MODE) & (uint16_t)0xFFF3) == 0x00) && ((MODE) != (uint16_t)0x00))
#define USART_HardwareFlowControl_None       ((uint16_t)0x0000)
#define USART_HardwareFlowControl_RTS        ((uint16_t)0x0100)
#define USART_HardwareFlowControl_CTS        ((uint16_t)0x0200)
#define USART_HardwareFlowControl_RTS_CTS    ((uint16_t)0x0300)
#define IS_USART_HARDWARE_FLOW_CONTROL(CONTROL)\
                              (((CONTROL) == USART_HardwareFlowControl_None) || \
                               ((CONTROL) == USART_HardwareFlowControl_RTS) || \
                               ((CONTROL) == USART_HardwareFlowControl_CTS) || \
                               ((CONTROL) == USART_HardwareFlowControl_RTS_CTS))
#define USART_Clock_Disable                  ((uint16_t)0x0000)
#define USART_Clock_Enable                   ((uint16_t)0x0800)
#define IS_USART_CLOCK(CLOCK) (((CLOCK) == USART_Clock_Disable) || \
                               ((CLOCK) == USART_Clock_Enable))
#define USART_CPOL_Low                       ((uint16_t)0x0000)
#define USART_CPOL_High                      ((uint16_t)0x0400)
#define IS_USART_CPOL(CPOL) (((CPOL) == USART_CPOL_Low) || ((CPOL) == USART_CPOL_High))
#define USART_CPHA_1Edge                     ((uint16_t)0x0000)
#define USART_CPHA_2Edge                     ((uint16_t)0x0200)
#define IS_USART_CPHA(CPHA) (((CPHA) == USART_CPHA_1Edge) || ((CPHA) == USART_CPHA_2Edge))

#define USART_LastBit_Disable                ((uint16_t)0x0000)
#define USART_LastBit_Enable                 ((uint16_t)0x0100)
#define IS_USART_LASTBIT(LASTBIT) (((LASTBIT) == USART_LastBit_Disable) || \
                                   ((LASTBIT) == USART_LastBit_Enable))
#define USART_DMAReq_Tx                      ((uint16_t)0x0080)
#define USART_DMAReq_Rx                      ((uint16_t)0x0040)
#define IS_USART_DMAREQ(DMAREQ) ((((DMAREQ) & (uint16_t)0xFF3F) == 0x00) && ((DMAREQ) != (uint16_t)0x00))
#define IS_DMA_ALL_PERIPH(PERIPH) (((PERIPH) == DMA1_Channel1) || \
                                   ((PERIPH) == DMA1_Channel2) || \
                                   ((PERIPH) == DMA1_Channel3) || \
                                   ((PERIPH) == DMA1_Channel4) || \
                                   ((PERIPH) == DMA1_Channel5) || \
                                   ((PERIPH) == DMA1_Channel6) || \
                                   ((PERIPH) == DMA1_Channel7) || \
                                   ((PERIPH) == DMA2_Channel1) || \
                                   ((PERIPH) == DMA2_Channel2) || \
                                   ((PERIPH) == DMA2_Channel3) || \
                                   ((PERIPH) == DMA2_Channel4) || \
                                   ((PERIPH) == DMA2_Channel5))

#define DMA_CCR1_EN                         ((uint16_t)0x0001)
#define DMA_DIR_PeripheralDST              ((uint32_t)0x00000010)
#define DMA_DIR_PeripheralSRC              ((uint32_t)0x00000000)
#define IS_DMA_DIR(DIR) (((DIR) == DMA_DIR_PeripheralDST) || \
                         ((DIR) == DMA_DIR_PeripheralSRC))
/**
  * @}
  */

/** @defgroup DMA_peripheral_incremented_mode 
  * @{
  */

#define DMA_PeripheralInc_Enable           ((uint32_t)0x00000040)
#define DMA_PeripheralInc_Disable          ((uint32_t)0x00000000)
#define IS_DMA_PERIPHERAL_INC_STATE(STATE) (((STATE) == DMA_PeripheralInc_Enable) || \
                                            ((STATE) == DMA_PeripheralInc_Disable))
/**
  * @}
  */

/** @defgroup DMA_memory_incremented_mode 
  * @{
  */

#define DMA_MemoryInc_Enable               ((uint32_t)0x00000080)
#define DMA_MemoryInc_Disable              ((uint32_t)0x00000000)
#define IS_DMA_MEMORY_INC_STATE(STATE) (((STATE) == DMA_MemoryInc_Enable) || \
                                        ((STATE) == DMA_MemoryInc_Disable))
/**
  * @}
  */

/** @defgroup DMA_peripheral_data_size 
  * @{
  */

#define DMA_PeripheralDataSize_Byte        ((uint32_t)0x00000000)
#define DMA_PeripheralDataSize_HalfWord    ((uint32_t)0x00000100)
#define DMA_PeripheralDataSize_Word        ((uint32_t)0x00000200)
#define IS_DMA_PERIPHERAL_DATA_SIZE(SIZE) (((SIZE) == DMA_PeripheralDataSize_Byte) || \
                                           ((SIZE) == DMA_PeripheralDataSize_HalfWord) || \
                                           ((SIZE) == DMA_PeripheralDataSize_Word))
/**
  * @}
  */

/** @defgroup DMA_memory_data_size 
  * @{
  */

#define DMA_MemoryDataSize_Byte            ((uint32_t)0x00000000)
#define DMA_MemoryDataSize_HalfWord        ((uint32_t)0x00000400)
#define DMA_MemoryDataSize_Word            ((uint32_t)0x00000800)
#define IS_DMA_MEMORY_DATA_SIZE(SIZE) (((SIZE) == DMA_MemoryDataSize_Byte) || \
                                       ((SIZE) == DMA_MemoryDataSize_HalfWord) || \
                                       ((SIZE) == DMA_MemoryDataSize_Word))

#define DMA_Mode_Circular                  ((uint32_t)0x00000020)
#define DMA_Mode_Normal                    ((uint32_t)0x00000000)
#define IS_DMA_MODE(MODE) (((MODE) == DMA_Mode_Circular) || ((MODE) == DMA_Mode_Normal))


#define DMA_Priority_VeryHigh              ((uint32_t)0x00003000)
#define DMA_Priority_High                  ((uint32_t)0x00002000)
#define DMA_Priority_Medium                ((uint32_t)0x00001000)
#define DMA_Priority_Low                   ((uint32_t)0x00000000)
#define IS_DMA_PRIORITY(PRIORITY) (((PRIORITY) == DMA_Priority_VeryHigh) || \
                                   ((PRIORITY) == DMA_Priority_High) || \
                                   ((PRIORITY) == DMA_Priority_Medium) || \
                                   ((PRIORITY) == DMA_Priority_Low))

#define DMA_M2M_Enable                     ((uint32_t)0x00004000)
#define DMA_M2M_Disable                    ((uint32_t)0x00000000)
#define IS_DMA_M2M_STATE(STATE) (((STATE) == DMA_M2M_Enable) || ((STATE) == DMA_M2M_Disable))
#define IS_DMA_BUFFER_SIZE(SIZE) (((SIZE) >= 0x1) && ((SIZE) < 0x10000))
#define IS_DMA_CLEAR_FLAG(FLAG) (((((FLAG) & 0xF0000000) == 0x00) || (((FLAG) & 0xEFF00000) == 0x00)) && ((FLAG) != 0x00))
#define IS_DMA_CONFIG_IT(IT) ((((IT) & 0xFFFFFFF1) == 0x00) && ((IT) != 0x00))


//mask
#define CCR_CLEAR_Mask           ((uint32_t)0xFFFF800F)
#define CR2_LBDL_Mask             ((uint16_t)0xFFDF)  /*!< USART LIN Break detection Mask */
#define CR2_STOP_CLEAR_Mask       ((uint16_t)0xCFFF)  /*!< USART CR2 STOP Bits Mask */
#define CR2_CLOCK_CLEAR_Mask      ((uint16_t)0xF0FF)  /*!< USART CR2 Clock Mask */
#define DMA1_Channel1_IT_Mask    ((uint32_t)(DMA_ISR_GIF1 | DMA_ISR_TCIF1 | DMA_ISR_HTIF1 | DMA_ISR_TEIF1))
#define DMA1_Channel2_IT_Mask    ((uint32_t)(DMA_ISR_GIF2 | DMA_ISR_TCIF2 | DMA_ISR_HTIF2 | DMA_ISR_TEIF2))
#define DMA1_Channel3_IT_Mask    ((uint32_t)(DMA_ISR_GIF3 | DMA_ISR_TCIF3 | DMA_ISR_HTIF3 | DMA_ISR_TEIF3))
#define DMA1_Channel4_IT_Mask    ((uint32_t)(DMA_ISR_GIF4 | DMA_ISR_TCIF4 | DMA_ISR_HTIF4 | DMA_ISR_TEIF4))
#define DMA1_Channel5_IT_Mask    ((uint32_t)(DMA_ISR_GIF5 | DMA_ISR_TCIF5 | DMA_ISR_HTIF5 | DMA_ISR_TEIF5))
#define DMA1_Channel6_IT_Mask    ((uint32_t)(DMA_ISR_GIF6 | DMA_ISR_TCIF6 | DMA_ISR_HTIF6 | DMA_ISR_TEIF6))
#define DMA1_Channel7_IT_Mask    ((uint32_t)(DMA_ISR_GIF7 | DMA_ISR_TCIF7 | DMA_ISR_HTIF7 | DMA_ISR_TEIF7))
#define DMA2_Channel1_IT_Mask    ((uint32_t)(DMA_ISR_GIF1 | DMA_ISR_TCIF1 | DMA_ISR_HTIF1 | DMA_ISR_TEIF1))
#define DMA2_Channel2_IT_Mask    ((uint32_t)(DMA_ISR_GIF2 | DMA_ISR_TCIF2 | DMA_ISR_HTIF2 | DMA_ISR_TEIF2))
#define DMA2_Channel3_IT_Mask    ((uint32_t)(DMA_ISR_GIF3 | DMA_ISR_TCIF3 | DMA_ISR_HTIF3 | DMA_ISR_TEIF3))
#define DMA2_Channel4_IT_Mask    ((uint32_t)(DMA_ISR_GIF4 | DMA_ISR_TCIF4 | DMA_ISR_HTIF4 | DMA_ISR_TEIF4))
#define DMA2_Channel5_IT_Mask    ((uint32_t)(DMA_ISR_GIF5 | DMA_ISR_TCIF5 | DMA_ISR_HTIF5 | DMA_ISR_TEIF5))
#define FLAG_Mask                 ((uint8_t)0x1F)

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

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

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

typedef struct
{
  uint16_t GPIO_Pin;             /*!< Specifies the GPIO pins to be configured.
                                      This parameter can be any value of @ref GPIO_pins_define */

  GPIOSpeed_TypeDef GPIO_Speed;  /*!< Specifies the speed for the selected pins.
                                      This parameter can be a value of @ref GPIOSpeed_TypeDef */

  GPIOMode_TypeDef GPIO_Mode;    /*!< Specifies the operating mode for the selected pins.
                                      This parameter can be a value of @ref GPIOMode_TypeDef */
}GPIO_InitTypeDef;

typedef enum
{ 
  GPIO_Speed_10MHz = 1,
  GPIO_Speed_2MHz, 
  GPIO_Speed_50MHz
}GPIOSpeed_TypeDef;

typedef enum
{ GPIO_Mode_AIN = 0x0,
  GPIO_Mode_IN_FLOATING = 0x04,
  GPIO_Mode_IPD = 0x28,
  GPIO_Mode_IPU = 0x48,
  GPIO_Mode_Out_OD = 0x14,
  GPIO_Mode_Out_PP = 0x10,
  GPIO_Mode_AF_OD = 0x1C,
  GPIO_Mode_AF_PP = 0x18
}GPIOMode_TypeDef;

typedef struct
{
  uint32_t USART_BaudRate;            /*!< This member configures the USART communication baud rate.
                                           The baud rate is computed using the following formula:
                                            - IntegerDivider = ((PCLKx) / (16 * (USART_InitStruct->USART_BaudRate)))
                                            - FractionalDivider = ((IntegerDivider - ((u32) IntegerDivider)) * 16) + 0.5 */

  uint16_t USART_WordLength;          /*!< Specifies the number of data bits transmitted or received in a frame.
                                           This parameter can be a value of @ref USART_Word_Length */

  uint16_t USART_StopBits;            /*!< Specifies the number of stop bits transmitted.
                                           This parameter can be a value of @ref USART_Stop_Bits */

  uint16_t USART_Parity;              /*!< Specifies the parity mode.
                                           This parameter can be a value of @ref USART_Parity
                                           @note When parity is enabled, the computed parity is inserted
                                                 at the MSB position of the transmitted data (9th bit when
                                                 the word length is set to 9 data bits; 8th bit when the
                                                 word length is set to 8 data bits). */
 
  uint16_t USART_Mode;                /*!< Specifies wether the Receive or Transmit mode is enabled or disabled.
                                           This parameter can be a value of @ref USART_Mode */

  uint16_t USART_HardwareFlowControl; /*!< Specifies wether the hardware flow control mode is enabled
                                           or disabled.
                                           This parameter can be a value of @ref USART_Hardware_Flow_Control */
} USART_InitTypeDef;

typedef struct
{

  uint16_t USART_Clock;   /*!< Specifies whether the USART clock is enabled or disabled.
                               This parameter can be a value of @ref USART_Clock */

  uint16_t USART_CPOL;    /*!< Specifies the steady state value of the serial clock.
                               This parameter can be a value of @ref USART_Clock_Polarity */

  uint16_t USART_CPHA;    /*!< Specifies the clock transition on which the bit capture is made.
                               This parameter can be a value of @ref USART_Clock_Phase */

  uint16_t USART_LastBit; /*!< Specifies whether the clock pulse corresponding to the last transmitted
                               data bit (MSB) has to be output on the SCLK pin in synchronous mode.
                               This parameter can be a value of @ref USART_Last_Bit */
} USART_ClockInitTypeDef;

typedef struct
{
  uint32_t DMA_PeripheralBaseAddr; /*!< Specifies the peripheral base address for DMAy Channelx. */

  uint32_t DMA_MemoryBaseAddr;     /*!< Specifies the memory base address for DMAy Channelx. */

  uint32_t DMA_DIR;                /*!< Specifies if the peripheral is the source or destination.
                                        This parameter can be a value of @ref DMA_data_transfer_direction */

  uint32_t DMA_BufferSize;         /*!< Specifies the buffer size, in data unit, of the specified Channel. 
                                        The data unit is equal to the configuration set in DMA_PeripheralDataSize
                                        or DMA_MemoryDataSize members depending in the transfer direction. */

  uint32_t DMA_PeripheralInc;      /*!< Specifies whether the Peripheral address register is incremented or not.
                                        This parameter can be a value of @ref DMA_peripheral_incremented_mode */

  uint32_t DMA_MemoryInc;          /*!< Specifies whether the memory address register is incremented or not.
                                        This parameter can be a value of @ref DMA_memory_incremented_mode */

  uint32_t DMA_PeripheralDataSize; /*!< Specifies the Peripheral data width.
                                        This parameter can be a value of @ref DMA_peripheral_data_size */

  uint32_t DMA_MemoryDataSize;     /*!< Specifies the Memory data width.
                                        This parameter can be a value of @ref DMA_memory_data_size */

  uint32_t DMA_Mode;               /*!< Specifies the operation mode of the DMAy Channelx.
                                        This parameter can be a value of @ref DMA_circular_normal_mode.
                                        @note: The circular buffer mode cannot be used if the memory-to-memory
                                              data transfer is configured on the selected Channel */

  uint32_t DMA_Priority;           /*!< Specifies the software priority for the DMAy Channelx.
                                        This parameter can be a value of @ref DMA_priority_level */

  uint32_t DMA_M2M;                /*!< Specifies if the DMAy Channelx will be used in memory-to-memory transfer.
                                        This parameter can be a value of @ref DMA_memory_to_memory */
}DMA_InitTypeDef;

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

UART::UART(int port){
  this->port = port;
}

int UART::init(int baud = 9600, int bits=8, int parity = NONE, int stop = 1)
{
	int ret = 1;
  unint16_t wordLength;

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
	return ret;
}

void UART::print(const char* line)
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
// void DMA1_Channel4_IRQHandler(void)
// {
// 	if ( DMA_GetFlagStatus(DMA1_FLAG_TC4) == SET ) {
// 		DMA_ClearFlag(DMA1_FLAG_TC4);
// 		DMA_Cmd(DMA1_Channel4, DISABLE);
// 		__uartPortParam[1].isTxDone = 1;
// 		__uartPortParam[1].txPtr = 0;
// 	}
// }

// void DMA1_Channel7_IRQHandler(void)
// {
// 	if ( DMA_GetFlagStatus(DMA1_FLAG_TC7) == SET ) {
// 		DMA_ClearFlag(DMA1_FLAG_TC7);
// 		DMA_Cmd(DMA1_Channel7, DISABLE);
// 		__uartPortParam[2].isTxDone = 1;
// 		__uartPortParam[2].txPtr = 0;
// 	}
// }

// //	DMA RX 1 Finish int
// void DMA1_Channel5_IRQHandler(void)
// {
// 	if ( DMA_GetFlagStatus(DMA1_FLAG_TC5) == SET ) {
// 		DMA_ClearFlag(DMA1_FLAG_TC5);
// 		DMA_Cmd(DMA1_Channel5, DISABLE);
// 		__uartPortParam[1].isRxDone = 1;
// 		__uart_rx_transfer(1);
// 	}
// }

// //	DMA RX 2 Finish int
// void DMA1_Channel6_IRQHandler(void)
// {
// 	if ( DMA_GetFlagStatus(DMA1_FLAG_TC6) == SET ) {
// 		DMA_ClearFlag(DMA1_FLAG_TC6);
// 		DMA_Cmd(DMA1_Channel6, DISABLE);
// 		__uartPortParam[2].isRxDone = 1;
// 		__uart_rx_transfer(2);
// 	}
// }

// // USART RX int
// void USART1_IRQHandler(void)
// {
// 	// static int cnt =0;
	
//  // 	if ( USART_GetITStatus(USART1, USART_IT_RXNE) != RESET ) {
//  //    	/* Read one byte from the receive data register */
//  // 		__rxBuffer[__rxPtr] = USART_ReceiveData(USART1);
//  // 		if ( __rxBuffer[__rxPtr] == '.' ) {
//  //    		__rxBuffer[__rxPtr+1] = '\0';
//  //    		__rxPtr = 0;
//  //    		rx_flag = 1;
//  //    	} else 
//  //    		__rxPtr++;
//  //    	//USART_ITConfig(USARTy, USART_IT_RXNE, DISABLE);    		
// 	// }
// 	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) {
// 		int t = USART1->SR;
// 		t = USART1->DR;
// 		DMA_Cmd(DMA1_Channel5, DISABLE);
// 		__uartPortParam[1].isRxDone = 2;
// 		__uart_rx_transfer(1);
// 	}
// }

// void USART2_IRQHandler(void)
// {
// 	if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) {
// 		int t = USART2->SR;
// 		t = USART2->DR;
// 		DMA_Cmd(DMA1_Channel6, DISABLE);
// 		__uartPortParam[2].isRxDone = 2;
// 		__uart_rx_transfer(2);
// 	}
// }

// transfer from the std peripheral
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB2ENR |= RCC_APB2Periph;
  }
  else
  {
    RCC->APB2ENR &= ~RCC_APB2Periph;
  }
}

void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
  uint32_t currentmode = 0x00, currentpin = 0x00, pinpos = 0x00, pos = 0x00;
  uint32_t tmpreg = 0x00, pinmask = 0x00;
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
  assert_param(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin));  
  
/*---------------------------- GPIO Mode Configuration -----------------------*/
  currentmode = ((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x0F);
  if ((((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x10)) != 0x00)
  { 
    /* Check the parameters */
    assert_param(IS_GPIO_SPEED(GPIO_InitStruct->GPIO_Speed));
    /* Output mode */
    currentmode |= (uint32_t)GPIO_InitStruct->GPIO_Speed;
  }
/*---------------------------- GPIO CRL Configuration ------------------------*/
  /* Configure the eight low port pins */
  if (((uint32_t)GPIO_InitStruct->GPIO_Pin & ((uint32_t)0x00FF)) != 0x00)
  {
    tmpreg = GPIOx->CRL;
    for (pinpos = 0x00; pinpos < 0x08; pinpos++)
    {
      pos = ((uint32_t)0x01) << pinpos;
      /* Get the port pins position */
      currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;
      if (currentpin == pos)
      {
        pos = pinpos << 2;
        /* Clear the corresponding low control register bits */
        pinmask = ((uint32_t)0x0F) << pos;
        tmpreg &= ~pinmask;
        /* Write the mode configuration in the corresponding bits */
        tmpreg |= (currentmode << pos);
        /* Reset the corresponding ODR bit */
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
        {
          GPIOx->BRR = (((uint32_t)0x01) << pinpos);
        }
        else
        {
          /* Set the corresponding ODR bit */
          if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
          {
            GPIOx->BSRR = (((uint32_t)0x01) << pinpos);
          }
        }
      }
    }
    GPIOx->CRL = tmpreg;
  }
/*---------------------------- GPIO CRH Configuration ------------------------*/
  /* Configure the eight high port pins */
  if (GPIO_InitStruct->GPIO_Pin > 0x00FF)
  {
    tmpreg = GPIOx->CRH;
    for (pinpos = 0x00; pinpos < 0x08; pinpos++)
    {
      pos = (((uint32_t)0x01) << (pinpos + 0x08));
      /* Get the port pins position */
      currentpin = ((GPIO_InitStruct->GPIO_Pin) & pos);
      if (currentpin == pos)
      {
        pos = pinpos << 2;
        /* Clear the corresponding high control register bits */
        pinmask = ((uint32_t)0x0F) << pos;
        tmpreg &= ~pinmask;
        /* Write the mode configuration in the corresponding bits */
        tmpreg |= (currentmode << pos);
        /* Reset the corresponding ODR bit */
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
        {
          GPIOx->BRR = (((uint32_t)0x01) << (pinpos + 0x08));
        }
        /* Set the corresponding ODR bit */
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
        {
          GPIOx->BSRR = (((uint32_t)0x01) << (pinpos + 0x08));
        }
      }
    }
    GPIOx->CRH = tmpreg;
  }
}

void USART_ClockInit(USART_TypeDef* USARTx, USART_ClockInitTypeDef* USART_ClockInitStruct)
{
  uint32_t tmpreg = 0x00;
  /* Check the parameters */
  assert_param(IS_USART_123_PERIPH(USARTx));
  assert_param(IS_USART_CLOCK(USART_ClockInitStruct->USART_Clock));
  assert_param(IS_USART_CPOL(USART_ClockInitStruct->USART_CPOL));
  assert_param(IS_USART_CPHA(USART_ClockInitStruct->USART_CPHA));
  assert_param(IS_USART_LASTBIT(USART_ClockInitStruct->USART_LastBit));
  
/*---------------------------- USART CR2 Configuration -----------------------*/
  tmpreg = USARTx->CR2;
  /* Clear CLKEN, CPOL, CPHA and LBCL bits */
  tmpreg &= CR2_CLOCK_CLEAR_Mask;
  /* Configure the USART Clock, CPOL, CPHA and LastBit ------------*/
  /* Set CLKEN bit according to USART_Clock value */
  /* Set CPOL bit according to USART_CPOL value */
  /* Set CPHA bit according to USART_CPHA value */
  /* Set LBCL bit according to USART_LastBit value */
  tmpreg |= (uint32_t)USART_ClockInitStruct->USART_Clock | USART_ClockInitStruct->USART_CPOL | 
                 USART_ClockInitStruct->USART_CPHA | USART_ClockInitStruct->USART_LastBit;
  /* Write to USART CR2 */
  USARTx->CR2 = (uint16_t)tmpreg;
}

void USART_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct)
{
  uint32_t tmpreg = 0x00, apbclock = 0x00;
  uint32_t integerdivider = 0x00;
  uint32_t fractionaldivider = 0x00;
  uint32_t usartxbase = 0;
  RCC_ClocksTypeDef RCC_ClocksStatus;
  /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_BAUDRATE(USART_InitStruct->USART_BaudRate));  
  assert_param(IS_USART_WORD_LENGTH(USART_InitStruct->USART_WordLength));
  assert_param(IS_USART_STOPBITS(USART_InitStruct->USART_StopBits));
  assert_param(IS_USART_PARITY(USART_InitStruct->USART_Parity));
  assert_param(IS_USART_MODE(USART_InitStruct->USART_Mode));
  assert_param(IS_USART_HARDWARE_FLOW_CONTROL(USART_InitStruct->USART_HardwareFlowControl));
  /* The hardware flow control is available only for USART1, USART2 and USART3 */
  if (USART_InitStruct->USART_HardwareFlowControl != USART_HardwareFlowControl_None)
  {
    assert_param(IS_USART_123_PERIPH(USARTx));
  }

  usartxbase = (uint32_t)USARTx;

/*---------------------------- USART CR2 Configuration -----------------------*/
  tmpreg = USARTx->CR2;
  /* Clear STOP[13:12] bits */
  tmpreg &= CR2_STOP_CLEAR_Mask;
  /* Configure the USART Stop Bits, Clock, CPOL, CPHA and LastBit ------------*/
  /* Set STOP[13:12] bits according to USART_StopBits value */
  tmpreg |= (uint32_t)USART_InitStruct->USART_StopBits;
  
  /* Write to USART CR2 */
  USARTx->CR2 = (uint16_t)tmpreg;

/*---------------------------- USART CR1 Configuration -----------------------*/
  tmpreg = USARTx->CR1;
  /* Clear M, PCE, PS, TE and RE bits */
  tmpreg &= CR1_CLEAR_Mask;
  /* Configure the USART Word Length, Parity and mode ----------------------- */
  /* Set the M bits according to USART_WordLength value */
  /* Set PCE and PS bits according to USART_Parity value */
  /* Set TE and RE bits according to USART_Mode value */
  tmpreg |= (uint32_t)USART_InitStruct->USART_WordLength | USART_InitStruct->USART_Parity |
            USART_InitStruct->USART_Mode;
  /* Write to USART CR1 */
  USARTx->CR1 = (uint16_t)tmpreg;

/*---------------------------- USART CR3 Configuration -----------------------*/  
  tmpreg = USARTx->CR3;
  /* Clear CTSE and RTSE bits */
  tmpreg &= CR3_CLEAR_Mask;
  /* Configure the USART HFC -------------------------------------------------*/
  /* Set CTSE and RTSE bits according to USART_HardwareFlowControl value */
  tmpreg |= USART_InitStruct->USART_HardwareFlowControl;
  /* Write to USART CR3 */
  USARTx->CR3 = (uint16_t)tmpreg;

/*---------------------------- USART BRR Configuration -----------------------*/
  /* Configure the USART Baud Rate -------------------------------------------*/
  RCC_GetClocksFreq(&RCC_ClocksStatus);
  if (usartxbase == USART1_BASE)
  {
    apbclock = RCC_ClocksStatus.PCLK2_Frequency;
  }
  else
  {
    apbclock = RCC_ClocksStatus.PCLK1_Frequency;
  }
  
  /* Determine the integer part */
  if ((USARTx->CR1 & CR1_OVER8_Set) != 0)
  {
    /* Integer part computing in case Oversampling mode is 8 Samples */
    integerdivider = ((25 * apbclock) / (2 * (USART_InitStruct->USART_BaudRate)));    
  }
  else /* if ((USARTx->CR1 & CR1_OVER8_Set) == 0) */
  {
    /* Integer part computing in case Oversampling mode is 16 Samples */
    integerdivider = ((25 * apbclock) / (4 * (USART_InitStruct->USART_BaudRate)));    
  }
  tmpreg = (integerdivider / 100) << 4;

  /* Determine the fractional part */
  fractionaldivider = integerdivider - (100 * (tmpreg >> 4));

  /* Implement the fractional part in the register */
  if ((USARTx->CR1 & CR1_OVER8_Set) != 0)
  {
    tmpreg |= ((((fractionaldivider * 8) + 50) / 100)) & ((uint8_t)0x07);
  }
  else /* if ((USARTx->CR1 & CR1_OVER8_Set) == 0) */
  {
    tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
  }
  
  /* Write to USART BRR */
  USARTx->BRR = (uint16_t)tmpreg;
}