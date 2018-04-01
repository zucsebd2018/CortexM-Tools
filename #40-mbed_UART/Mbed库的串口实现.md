# Mbed库的串口实现
## Mbed库的结构
Mbed库的最新源代码位于<https://os.mbed.com/users/mbed_official/code/mbed-dev/>

库中分为5个目录和一个总的头文件，这5个目录分别是：
### cmsis
CMSIS全称Cortex Microcontroller Software Interface Standard，是ARM公司与多家不同的芯片和软件供应商一起紧密合作定义的，提供了内核与外设、实时操作系统和中间设备之间的通用接口。在Mbed库中主要是用于对CPU的操作和对编译过程的控制

### drivers
这里是所有设备驱动中与硬件无关的驱动部分，也是所有驱动的最顶层

### hal
HAL全称Hardware Abstraction Layer，即硬件抽象层，它为drivers中的驱动提供了统一的硬件抽象，为Mbed提供跨硬件平台支持，其中包含大量的规定接口的头文件和少量.c文件。

### platform
这是Mbed库中除了设备驱动以外的其他部分，提供了各类经常使用到的数据结构和机制，类似于C库

### targets
这里是所有目标平台的硬件相关代码

## Mbed库的串口代码
根据Mbed库的结构，我们可以知道，串口的代码会被分成三层分布于drivers,hal和targets中，以我们使用的STM32f103为例，经过查找，串口类定义和实现位于drivers中的`Serial.h`,`Serial.cpp`,`SerialBase.h`,`SerialBase.cpp`中，串口的硬件抽象层接口在hal中的`serial_api.h`中，STMF1系列的串口具体实现位于targets中的`TARGET_STM/serial_api.c`,`TARGET_STM/serial_api_hal.h`以及`TARGET_STM/TARGET_STM32F1/serial_device.c`中。而对串口硬件部分的操作全都在`TARGET_STM/TARGET_STM32F1/device/`目录中，包括UART和USART，这里是最底层的实现。

>在这些文件中，Mbed库使用了预编译宏控制了串口的同/异步和是否有流控,如果目标设备支持异步则串口会具备异步收发功能。但是异步时留下了DMA的配置空间却没有指明是否使用DMA，因此异步读写时采用单纯中断方式还是DMA方式留给了targets中的硬件相关部分代码决定。我发现STM32f103部分的串口实现在`TARGET_STM/TARGET_STM32F1/device/`中包括了轮询、中断和DMA三种方式的实现，但是在`TARGET_STM/TARGET_STM32F1/serial_device.c`还没有采用DMA方式，只采用了轮询和中断方式。

### drivers
这部分主要是定义了两个类，首先是SerialBase类（`SerialBase.h`,`SerialBase.cpp`），它包装了串口所需的所有函数，包括初始化设置，是否可读/写，设置中断（这里的中断指的是接受到数据或者数据发送完成的中断，而不是发送/接受过程中使用到的中断）和读写操作。

而Serial类（`Serial.h`,`Serial.cpp`）则是继承了SerialBase类和Stream类，将串口设备抽象成数据流进行操作，提供了更高级别的抽象。
### hal
`serial_api.h`这里面定义了一系列常数和枚举类型，并且给出了串口所有操作的接口
### targets
`TARGET_STM/serial_api_hal.h`名义上是hal中的接口的扩展，实际上并没有内容。

`TARGET_STM/serial_api.c`使用UART提供了具体的串口初始化和释放操作，实现了是否可读/写函数，以及波特率和数据格式的设置操作

`TARGET_STM/TARGET_STM32F1/serial_device.c`使用UART提供了具体的收发控制函数，我们要分析的其实主要是这个文件。

`TARGET_STM/TARGET_STM32F1/device`里是对硬件直接进行操作的最底层代码。

## Mbed库的串口同步使用方式
当将Serial类作为数据流进行输入输出时，串口会用阻塞方式进行读写。包括`getc`,`putc`,`puts`,`gets`,`scanf`,`printf`等等一系列从Stream类继承而来的函数。
## Mbed库的串口异步使用方式
为了完成异步操作，Mbed设计了`event_callback_t`类封装回调函数，这个类不仅可以将普通C函数作为回调函数，还可以将类的成员函数（包括静态成员函数）作为回调函数。

