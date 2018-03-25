# 常用API——初始化和GPIO
## 初始化 
`RCC_OscInitTypeDef`
类
振荡器配置器
成员：
- OscillatorType：振荡器类型
- xxxState: 有HSE LSE HSI等等，表示使不使用这些振荡器
- PLL：PLL对象

`RCC_PLLInitTypeDef`
类
PLL初始化
成员：
- PLLState： 使不使用PLL
- PLLSource：PLL源振荡器
- PLLMUL：倍频

`RCC_ClkInitTypeDef`
类
时钟配置器
成员：
- ClockType：时钟类型
- SYSCLKSource：时钟源
- AHBCLKDivider：AHB分频
- APB1CLKDivider：APB1分频
- APB2CLKDivider：APB2分频


`HAL_StatusTypeDef HAL_Init(void)`
函数
初始化HAL
无参

`HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef  *RCC_OscInitStruct)`
函数
初始化振荡器
参数：
- RCC_OscInitStruct：振荡器配置器指针
返回值：是否初始化成功（HAL_OK,HAL_ERROR,HAL_BUSY,HAL_TIMEOUT)

`HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef  *RCC_ClkInitStruct, uint32_t FLatency)`
函数
初始化时钟
参数：
- RCC_ClkInitStruct：时钟配置器指针
- FLatency：Flash Latency
返回值：是否初始化成功



##GPIO
`GPIO_InitTypeDef`
类
GPIO配置器
成员：
- Pin: 引脚 （GPIO_PIN_0- GPIO_PIN_15)
- Mode: 引脚模式 （输入输出复用等等）
- Pull: 引脚拉高拉低
- Speed：引脚速度（高速，中速，低速）

` __HAL_RCC_GPIOA_CLK_ENABLE()`
函数（其实是一个宏）
启用A组GPIO，（还有BCD组GPIO，函数类似）

`void  HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)`
函数
初始化GPIO
参数：
- GPIOx：GPIO组对象指针，可以是GPIOA，GPIOB等
- GPIO_Init: GPIO配置器指针

`GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);`
函数
读取GPIO输入
参数：
- GPIOx: GPIO组对象指针，可以是GPIOA,GPIOB等
- GPIO_Pin: GPIO_Pin 引脚号，GPIO_PIN_0 - 15
`void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);`
和上述类似
功能是写GPIO
`void HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);`
和上述类似
功能是切换GPIO引脚高低
`HAL_StatusTypeDef HAL_GPIO_LockPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);`
和上述类似
功能是锁存GPIO引脚状态，1保持1,0保持0
`void HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin);`
`void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);`
这两个用于GPIO中断，暂时不知道怎么用

实际使用这些函数和类，可以看代码实践.md