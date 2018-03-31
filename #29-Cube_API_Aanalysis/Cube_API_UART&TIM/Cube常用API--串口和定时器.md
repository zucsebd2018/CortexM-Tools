# Cube常用API--串口和定时器相关

## 串口相关API（UART）

### 核心结构体

`UART_HandleTypeDe`:

```C
typedef struct
{
  USART_TypeDef                 *Instance;        /*!< UART registers base address        */

  UART_InitTypeDef              Init;             /*!< UART communication parameters      */

  uint8_t                       *pTxBuffPtr;      /*!< Pointer to UART Tx transfer Buffer */

  uint16_t                      TxXferSize;       /*!< UART Tx Transfer size              */

  __IO uint16_t                 TxXferCount;      /*!< UART Tx Transfer Counter           */

  uint8_t                       *pRxBuffPtr;      /*!< Pointer to UART Rx transfer Buffer */

  uint16_t                      RxXferSize;       /*!< UART Rx Transfer size              */

  __IO uint16_t                 RxXferCount;      /*!< UART Rx Transfer Counter           */

  DMA_HandleTypeDef             *hdmatx;          /*!< UART Tx DMA Handle parameters      */

  DMA_HandleTypeDef             *hdmarx;          /*!< UART Rx DMA Handle parameters      */

  HAL_LockTypeDef               Lock;             /*!< Locking object                     */

  __IO HAL_UART_StateTypeDef    gState;           /*!< UART state information related to global Handle management 
                                                       and also related to Tx operations.
                                                       This parameter can be a value of @ref HAL_UART_StateTypeDef */
  
  __IO HAL_UART_StateTypeDef    RxState;          /*!< UART state information related to Rx operations.
                                                       This parameter can be a value of @ref HAL_UART_StateTypeDef */

  __IO uint32_t                 ErrorCode;        /*!< UART Error code                    */
}UART_HandleTypeDef;
```
* Instance是 USART_TypeDef结构类型指针，是执行寄存器基址； 
* Init是 UART_InitTypeDef 结构体指针，用于设置串口各个参数； 
* pTxBuffPtr，TxXferSize 和 TxXferCount 用来设置串口发送的数据缓存指针，用于标记发送数据量还有剩余的要发送的数据量； 
* pRxBuffPtr，RxXferSize 和 RxXferCount 是设置接受数据缓存指针，接受最大数据量以及剩余的接受数据量； 
* hdmatx 和 hdmarx 是串口 DMA 相关的变量，指向 DMA 句柄； 
* AdvancedInit 是用来配置串口的高级功能； 
* 其余三个变量为HAL处理过程状态标志位和串口通信的错误码。

### 初始化函数

`HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart);`

当调用这一API时，会根据huart结构体中的`UART_InitTypeDef`结构体中的各个参数来对串口进行初始化。

`UART_InitTypeDef`:

