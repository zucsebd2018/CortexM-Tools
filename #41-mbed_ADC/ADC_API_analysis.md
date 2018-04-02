## Mbed ADC API analysis

The source code is at https://os.mbed.com/users/mbed_official/code/mbed-dev/



#### class AnalogIn

Position: **driver/Analogin.h**

Let's have a look at the constructor first:

```c
AnalogIn(PinName pin) {
    lock();
    analogin_init(&_adc, pin);
    unlock();
}
```

All the function definitions are in the .h file instead of the .cpp file because all of these functions are very tiny. There are 3 interfaces here: `read()`, `read_u16()`, `operator float()` (Overloaded type). 

Each of them use the locks operations `lock()` and `unlock()`. They use a **mutual exclusive lock** called the class `mutex` which is stated in the .cpp file, **drivers/AnalogIn.cpp**.

The definition of the `read()` function and `read_u16()` function:

```c
float read() {
    lock();
    float ret = analogin_read(&_adc);
    unlock();
    return ret;
}

unsigned short read_u16() {
    lock();
    unsigned short ret = analogin_read_u16(&_adc);
    unlock();
    return ret;
}
```



We may pay attention to these 3 important functions:

+ **analogin_init** (In the constructor)
+ **analogin_read_u16()** (In `read_u16()`)
+ **analogin_read**  (In `read()`)

They are included in **hal/analogin_api.h**, but their implementation is dependent on specific hardwares. 

So we go to **targets/TARGET_STM**, just as what we did before in the analysis on functions about serial ports in mbed libraries.



#### targets/TARGET_STM/analogin_api.c

At first, we should check **analogin_api.c** first.

```c
uint16_t adc_read(analogin_t *obj);
 
uint16_t analogin_read_u16(analogin_t *obj)
{
    uint16_t value = adc_read(obj);
    // 12-bit to 16-bit conversion
    value = ((value << 4) & (uint16_t)0xFFF0) | ((value >> 8) & (uint16_t)0x000F);
    return value;
}
 
float analogin_read(analogin_t *obj)
{
    uint16_t value = adc_read(obj);
    return (float)value * (1.0f / (float)0xFFF); // 12 bits range
}
```

We can see both of `analogin_read_u16()` and `analogin_read()` calls the same function `adc_read()`.

So the only two function need to analyse are`adc_read()` and `analogin_init()`.



#### targets/TARGET_STM/TARGET_STM32F1/analogin_device.c

##### analogin_init()

The `analogin_init()` function is showed below:

```c
void analogin_init(analogin_t *obj, PinName pin)
{
    static bool adc_calibrated = false;
    RCC_PeriphCLKInitTypeDef  PeriphClkInit;
    uint32_t function = (uint32_t)NC;
 
    // ADC Internal Channels "pins"  (Temperature, Vref, Vbat, ...)
    //   are described in PinNames.h and PeripheralPins.c
    //   Pin value must be between 0xF0 and 0xFF
    if ((pin < 0xF0) || (pin >= 0x100)) {
        // Normal channels
        // Get the peripheral name from the pin and assign it to the object
        obj->handle.Instance = (ADC_TypeDef *)pinmap_peripheral(pin, PinMap_ADC);
        // Get the functions (adc channel) from the pin and assign it to the object
        function = pinmap_function(pin, PinMap_ADC);
        // Configure GPIO
        pinmap_pinout(pin, PinMap_ADC);
    } else {
        // Internal channels
        obj->handle.Instance = (ADC_TypeDef *)pinmap_peripheral(pin, PinMap_ADC_Internal);
        function = pinmap_function(pin, PinMap_ADC_Internal);
        // No GPIO configuration for internal channels
    }
    MBED_ASSERT(obj->handle.Instance != (ADC_TypeDef *)NC);
    MBED_ASSERT(function != (uint32_t)NC);
 
    obj->channel = STM_PIN_CHANNEL(function);
 
    // Save pin number for the read function
    obj->pin = pin;
 
    // Enable ADC clock
    __HAL_RCC_ADC1_CLK_ENABLE();
 
    // Configure ADC object structures
    obj->handle.State = HAL_ADC_STATE_RESET;
    obj->handle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    obj->handle.Init.ScanConvMode          = DISABLE;
    obj->handle.Init.ContinuousConvMode    = DISABLE;
    obj->handle.Init.NbrOfConversion       = 1;
    obj->handle.Init.DiscontinuousConvMode = DISABLE;
    obj->handle.Init.NbrOfDiscConversion   = 0;
    obj->handle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
 
    if (HAL_ADC_Init(&obj->handle) != HAL_OK) {
        error("Cannot initialize ADC");
    }
 
    // This section is done only once
    if (!adc_calibrated) {
        adc_calibrated = true;
        // Configure ADC clock prescaler
        // Caution: On STM32F1, ADC clock frequency max is 14 MHz (refer to device datasheet).
        // Therefore, ADC clock prescaler must be configured in function
        // of ADC clock source frequency to remain below this maximum frequency.
        // with 8 MHz external xtal: PCLK2 = 72 MHz --> ADC clock = 72/6 = 12 MHz
        // with internal clock     : PCLK2 = 64 MHz --> ADC clock = 64/6 = 10.67 MHz
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
        PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
        HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
        // Calibration
        HAL_ADCEx_Calibration_Start(&obj->handle);
    }
}
```

