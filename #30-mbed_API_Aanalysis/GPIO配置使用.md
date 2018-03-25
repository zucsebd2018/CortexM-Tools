# GPIO使用
mbed提供了一些在中间层HAL上调用的API函数，函数声明在头文件mbed-STM32F103C8T6/gpio_api.h中。
```
/* Set the given pin as GPIO and return the GPIO port mask for this pin*/
uint32_t gpio_set(PinName pin);

/* Checks if gpio object is connected (pin was not initialized with NC) */
int gpio_is_connected(const gpio_t *obj);

/* Initialize the GPIO pin */
void gpio_init(gpio_t *obj, PinName pin);

/* Set the input pin mode */
void gpio_mode(gpio_t *obj, PinMode mode);

/* Set the pin direction */
void gpio_dir(gpio_t *obj, PinDirection direction);

/* Set the output value */
void gpio_write(gpio_t *obj, int value);

/* Read the input value */
int gpio_read(gpio_t *obj);

/* Init the input pin and set mode to PullDefault */
void gpio_init_in(gpio_t* gpio, PinName pin);

/* Init the input pin and set the mode */
void gpio_init_in_ex(gpio_t* gpio, PinName pin, PinMode mode);

/* Init the output pin as an output, with predefined output value 0 */
void gpio_init_out(gpio_t* gpio, PinName pin);

/* Init the pin as an output and set the output value */
void gpio_init_out_ex(gpio_t* gpio, PinName pin, int value);

/* Init the pin to be in/out */
void gpio_init_inout(gpio_t* gpio, PinName pin, PinDirection direction, PinMode mode, int value);
```
这里有几个问题需要注意：

* 具体引脚定义在PinNames.h头文件中
* NC = 0xFFFFFFFF （Not Connected)
* PinMode设定引脚的工作模式，可以是PullUp（上拉）, PullDown（下拉）, PullNone（无）, OpenDrain（开漏输出），PullDefault等状态 
* PinDirection可以设定为PIN_INPUT或PIN_OUTPUT

# GPIO示例代码
实际上，mbed API是对mbed HAL API的进一步封装。HAL API这一层是“MCU无关层”的最底层，它的接口定义对于不同的开发板是一样的，但是其HAL API的实现是"MCU相关层"。为了和week2中的闪烁程序进行比较，调用上面提供的API编写同样功能的程序。
``` 
int main() {
    gpio_t obj;
    gpio_init(&obj,LED1);
    /* 或者使用
        gpio_init_out_ex(&obj,LED1,1);
    	gpio_init_out(&obj,LED1);
    */
    if(LED1!=NC) {
        gpio_write(&obj,0);//赋初值
        gpio_dir(&obj,PIN_OUTPUT);//设为输出
        gpio_mode(&obj,PullNone);//工作模式
    }
    //闪烁
    while(1) {
        gpio_write(&obj,1);
		wait(0.5);
        gpio_write(&obj,0);
		wait(0.5);
    }
} 
```
编译下载后实现闪烁
![](https://github.com/G1mcrack/link/raw/master/week3/2.jpg)
![](https://github.com/G1mcrack/link/raw/master/week3/1.jpg)