```C
typedef struct
{
  uint32_t BaudRate;                  /*!< This member configures the UART communication baud rate.
                                           The baud rate is computed using the following formula:
                                           - IntegerDivider = ((PCLKx) / (16 * (huart->Init.BaudRate)))
                                           - FractionalDivider = ((IntegerDivider - ((uint32_t) IntegerDivider)) * 16) + 0.5 */

  uint32_t WordLength;                /*!< Specifies the number of data bits transmitted or received in a frame.
                                           This parameter can be a value of @ref UART_Word_Length */

  uint32_t StopBits;                  /*!< Specifies the number of stop bits transmitted.
                                           This parameter can be a value of @ref UART_Stop_Bits */

  uint32_t Parity;                    /*!< Specifies the parity mode.
                                           This parameter can be a value of @ref UART_Parity
                                           @note When parity is enabled, the computed parity is inserted
                                                 at the MSB position of the transmitted data (9th bit when
                                                 the word length is set to 9 data bits; 8th bit when the
                                                 word length is set to 8 data bits). */

  uint32_t Mode;                      /*!< Specifies whether the Receive or Transmit mode is enabled or disabled.
                                           This parameter can be a value of @ref UART_Mode */

  uint32_t HwFlowCtl;                 /*!< Specifies whether the hardware flow control mode is enabled or disabled.
                                           This parameter can be a value of @ref UART_Hardware_Flow_Control */

  uint32_t OverSampling;              /*!< Specifies whether the Over sampling 8 is enabled or disabled, to achieve higher speed (up to fPCLK/8).
                                           This parameter can be a value of @ref UART_Over_Sampling. This feature is only available 
                                           on STM32F100xx family, so OverSampling parameter should always be set to 16. */
}UART_InitTypeDef;
```
* BaudRate：波特率； 
* WordLength：字长，可以设置为8或者9； 
* StopBits：停止位，可以设置1或2个； 
* Parity：奇偶校验； 
* Mode：串口模式，可为只发，只收，或者收发模式； 
* HwFlowCtl：硬件流控制； 
* OverSampling：采样为16倍或者8倍；


示例函数：

```C
static void MX_USART1_UART_Init(void)
{
  //设置参数
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  //更多参数...
  //调用初始化函数
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
}
```

### IO相关API

#### HAL_UART_Transmit

`HAL_StatusTypeDef HAL_UART_Transmit
(UART_HandleTypeDef * huart, uint8_t * pData, uint16_t Size,
uint32_t Timeout);`

串口发送，发送指定长度的数据。如果超时没发送完成，则不再发送，返回超时标志（HAL_TIMEOUT）。

* huart:一个指向UART_HandleTypeDef结构的指针
* pData：数据缓冲区
* Size：数据长度
* Timeout：设置超时

#### HAL_UART_Receive

`HAL_StatusTypeDef HAL_UART_Receive
(UART_HandleTypeDef * huart, uint8_t * pData, uint16_t Size,
uint32_t Timeout)`

串口接收，接收指定长度的数据。如果超时没接收完成，则不再接收数据到指定缓冲区，返回超时标志（HAL_TIMEOUT）。

参数基本与上同。

#### HAL_UART_Transmit_IT

`HAL_StatusTypeDef HAL_UART_Transmit_IT
(UART_HandleTypeDef * huart, uint8_t * pData, uint16_t Size)`

串口中断发送，以中断方式发送指定长度的数据。把发送缓冲区指针指向要发送的数据，设置发送长度、发送计数器初值，然后使能串口发送中断，触发串口中断。然后串口中断函数处理，直到数据发送完成，而后关闭中断，不再发送数据，串口发送完成回调函数。

参数基本与上同，但是没有Timeout参数了。

#### HAL_UART_Receive_IT

`HAL_StatusTypeDef HAL_UART_Receive_IT
(UART_HandleTypeDef * huart, uint8_t * pData, uint16_t Size)`

串口终端接受，以中断方式发送指定长度的数据，具体细节与中断发送相类似。

参数基本同上。

#### HAL_UART_Transmit_DMA

`HAL_StatusTypeDef HAL_UART_Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);`

串口DMA发送，以DMA方式发送指定长度的数据。把发送缓冲区指针指向要发送的数据，设置发送长度，发送计数器初值，设置DMA传输完成中断的回调函数，使能DMA控制器中断，使能DMA控制器传输，使能UART的DMA传输请求。UART会发送数据，直到发送完成，触发DMA中断。

####　HAL_UART_Receive_DMA

`HAL_StatusTypeDef HAL_UART_Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);`

串口DMA接受，以DMA方式接收指定长度的数据。具体细节与DMA发送类似。

参数基本同上

#### 其他

除了上述几个io的API，HAL库中还有对应的Abort函数，即中断对应的行为，这里举出一些：