There are 4 HAL functions here, which is defined in **targets/TARGET_STM/TARGET_STM32F1/device/stm32f1xx_hal_adc.c**.

+ **__HAL_RCC_ADC1_CLK_ENABLE();**
+ **HAL_ADC_Init(&obj->handle)**
+ **HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);**
+ **HAL_ADCEx_Calibration_Start(&obj->handle);**

`__HAL_RCC_ADC1_CLK_ENABLE()` and `HAL_RCCEx_PeriphCLKConfig()` are used to configure the hardware clock;

`HAL_ADC_Init()` is for initializing the ADC device, and `HAL_ADCEx_Calibration_Start()` is for calibration.

These are not very important here because what we focus on is how the ADC functions read the values of voltages.



##### adc_read()

The `adc_read()` function is showed below (Some parts of it is omitted):

```c
uint16_t adc_read(analogin_t *obj)
{
    ADC_ChannelConfTypeDef sConfig = {0};
 
    // Configure ADC channel
    sConfig.Rank         = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
 
    switch (obj->channel) {
        case 0:
            sConfig.Channel = ADC_CHANNEL_0;
            break;
		...
        case 17:
            sConfig.Channel = ADC_CHANNEL_VREFINT;
            break;
        default:
            return 0;
    }
 
    HAL_ADC_ConfigChannel(&obj->handle, &sConfig);
 
    HAL_ADC_Start(&obj->handle); // Start conversion
 
    // Wait end of conversion and get value
    if (HAL_ADC_PollForConversion(&obj->handle, 10) == HAL_OK) {
        return (uint16_t)HAL_ADC_GetValue(&obj->handle);
    } else {
        return 0;
    }
}
```

We can also pay attention to the functions with HAL prefixes.

+ **HAL_ADC_ConfigChannel(&obj->handle, &sConfig)**
+ **HAL_ADC_Start(&obj->handle)**
+ **HAL_ADC_PollForConversion(&obj->handle, 10)**
+ **HAL_ADC_GetValue(&obj->handle)**

And also move to the .c file which defines three of them.



#### HAL_ADC_ConfigChannel()

Position: **targets/TARGET_STM/TARGET_STM32F1/device/stm32f1xx_hal_adc.c**

After the function checks some parameters and locks the hardware:

```c
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
  assert_param(IS_ADC_CHANNEL(sConfig->Channel));
  assert_param(IS_ADC_REGULAR_RANK(sConfig->Rank));
  assert_param(IS_ADC_SAMPLE_TIME(sConfig->SamplingTime));
  /* Process locked */
  __HAL_LOCK(hadc);
```



It starts to modify the registers that sets the "rank" of ADC device.

```c
  if (sConfig->Rank < 7U)
  {
    MODIFY_REG(hadc->Instance->SQR3                        ,
               ADC_SQR3_RK(ADC_SQR3_SQ1, sConfig->Rank)    ,
               ADC_SQR3_RK(sConfig->Channel, sConfig->Rank) );
  }
  /* For Rank 7 to 12 */
  else if (sConfig->Rank < 13U)
  {
    MODIFY_REG(hadc->Instance->SQR2                        ,
               ADC_SQR2_RK(ADC_SQR2_SQ7, sConfig->Rank)    ,
               ADC_SQR2_RK(sConfig->Channel, sConfig->Rank) );
  }
  /* For Rank 13 to 16 */
  else
  {
    MODIFY_REG(hadc->Instance->SQR1                        ,
               ADC_SQR1_RK(ADC_SQR1_SQ13, sConfig->Rank)   ,
               ADC_SQR1_RK(sConfig->Channel, sConfig->Rank) );
  }
  
  
  /* Channel sampling time configuration */
  /* For channels 10 to 17 */
  if (sConfig->Channel >= ADC_CHANNEL_10)
  {
    MODIFY_REG(hadc->Instance->SMPR1                             ,
               ADC_SMPR1(ADC_SMPR1_SMP10, sConfig->Channel)      ,
               ADC_SMPR1(sConfig->SamplingTime, sConfig->Channel) );
  }
  else /* For channels 0 to 9 */
  {
    MODIFY_REG(hadc->Instance->SMPR2                             ,
               ADC_SMPR2(ADC_SMPR2_SMP0, sConfig->Channel)       ,
               ADC_SMPR2(sConfig->SamplingTime, sConfig->Channel) );
  }
```



