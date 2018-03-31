Exploration of Serial Port API of Mbed
====
> I am not going to replicate what the offical [documentation](https://os.mbed.com/handbook/Serial) has specified.

Class Serial
----
First let's recall what a serial port is. In computing, a serial port is a serial communication interface through which information transfers in and out one bit at a time (by [Wikipedia](https://en.wikipedia.org/wiki/Serial_port)). Using this kind of communication methods implemented by Mbed, we have successfully controlled a STM32 board, either making it to [flash an LED](https://github.com/zucsebd2018/CortexM-Tools/blob/master/%2321-mbed_Blink/doc.md) or asking it to [echo what a host device has sent to it](https://github.com/zucsebd2018/CortexM-Tools/blob/master/%2323-mbed_echo/mbed%E7%9A%84echo%E7%A8%8B%E5%BA%8F.md). You may notice that both of our success entail the use of class **Serial**, a class which aims to provide a way for your Mbed microcontroller to communicate with a host PC through a USB Virtual Serial Port.

Overview of Its Member Functions
----
```C
Serial s(?);
s.?(?);
```
Probably there are two problems confusing you -
* how to construct an object of this class, and
* what can I do after creating such an object.

For the first question, you may come up with its constructor. In fact, it is something like this
```C
Serial( PinName [transmit pin],
        PinName [receive pin],
        const char* name = NULL)
```
**name** for an Serial object is not a necessity, but the first two pin specifications are worth your attention. The so-called **transmit pin** is the one to which you link from the *RXD* port of your UART, while the **receive pin** is the one connected to the *TXD* port. Sets of pin name are different from one kind of microcontroller to another. For your STM32F103C8T6 board, refer to the header file *stm32f103c8t6.h* . It is recommended that you set the transmit pin to be *PA_9* and receive pin to be *PA_10*, i.e. 
```C
Serial s(PA_9, PA_10);
```
so that you can communicate with your board  over the same USB cable that is used for programming.

Once you obtain a Serial object, you can use it do something like

Things you may want to do | Functions you can use
:-:|:-:
set up transmission parameters | baud() format()
send to / receive from a host device a message | putc(), puts(), getc(), gets(), printf(), scanf(), vprintf(), vscanf()
judge the status of buffers | readable(), writable() / writeable()
process interrupt from a host device | attach()
others | tentatively no way

Let's delve into them.

---
```C
void baud(int [baudrate])
```
The default baudrate of a serial port is 9600, and you can change it using the above member function. Whatever baudrate you set, remember it when you try to flash your codes or run a terminal.

Bit rates commonly supported include 75, 110, 300, 1200, 2400, 4800, 9600, 19200, 38400, 57600 and 115200 bit/s. (Serial ports use two-level (binary) signaling, so the data rate in bits per second is equal to the symbol rate in baud.) Do not specify a random number ;) [More]

---
```C
void format(int [data bits] = 8,
            Parity [parity] = Serial::None,
            int [stop bits] = 1)
```
Set parameters other than baudrate, which are introduced as follows.
* The number of *data bits* in each character can be 5 (for Baudot code), 6 (rarely used), 7 (for true ASCII), 8 (for most kinds of data, as this size matches the size of a byte), or 9 (rarely used).
* When *parity* is used with a serial port, an extra data bit is sent with each data character, arranged so that the number of 1 bits in each character, including the parity bit, is always odd or always even. What you can use: *None*, *Odd*, *Even*, *Forced1*, *Forced2*, which is difined at *SerialBase.h*. (Class **Serial** is extended from Class **SerialBase**)
* *Stop bits* sent at the end of every character allow the receiving signal hardware to detect the end of a character and to resynchronise with the character stream.

---
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
All the prototypes you can find in *Stream.h* (Class **Serial** is also extended from Class **Stream**).

You may be familiar with these functions, because they are much like what they are in C standard library. What left to me to introduce is the direction - the direction which putc() is in is from your board to your host device, and what getc() in is the opposite one. The remainders are anolog to them.

---
```C
bool readable()
bool writable()
bool writeable()
```
When using *readable()*, you are trying to identify if a call to *getc()* or other functions with the same I/O direction is reasonable. In other words, if you want to know if there is a character from your host device available to read, use it.

As you can imagine, *writable()* and *writeable()* function in the opposite way.

---
```C
void attach(void(*)(void), [pointer of function],
            IrqType [interrupt type] = RxIrq)
void attach(T * [pointer of object],
            (void)(T::*)(void) [pointer of method],
            IrqType [interrupt type] = RxIrq)
```
When an interrupt of kind *interrupt type* (*RxIrq* for receive a message, *TxIrq* for transmit buffer is empty) is generated, the specified funtion will be called. The first prototype matches a call to a common function and the second one mathces a call to a member function.
In particular, the input message should be consumed when dealing with *RxIrq* interrupt, otherwise the interrupt will assert.

---

Comprehensive Use
---
The following piece of code tries to cover all the above functions.
```C
#include "stm32f103c8t6.h"
#include "mbed.h"

Serial s1(PA_9, PA_10);								// specify pinname
const char * prompt = "Ha\n";

void echo(void) {									// callback function
    if (s1.readable())							    // check if anything to read
        s1.putc(s1.getc());							// echo
}

void idle(void){
    s1.printf("%s", prompt);
}
 
int main() {
    char guest[100];
    s1.baud(115200);								// set baudrate
    s1.format(8, Serial::None, 1);					// set format
    s1.printf("Tell me your name\n");				// using printf
    s1.scanf("%s", guest);							// using scanf
    s1.printf("%s, have a good time.\n", guest);

    s1.attach(&echo);								// handling interrupt when receive a message
    s1.attach(&idle, Serial::TxIrq);				// handling interrupt when transmit buffer is empty
}
```
The outcome of running this piece of code looks like this

<p><img src="https://github.com/SamuelGong/Embedded-System-Experiments/raw/master/Week%203/image/1.png" width="60%"  /></p>
<p><img src="https://github.com/SamuelGong/Embedded-System-Experiments/raw/master/Week%203/image/2.png" width="60%"  /></p>
<p><img src="https://github.com/SamuelGong/Embedded-System-Experiments/raw/master/Week%203/image/3.png" width="60%"  /></p>
