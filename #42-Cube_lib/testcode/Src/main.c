
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
ADC_HandleTypeDef hadc1;
/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MY_GPIO_Init();
static void MY_UART_Init();
static void MY_ADC_Init();
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
  MY_GPIO_Init();
  MY_ADC_Init();
  MY_UART_Init();
  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */
  HAL_ADCEx_Calibration_Start(&hadc1);
  
  uint32_t adc1_value;
  float adc1_v;
  char buf[100];
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_ADC_Start(&hadc1);
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
   HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_2);//闪烁
   HAL_Delay(500);
   HAL_ADC_PollForConversion(&hadc1, 100);//轮询adc
   adc1_value = HAL_ADC_GetValue(&hadc1);//取得测到的值
   adc1_v = (float)adc1_value / 4095 * 3.3;//值换算
   sprintf(buf, "Current Voltage is: %.2f\n", adc1_v);
   HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), 0xFFFF);//串口输出
  }
  HAL_ADC_Stop(&hadc1);
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

static void MY_GPIO_Init(){
  __HAL_RCC_GPIOA_CLK_ENABLE();//启用GPIO A组引脚
  GPIO_InitTypeDef GPIO_InitStruct;//GPIO配置器

  //PA9 串口输出
  GPIO_InitStruct.Pin = GPIO_PIN_9;//配置的引脚号
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;//引脚模式
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;//引脚速度
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);//配置相应的引脚

  //PA10 串口输入
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;//引脚拉高or拉低ornull
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  //PA2 输出用于闪烁灯
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

static void MY_UART_Init(){
  huart1.Instance = USART1;// 启用USART1， 还可以用2,3，USART123是相应寄存器的地址
  huart1.Init.BaudRate = 9600;// 设置波特率 9600
  huart1.Init.WordLength = UART_WORDLENGTH_8B;// 设置数据位 8bit
  huart1.Init.StopBits = UART_STOPBITS_1;// 设置停止位 1
  huart1.Init.Parity = UART_PARITY_NONE;// 设置奇偶校验位 无
  huart1.Init.Mode = UART_MODE_TX_RX;// 设置串口模式，全双工
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;//设置流控 无
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;//设置过采样 16倍
  if (HAL_UART_Init(&huart1) != HAL_OK) //初始化串口
  {
    _Error_Handler(__FILE__, __LINE__);//初始化失败
  }
}

static void MY_ADC_Init(){
  ADC_ChannelConfTypeDef sConfig;//通道配置器
  hadc1.Instance = ADC1;//启用ADC1 还可以是ADC2
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;//不使用扫描模式
  hadc1.Init.ContinuousConvMode = ENABLE;//使用连续模式
  hadc1.Init.DiscontinuousConvMode = DISABLE;//不使用间断模式
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;//关闭外部触发
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;//数据右对齐
  hadc1.Init.NbrOfConversion = 1;//一组一通道，因为没用到间断模式
  if (HAL_ADC_Init(&hadc1) != HAL_OK)//初始化adc
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfig.Channel = ADC_CHANNEL_0;//配置通道0
  sConfig.Rank = ADC_REGULAR_RANK_1;//配置为常规组
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;//采样时间1.5个ADC时钟周期
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)//初始化ADC通道
  {
    _Error_Handler(__FILE__, __LINE__);
  }
}
/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