For some special channels(For **STM32F1**, the VrefInt/TempSensor channels), it should delay for serval seconds. 

```c
if (hadc->Instance == ADC1)
{
    if (READ_BIT(hadc->Instance->CR2, ADC_CR2_TSVREFE) == RESET)
    {
        SET_BIT(hadc->Instance->CR2, ADC_CR2_TSVREFE);

        if ((sConfig->Channel == ADC_CHANNEL_TEMPSENSOR))
        {
            /* Delay for temperature sensor stabilization time */
            /* Compute number of CPU cycles to wait for */
            wait_loop_index = (ADC_TEMPSENSOR_DELAY_US * (SystemCoreClock / 1000000U));
            while(wait_loop_index != 0U)
            {
                wait_loop_index--;
            }
        }
    }
}
```



Then unlock and return the status.

```c
  /* Process unlocked */
  __HAL_UNLOCK(hadc);
  
  /* Return function status */
  return tmp_hal_status;
```



#### HAL_ADC_Start()

###### Lock

The lock operation is strongly required.

```c
  /* Process locked */
  __HAL_LOCK(hadc);
```



###### Modify the ADC state

There are different conditions in this function. 

If the ADC device is multimoded, it sets the SLAVE bit on the status in the hardware.
If it is not, then the bit is clear.

Both of the conditions need to check if the conversions are triggering group injected.

```c
/* Set group injected state (from auto-injection) and multimode state     */
/* for all cases of multimode: independent mode, multimode ADC master     */
/* or multimode ADC slave (for devices with several ADCs):                */
if (ADC_NONMULTIMODE_OR_MULTIMODEMASTER(hadc))
{
    /* Set ADC state (ADC independent or master) */
    CLEAR_BIT(hadc->State, HAL_ADC_STATE_MULTIMODE_SLAVE);

    /* If conversions on group regular are also triggering group injected,  */
    /* update ADC state.                                                    */
    if (READ_BIT(hadc->Instance->CR1, ADC_CR1_JAUTO) != RESET)
    {
        ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_INJ_EOC, HAL_ADC_STATE_INJ_BUSY);  
    }
}
else
{
    /* Set ADC state (ADC slave) */
    SET_BIT(hadc->State, HAL_ADC_STATE_MULTIMODE_SLAVE);

    /* If conversions on group regular are also triggering group injected,  */
    /* update ADC state.                                                    */
    if (ADC_MULTIMODE_AUTO_INJECTED(hadc))
    {
        ADC_STATE_CLR_SET(hadc->State, HAL_ADC_STATE_INJ_EOC, HAL_ADC_STATE_INJ_BUSY);
    }
}
```



###### Clear Error code

Then it checks if an injected conversion is ongoing, and clear some or all the error bits on the hardware.

```c
if (HAL_IS_BIT_SET(hadc->State, HAL_ADC_STATE_INJ_BUSY))
{
    /* Reset ADC error code fields related to conversions on group regular */
    CLEAR_BIT(hadc->ErrorCode, (HAL_ADC_ERROR_OVR | HAL_ADC_ERROR_DMA));         
}
else
{
    /* Reset ADC all error code fields */
    ADC_CLEAR_ERRORCODE(hadc);
}
```



###### Unlock

It should be unlocked before the conversions starts, because some interruptions may occur.

```c
/* Process unlocked */
/* Unlock before starting ADC conversions: in case of potential           */
/* interruption, to let the process to ADC IRQ Handler.                   */
__HAL_UNLOCK(hadc);
```



###### Start conversion

There are some different cases here:

+ If software start has been selected, conversion starts immediately.
+ If external trigger has been selected, conversion will start at next trigger event.

Case of multimode enabled:

* If ADC is slave, ADC is enabled only (conversion is not started).
* if ADC is master, ADC is enabled and conversion is started. 
* Not multimode, ADC is enabled and conversion is started.

```c
/* Note: Alternate trigger for single conversion could be to force an     */
/*       additional set of bit ADON "hadc->Instance->CR2 |= ADC_CR2_ADON;"*/
if (ADC_IS_SOFTWARE_START_REGULAR(hadc)      &&
    ADC_NONMULTIMODE_OR_MULTIMODEMASTER(hadc)  )
{
    /* Start ADC conversion on regular group with SW start */
    SET_BIT(hadc->Instance->CR2, (ADC_CR2_SWSTART | ADC_CR2_EXTTRIG));
}
else
{
    /* Start ADC conversion on regular group with external trigger */
    SET_BIT(hadc->Instance->CR2, ADC_CR2_EXTTRIG);
}
```



