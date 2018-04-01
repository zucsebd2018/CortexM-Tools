# 相关源代码
* AnalogIn.h 
* analogin_api.c 
* analogin_api.h 
* stm32f10x.h 
* stm32f10x_adc.h
* stm32f10x_adc.c

# AnalogIn.h
AnalogIn类中定义的构造函数、read()和read_u16()分别调用analogin_api.h中声明的analogin_init、analogin_read和analogin_read_u16函数。
``` 
class AnalogIn {
public:
    /** Create an AnalogIn, connected to the specified pin
     *
     * @param pin AnalogIn pin to connect to
     * @param name (optional) A string to identify the object
     */
    AnalogIn(PinName pin) {
        analogin_init(&_adc, pin);
    }

    /** Read the input voltage, represented as a float in the range [0.0, 1.0]
     *
     * @returns A floating-point value representing the current input voltage, measured as a percentage
     */
    float read() {
        return analogin_read(&_adc);
    }

    /** Read the input voltage, represented as an unsigned short in the range [0x0, 0xFFFF]
     *
     * @returns
     *   16-bit unsigned short representing the current input voltage, normalised to a 16-bit value
     */
    unsigned short read_u16() {
        return analogin_read_u16(&_adc);
    }
```

# analogin_api.c
## analogin_init
代码中定义的结构体可以在stm32f10x.h和stm32f10x_adc.h中查找。其中配置ADC初始化的核心代码如下
```
// The ADC initialization is done once
    if (adc_inited == 0) {
        adc_inited = 1;

        // Get ADC registers structure address
        adc = (ADC_TypeDef *)(obj->adc);

        // Enable ADC clock (14 MHz maximum)
        // PCLK2 = 64 MHz --> ADC clock = 64/6 = 10.666 MHz
        RCC_ADCCLKConfig(RCC_PCLK2_Div6);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

        // Configure ADC
        ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;
        ADC_InitStructure.ADC_ScanConvMode       = DISABLE;
        ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
        ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
        ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
        ADC_InitStructure.ADC_NbrOfChannel       = 1;
        ADC_Init(adc, &ADC_InitStructure);

        // Enable ADC
        ADC_Cmd(adc, ENABLE);

        // Calibrate ADC
        ADC_ResetCalibration(adc);
        while (ADC_GetResetCalibrationStatus(adc));
        ADC_StartCalibration(adc);
        while (ADC_GetCalibrationStatus(adc));
    }
```
中间几个重要参数设置说明：
* 第一个参数是ADC_Mode，这里设置为独立模式：
ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
在这个模式下，双ADC不能同步，每个ADC接口独立工作。所以如果不需要ADC同步或者只是用了一个ADC的时候，就应该设成独立模式了。
* 第二个参数是ADC_ScanConvMode，这里设置为DISABLE。
ADC_InitStructure.ADC_ScanConvMode = DISABLE;
如果只是用了一个通道的话，DISABLE就可以了，如果使用了多个通道的话，则必须将其设置为ENABLE。
* 第三个参数是ADC_ContinuousConvMode，这里设置为DISABLE，即单次转换。如果设置为ENABLE，则是连续转换。两者的区别在于连续转换直到所有的数据转换完成后才停止转换，而单次转换则只转换一次数据就停止，要再次触发转换才可以。所以如果需要一次性采集1024个数据或者更多，则采用连续转换。
* 第四个参数是ADC_ExternalTrigConv，即选择外部触发模式。这里只讲三种：
1. 第一种是最简单的软件触发，参数为ADC_ExternalTrigConv_None。设置好后还要记得调用库函数触发才会启动：
```
ADC_SoftwareStartConvCmd(ADC1, ENABLE);
```
2. 第二种是定时器通道输出触发。共有这几种：ADC_ExternalTrigConv_T1_CC1、ADC_ExternalTrigConv_T1_CC2、ADC_ExternalTrigConv_T2_CC2、
ADC_ExternalTrigConv_T3_T以及ADC_ExternalTrigConv_T4_CC4。定时器输出触发比较麻烦，还需要设置相应的定时器。

3. 第三种是外部引脚触发,对于规则通道，选择EXTI线11和TIM8_TRGO作为外部触发事件；而注入通道组则选择EXTI线15和TIM8_CC4作为外部触发事件。

* 第五个参数是ADC_DataAlign,这里设置为ADC_DataAlign_Right右对齐方式。建议采用右对齐方式，因为这样处理数据会比较方便。当然如果要从高位开始传输数据，那么采用左对齐优势就明显了。
* 第六个参数是ADC_NbrOfChannel，顾名思义：通道的数量。要是到多个通道采集数据的话就得设置一下这个参数。此外在规则通道组的配置函数中也许将各个通道的顺序定义一下，如：
```
ADC_RegularChannelConfig(ADC1,ADC_Channel_13,1,ADC_SampleTime_13Cycles5);
ADC_RegularChannelConfig(ADC1,ADC_Channel_14,2,ADC_SampleTime_13Cycles5);
```

## analogin_read和analogin_read_u16
analogin_read和analogin_read_u16函数都是直接调用内联函数adc_read，其中adc_read核心代码如下(调用到的函数在stm32f10x_adc.c中查找定义)
```
ADC_RegularChannelConfig(adc, channel, 1, ADC_SampleTime_7Cycles5);
ADC_SoftwareStartConvCmd(adc, ENABLE); // Start conversion
while (ADC_GetFlagStatus(adc, ADC_FLAG_EOC) == RESET); // Wait end of conversion
return (ADC_GetConversionValue(adc)); // Get conversion value
```
* 函数ADC_RegularChannelConfig用来设置指定ADC的规则组通道，设置转化顺序和采样时间。参数1可以是1、2或3来选择ADC外设；参数2是被设置的ADC通道，取值为ADC_Channel_0到ADC_Channel_17；参数3是采样顺序，取值为1到16；参数4是ADC通道的采样时间值，可以为ADC_SampleTime_1Cycles、ADC_SampleTime_7Cycles5、ADC_SampleTime_13Cycles5、ADC_SampleTime_28Cycles5、ADC_SampleTime_41Cycles5、ADC_SampleTime_55Cycles5、ADC_SampleTime_71Cycles5、ADC_SampleTime_239Cycles5。
* 函数ADC_GetFlagStatus用来判断ADC的flag状态，可以设置为ADC_FLAG_AWD: Analog watchdog flag；ADC_FLAG_EOC: End of conversion flag；ADC_FLAG_JEOC: End of injected group conversion flag；ADC_FLAG_JSTRT: Start of injected group conversion flag；ADC_FLAG_STRT: Start of regular group conversion flag

# 参考资料
https://blog.csdn.net/gtkknd/article/details/52863295
https://blog.csdn.net/qq_27312943/article/details/51380987
http://blog.sina.com.cn/s/blog_5fd719d60100sd5a.html
