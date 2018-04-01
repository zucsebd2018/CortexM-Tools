Serial Port Implementation
===
Polling
---
Basically, there are four ways for a CPU to control I/O devices:
* **Polling** In this way, a CPU checks the status of devices again and again until they are available for it to use.
* **Interrupt** Using this way, after a CPU informs a device to work for it, it will start to perform other tasks until the device finishes the work and signal an interruption.
* **Direct Memory Access, DMA** In the previous case, CPU doesn't have to wait for I/O devices, though, it still has to direclty give I/O instructions and response of every interrupt after a unit of I/O work has been done. DMA can take charge of these trivialities.

* **Channel** A processor aimed at performing I/O tasks, sharing memory with CPU.

For a serial port, polling is used in Mbed-generated code. Tracing the source code can inform you of this.

First, in order to do some analysis on the source code, make sure that you have import library `mbed-src`.

Recall that last week, I introduced you that [APIs](https://github.com/zucsebd2018/CortexM-Tools/blob/master/%2330-mbed_API_Aanalysis/Serial%20Port/doc.md) for send / receive message on serial port consist of the following methods of **Class Serial**:
```C
int putc(int c)
int puts(const char *s)
int getc()
char *gets(char *s, int size)
int printf(const char* format, ...)
int scanf(const char* format, ...)
int vprintf(const char* format, std::va_list args)
int vscanf(const char* format, std::va_list args)
```
Tracing their call relationship, you will end up with these two functions declared in `mbed-src/api/Serial.h`.
```C
class Serial : public SerialBase, public Stream {

    // ...

protected:
    virtual int _getc();
    virtual int _putc(int c);
};
```
Whose definitions are specified in `mbed-src/common/Serial.cpp`.
```C
int Serial::_getc() {
    return _base_getc();
}

int Serial::_putc(int c) {
    return _base_putc(c);
}
```
Then what's in *_base_getc* and *_base_putc* ? These two methods are what Class Serial inherits from Class **SerialBase**. Here is part of the definition of the class. (Find them in `mbed-src/api/SerialBase.h`)
```C
class SerialBase {

    // ...

protected:
    int _base_getc();
    int _base_putc(int c);
};
```
And here are the definitions of them. (Find them in `mbed-src/common/SerialBase.cpp`)
```C
int SerialBase::_base_getc() {
    return serial_getc(&_serial);
}

int SerialBase::_base_putc(int c) {
    serial_putc(&_serial, c);
    return c;
}
```
Go on tracing, you will find that function `serial_getc()` and `serial_putc()` is declared in `mbed-src/hal/serial_api.h`.
```C
int  serial_getc       (serial_t *obj);
void serial_putc       (serial_t *obj, int c);
```

Till now, all the things are platform-independent. But the following part of your exploring starts to be  concerned with the specific platform. In this context, I mean **STM32F103C8T6**.

In `mbed-src/targets/hal/TARGET_STM/TARGET_NUCLEO_F103RB/serial_api.c`, you can read the implemetation of the above two functions written for STM32F103.
```C
int serial_getc(serial_t *obj)
{
    USART_TypeDef *usart = (USART_TypeDef *)(obj->uart);
    while (!serial_readable(obj));
    return (int)(USART_ReceiveData(usart));
}

void serial_putc(serial_t *obj, int c)
{
    USART_TypeDef *usart = (USART_TypeDef *)(obj->uart);
    while (!serial_writable(obj));
    USART_SendData(usart, (uint16_t)c);
}
```
Notice that the two obvious while loops.
Now you may agree with me that the I/O control method Mbed-generated code uses is **polling**.

More About Sending and Receiving
---
This part will tell you a little bit more things about the send / receive mechanism, for example, what do *USART_SendData()* and *USART_ReceiveData()* do, or what do *serial_readable()*
and *serial_writable()* checks in detail?

First let's focus on the definitions of *serial_readable()*
and *serial_writable()* in `mbed-src/targets/hal/TARGET_STM/TARGET_NUCLEO_F103RB/serial_api.c`:
```C
int serial_readable(serial_t *obj)
{
    int status;
    USART_TypeDef *usart = (USART_TypeDef *)(obj->uart);
    // Check if data is received
    status = ((USART_GetFlagStatus(usart, USART_FLAG_RXNE) != RESET) ? 1 : 0);
    return status;
}

int serial_writable(serial_t *obj)
{
    int status;
    USART_TypeDef *usart = (USART_TypeDef *)(obj->uart);
    // Check if data is transmitted
    status = ((USART_GetFlagStatus(usart, USART_FLAG_TXE) != RESET) ? 1 : 0);
    return status;
}
```

It is clear that these two methods will check two flag, *USART_FLAG_RXNE*, which literally means that the receive buffer is not empty, and USART_FLAG_TXE, which means that the transmit buffer is empty. 
These two flags are in fact embedded in a register called **SR**, which you can find that in file `mbed-src/targets/cmsis/TARGET_STM/TARGET_NUCLEO_F103RB/TOOLCHAIN_IAR/stm32f10x_usart.c`.
```C
FlagStatus USART_GetFlagStatus(USART_TypeDef* USARTx, uint16_t USART_FLAG)
{
    // ...
  
  if ((USARTx->SR & USART_FLAG) != (uint16_t)RESET)
  {
    bitstatus = SET;
  }
  else  
  {
    bitstatus = RESET;
  }
  return bitstatus;
}
```
In terms of type *USART_TypeDef*, you can find it in `mbed-src/targets/cmsis/TARGET_STM/TARGET_NUCLEO_F103RB/stm32f10x.h`.
```C
typedef struct
{
  __IO uint16_t SR;
  __IO uint16_t DR;
  
  // ...

} USART_TypeDef;
```
According to [official documentation](http://www.st.com/content/ccc/resource/technical/document/reference_manual/59/b9/ba/7f/11/af/43/d5/CD00171190.pdf/files/CD00171190.pdf/jcr:content/translations/en.CD00171190.pdf), *SR* stands for **Status Register**, with a look like this:

<p><img src="https://raw.githubusercontent.com/SamuelGong/Embedded-System-Experiments/master/Week%204/image/SR.png" width="80%"  /></p>

Now let's return to the main part of sending and receiving. In previous chapter, I told you that the program will trap in busy waiting before it can call *USART_ReceiveData()* or *USART_SendData()*, which is obviously polling. Let me here tell you what they look like.
```C
void USART_SendData(USART_TypeDef* USARTx, uint16_t Data)
{
  /* Check the parameters */
  // ...
    
  /* Transmit Data */
  USARTx->DR = (Data & (uint16_t)0x01FF);
}

uint16_t USART_ReceiveData(USART_TypeDef* USARTx)
{
  /* Check the parameters */
  // ...
  
  /* Receive Data */
  return (uint16_t)(USARTx->DR & (uint16_t)0x01FF);
}
```

As you can figure out, they all read from / write to **DR** data. In fact, *DR* is another register which contains the received or transmitted data character with the form like this.

<p><img src="https://raw.githubusercontent.com/SamuelGong/Embedded-System-Experiments/master/Week%204/image/DR.png" width="80%"  /></p>

To understand it better, you are recommended to read this block diagram for USART:

<p><img src="https://raw.githubusercontent.com/SamuelGong/Embedded-System-Experiments/master/Week%204/image/diagram.png" width="80%"  /></p>

Setting of UART port
---

If my comprehension is right, other setting of UART port refers to the setting of data bits, parity, stop bit, baud rate and something like that. If that is the case, I can demonstrate that it is almost about setting of registers in the lowest level. To be exact, it entails two control registers - **CR1** and **CR2**.

```C
typedef struct
{
  __IO uint16_t SR;
  uint16_t  RESERVED0;
  __IO uint16_t DR;
  uint16_t  RESERVED1;
  __IO uint16_t BRR;
  uint16_t  RESERVED2;
  __IO uint16_t CR1;
  uint16_t  RESERVED3;
  __IO uint16_t CR2;
  uint16_t  RESERVED4;
  __IO uint16_t CR3;
  uint16_t  RESERVED5;
  __IO uint16_t GTPR;
  uint16_t  RESERVED6;
} USART_TypeDef;
```

For *CR1*,

<p><img src="https://raw.githubusercontent.com/SamuelGong/Embedded-System-Experiments/master/Week%204/image/CR1.png" width="80%"  /></p>

Specification of some essential bits -
Bit | Name | Specification
:- | :- | :- 
12 | M | **Word length**: 0 for 1 Start bit, 8 Data bits, n Stop bit and 1 for 1 Start bit, 9 Data bits, n Stop bit.
10 | PCE | **Parity control enable**: 0 for disabled and 1 for enabled.
9 | PS | **Parity selection**: 0 for even parity and 1 for odd parity.

For *CR2*,

<p><img src="https://raw.githubusercontent.com/SamuelGong/Embedded-System-Experiments/master/Week%204/image/CR2.png" width="80%"  /></p>

Specification of some essential bits -
Bit | Name | Specification
:- | :- | :- 
13:12 | STOP | **STOP bits**: 00 for 1 Stop bit, 01 for 0.5 Stop bit, 10 for 2 stop bits, 11 for 1.5 stop bits.

More are available in the document.