#### HAL_ADC_PollForConversion()

######Get the start time(tick)

``` c
   /* Get tick count */
  tickstart = HAL_GetTick();
```



######Verify that ADC is not in DMA mode

```c
  /* Verification that ADC configuration is compliant with polling for        */
  /* each conversion:                                                         */
  /* Particular case is ADC configured in DMA mode                            */
  if (HAL_IS_BIT_SET(hadc->Instance->CR2, ADC_CR2_DMA))
  {
    /* Update ADC state machine to error */
    SET_BIT(hadc->State, HAL_ADC_STATE_ERROR_CONFIG);
    
    /* Process unlocked */
    __HAL_UNLOCK(hadc);
    
    return HAL_ERROR;
  }
```



######Polling for end of conversion

It waits**(polls)** until the EOC flag is raised. If the delay is too long, then a TIMEOUT error is raised.

```c
if (HAL_IS_BIT_CLR(hadc->Instance->CR1, ADC_CR1_SCAN) &&
    HAL_IS_BIT_CLR(hadc->Instance->SQR1, ADC_SQR1_L)    )
{
    /* Wait until End of Conversion flag is raised */
    while(HAL_IS_BIT_CLR(hadc->Instance->SR, ADC_FLAG_EOC))
    {
        /* Check if timeout is disabled (set to infinite wait) */
        if(Timeout != HAL_MAX_DELAY)
        {
            if((Timeout == 0U) || ((HAL_GetTick() - tickstart ) > Timeout))
            {
                /* Update ADC state machine to timeout */
                SET_BIT(hadc->State, HAL_ADC_STATE_TIMEOUT);

                /* Process unlocked */
                __HAL_UNLOCK(hadc);

                return HAL_TIMEOUT;
            }
        }
    }
}
else
{
    /* Replace polling by wait for maximum conversion time */
    /*  - Computation of CPU clock cycles corresponding to ADC clock cycles   */
    /*    and ADC maximum conversion cycles on all channels.                  */
    /*  - Wait for the expected ADC clock cycles delay                        */
    Conversion_Timeout_CPU_cycles_max = ((SystemCoreClock
                                          / HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_ADC))
                                         * ADC_CONVCYCLES_MAX_RANGE(hadc)                 );

    while(Conversion_Timeout_CPU_cycles < Conversion_Timeout_CPU_cycles_max)
    {
        /* Check if timeout is disabled (set to infinite wait) */
        if(Timeout != HAL_MAX_DELAY)
        {
            if((Timeout == 0U) || ((HAL_GetTick() - tickstart) > Timeout))
            {
                /* Update ADC state machine to timeout */
                SET_BIT(hadc->State, HAL_ADC_STATE_TIMEOUT);

                /* Process unlocked */
                __HAL_UNLOCK(hadc);

                return HAL_TIMEOUT;
            }
        }
        Conversion_Timeout_CPU_cycles ++;
    }
}
```



###### The remaining work

```c
/* Clear regular group conversion flag */
__HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_STRT | ADC_FLAG_EOC);

/* Update ADC state machine */
SET_BIT(hadc->State, HAL_ADC_STATE_REG_EOC);

/* Determine whether any further conversion upcoming on group regular       */
/* by external trigger, continuous mode or scan sequence on going.          */
/* Note: On STM32F1 devices, in case of sequencer enabled                   */
/*       (several ranks selected), end of conversion flag is raised         */
/*       at the end of the sequence.                                        */
if(ADC_IS_SOFTWARE_START_REGULAR(hadc)        && 
   (hadc->Init.ContinuousConvMode == DISABLE)   )
{   
    /* Set ADC state */
    CLEAR_BIT(hadc->State, HAL_ADC_STATE_REG_BUSY);   

    if (HAL_IS_BIT_CLR(hadc->State, HAL_ADC_STATE_INJ_BUSY))
    { 
        SET_BIT(hadc->State, HAL_ADC_STATE_READY);
    }
```



#### HAL_ADC_GetValue(&obj->handle)

It reads the result of conversion directly, from the **hadc** struct.

```c
uint32_t HAL_ADC_GetValue(ADC_HandleTypeDef* hadc)
{
  /* Check the parameters */
  assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));
 
  /* Note: EOC flag is not cleared here by software because automatically     */
  /*       cleared by hardware when reading register DR.                      */
  
  /* Return ADC converted value */ 
  return hadc->Instance->DR;
}
```