* `HAL_UART_Abort`
* `HAL_UART_AbortTransmit`
* `HAL_UART_Abort_IT`
* `HAL_UART_AbortTransmit_IT`
* `HAL_UART_AbortReceive_IT`
* `HAL_UART_AbortReceive`

## 定时器相关API

```C
typedef struct
{
  TIM_TypeDef                 *Instance;     /*!< Register base address             */
  TIM_Base_InitTypeDef        Init;          /*!< TIM Time Base required parameters */
  HAL_TIM_ActiveChannel       Channel;       /*!< Active channel                    */
  DMA_HandleTypeDef           *hdma[7U];     /*!< DMA Handlers array
                                                This array is accessed by a @ref TIM_DMA_Handle_index */
  HAL_LockTypeDef             Lock;          /*!< Locking object                    */
  __IO HAL_TIM_StateTypeDef   State;         /*!< TIM operation state               */
}TIM_HandleTypeDef;
```

同样也是一个句柄，和串口的结构体非常相似，里面也有相应的初始化结构体，存有相应的参数，在执行初始化时会用到。

### 初始化

初始化结构体：

```C
typedef struct
{
  uint32_t Prescaler;         /*!< Specifies the prescaler value used to divide the TIM clock.
                                   This parameter can be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF */

  uint32_t CounterMode;       /*!< Specifies the counter mode.
                                   This parameter can be a value of @ref TIM_Counter_Mode */

  uint32_t Period;            /*!< Specifies the period value to be loaded into the active
                                   Auto-Reload Register at the next update event.
                                   This parameter can be a number between Min_Data = 0x0000 and Max_Data = 0xFFFF.  */

  uint32_t ClockDivision;     /*!< Specifies the clock division.
                                   This parameter can be a value of @ref TIM_ClockDivision */

  uint32_t RepetitionCounter;  /*!< Specifies the repetition counter value. Each time the RCR downcounter
                                    reaches zero, an update event is generated and counting restarts
                                    from the RCR value (N).
                                    This means in PWM mode that (N+1) corresponds to:
                                        - the number of PWM periods in edge-aligned mode
                                        - the number of half PWM period in center-aligned mode
                                     This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF.
                                     @note This parameter is valid only for TIM1 and TIM8. */
									 
  uint32_t AutoReloadPreload;  /*!< Specifies the auto-reload preload.
                                    This parameter can be a value of @ref TIM_AutoReloadPreload */
} TIM_Base_InitTypeDef;
```

#### HAL_TIM_Base_Init

`HAL_StatusTypeDef HAL_TIM_Base_Init (TIM_HandleTypeDef
*htim)`

初始化的方法与UART是类似的，同样的通过`TIM_HandleTypeDef`中的`TIM_Base_InitTypeDef`设置相关参数后执行这一参数即可。

### 其他API

#### HAL_TIM_Base_Start

`HAL_StatusTypeDef HAL_TIM_Base_Start
(TIM_HandleTypeDef * htim)`

开始定时器

#### HAL_TIM_Base_Stop

`HAL_StatusTypeDef HAL_TIM_Base_Stop
(TIM_HandleTypeDef * htim)`

停止定时器

#### HAL_TIM_Base_Start_IT

`HAL_StatusTypeDef HAL_TIM_Base_Start_IT
(TIM_HandleTypeDef * htim)`

以中断方式开始定时器。

#### HAL_TIM_Base_Stop_IT

`HAL_StatusTypeDef HAL_TIM_Base_Stop_IT
(TIM_HandleTypeDef * htim)`

以中断方式结束定时器

#### HAL_TIM_Base_Start_DMA

`HAL_StatusTypeDef HAL_TIM_Base_Start_DMA
(TIM_HandleTypeDef * htim, uint32_t * pData, uint16_t Length)`

以DMA方式开始定时器

#### HAL_TIM_Base_Stop_DMA

`HAL_StatusTypeDef HAL_TIM_Base_Stop_DMA
(TIM_HandleTypeDef * htim)`

以DMA方式结束定时器