以下是`event_callback_t`的用法:

### FunctionPointerArg1< R, A1 > Class （即event\_callback\_t）
#### Public Member Functions

```
FunctionPointerArg1 (R(*function)(A1)=0)
 	创建一个函数指针指向静态函数
template<typename T > 
FunctionPointerArg1 (T *object, R(T::*member)(A1))
 	创建一个函数指针指向类成员函数 
void 	attach (R(*function)(A1))
 	绑定一个静态函数
template<typename T >
void 	attach (T *object, R(T::*member)(A1))
 	绑定一个类成员函数
R 	call (A1 a)
 	调用绑定的函数 
```
#### Data Fields
```
R(*)() 	get_function (A1)
 	获取绑定的函数
```
异步时，发送函数为`write`, 它需要一个缓冲区以及缓冲区的长度，一个侦听事件标志`event`（在`serial_api.h`中定义，用位操作指定哪些事件会导致回调函数，默认为传输完成事件），以及一个`event_callback_t`用于在完成传输或者发生传输错误时调用。当已经有传输在进行的时候，`write`函数直接返回错误码，而不会调用回调函数。

接受函数为`read`，参数与`write`类似，但是它有第五个参数`char_match`,当事件设置包含`SERIAL_EVENT_RX_CHARACTER_MATCH`标志的时候，如果接收到与匹配的字符，串口会停止接受数据并调用回调函数。
## 异步串口示例代码
```
#include "mbed.h"

/*------------ Constant definitions --------------*/
#define TX_PIN          USBTX
#define RX_PIN          USBRX
#define BRATE           115200
#define LED_PIN         LED0
#define TOGGLE_RATE     (0.5f)
#define BUFF_LENGTH     5

/*-------- Check if platform compatible ----------*/
#if DEVICE_SERIAL_ASYNCH
Serial test_connection(USBTX, USBRX);
#else
#error "Platform not compatible with Low Power APIs for Serial"
#endif

/*------------------ Variables -------------------*/
LowPowerTicker      blinker;
bool                blinking = false;
event_callback_t    serialEventCb;
DigitalOut          LED(LED_PIN);
uint8_t             rx_buf[BUFF_LENGTH + 1];

/*------------------ Callbacks -------------------*/
void blink(void) {
    LED = !LED;
}

/**
* This is a callback! Do not call frequency-dependent operations here.
*
* For a more thorough explanation, go here: 
* https://developer.mbed.org/teams/SiliconLabs/wiki/Using-the-improved-mbed-sleep-API#mixing-sleep-with-synchronous-code
**/
void serialCb(int events) {
    /* Something triggered the callback, either buffer is full or 'S' is received */
    unsigned char i;
    if(events & SERIAL_EVENT_RX_CHARACTER_MATCH) {
        //Received 'S', check for buffer length
        for(i = 0; i < BUFF_LENGTH; i++) {
            //Found the length!
            if(rx_buf[i] == 'S') break;
        }
        
        // Toggle blinking
        if(blinking) {
            blinker.detach();
            blinking = false;
        } else {
            blinker.attach(blink, TOGGLE_RATE);
            blinking = true;
        }
    } else if (events & SERIAL_EVENT_RX_COMPLETE) {
        i = BUFF_LENGTH - 1;
    } else {
        rx_buf[0] = 'E';
        rx_buf[1] = 'R';
        rx_buf[2] = 'R';
        rx_buf[3] = '!';
        rx_buf[4] = 0;
        i = 3;
    }
    
    // Echo string, no callback
    test_connection.write(rx_buf, i+1, 0, 0);
    
    // Reset serial reception
    test_connection.read(rx_buf, BUFF_LENGTH, serialEventCb, SERIAL_EVENT_RX_ALL, 'S');
}

/*-------------------- Main ----------------------*/
int main() {
    /* Very Simple Main (tm) */
    serialEventCb.attach(serialCb);
    
    /* Setup serial connection */
    test_connection.baud(BRATE);
    test_connection.printf("Low Power API test\n\nSend 'S' to toggle blinking\n");
    test_connection.read(rx_buf, BUFF_LENGTH, serialEventCb, SERIAL_EVENT_RX_ALL, 'S');
    
    /* Let the callbacks take care of everything */
    while(1) sleep();
}
```