Tutorial
====
Prerequisite
----
You are expected to get familiar with these following issues before you head on
* **Construct Physical Connection** - how to connect your CP2102 to your STM32F103C8T6 board. [link](https://github.com/zucsebd2018/CortexM-Tools/blob/master/Issue_5/%E5%91%BD%E4%BB%A4%E8%A1%8C%E7%9A%84%E4%B8%B2%E5%8F%A3%E4%B8%8B%E8%BD%BD%E5%B7%A5%E5%85%B7-MacOS.md#%E4%B8%B2%E5%8F%A3%E8%BF%9E%E6%8E%A5%E6%96%B9%E5%BC%8F)
* **Compile using the GNU Embedded Toolchain for Arm** - where to download, how to install and use it to compile an C/C++ project for Arm. [link](https://github.com/zucsebd2018/CortexM-Tools/blob/master/Issues_1/%E5%91%BD%E4%BB%A4%E8%A1%8C%E7%9A%84C%E8%AF%AD%E8%A8%80%E4%BA%A4%E5%8F%89%E7%BC%96%E8%AF%91%E5%99%A8-Windows.md)
* **Flash your STM32F103C8T6 board** - where to find a handy tool and how to use it to send your .bin/.hex file to your board. [link](https://github.com/zucsebd2018/CortexM-Tools/blob/master/%E5%91%BD%E4%BB%A4%E8%A1%8C%E7%9A%84%E4%B8%B2%E5%8F%A3%E4%B8%8B%E8%BD%BD%E5%B7%A5%E5%85%B7-Windows.md#download-the-hex-program)

You can try to build the test code in Week 1 [issue](https://github.com/zucsebd2018/CortexM-Tools/tree/master/testcode), connect your two chips, and flash the binary file to your STM32 board to check whether you have learnt how to resolve these three problems.

Start with Mbed online compiler
----
***Mbed*** is a platform and operating system for devices based on 32-bit ARM Cortex-M microcontroller. Leave this wiki [site](https://en.wikipedia.org/wiki/Mbed) for your further learning.

It provides an online compiler for your to build programs. To start with, browse this [website](https://os.mbed.com/accounts/login/) first to sign up for free.
After logging in, you will see the home page like this.

<p><img src="https://github.com/SamuelGong/Embedded-System-Experiments/raw/master/Week%202/image/mbed1.png" width="80%"  /></p>

Click *Compiler* to greet with the online compiler.

<p><img src="https://github.com/SamuelGong/Embedded-System-Experiments/raw/master/Week%202/image/mbed2.png" width="80%"  /></p>

Build your LED blinking program
 ----
 First, new a program. You will be asked to select a target platform for the compiler. Search and select `NUCLEO-F103RB`. And select `Blink LED Hello World` as template.

 <p><img src="https://github.com/SamuelGong/Embedded-System-Experiments/raw/master/Week%202/image/mbed3.png" width="60%"  /></p>

Next, import the mbed-STM32F103C8T6 library into your project. Click *Import*, and then search ` mbed_STM32F103C8T6` for library. Select the expected result and then click `Import!` on the upper left corner.

 <p><img src="https://github.com/SamuelGong/Embedded-System-Experiments/raw/master/Week%202/image/mbed4.png" width="80%"  /></p>

Finally add this statement to your `main.cpp`.
```
#include "stm32f103c8t6.h"
```

Make sure that it is the first statement, i.e. putting it ahead of the statement
```
#include "mbed.h"
```

All of the efforts end up like this.

 <p><img src="https://github.com/SamuelGong/Embedded-System-Experiments/raw/master/Week%202/image/mbed5.png" width="80%"  /></p>

Analysis of the automatically generated code
----
As you can see, all of the code line has its comment except for the following one
```
DigitalOut myled(LED1);
```
According to official documentation, the DigitalOut interface is used to configure and control a digital output pin. Here we just set the LED1 as output pin.

Compile program and run it
----
In fact, we can use *Compile* directly to get a .bin file. However, in this issue, forget about it - you have to build it offline.

First, right click the program directory on the left panel and select *Export Program...* . After that, you will meet a dialog like this

<p><img src="https://github.com/SamuelGong/Embedded-System-Experiments/raw/master/Week%202/image/mbed6.png" width="60%"  /></p>

Before exporting, remember to select `GCC (ARM Embedded)` as *Export Toolchain*.

After unzipping the download folder, build it.

<p><img src="https://github.com/SamuelGong/Embedded-System-Experiments/raw/master/Week%202/image/make.png" width="80%"  /></p>

After you properly connect your device and your chips, flash it.

<p><img src="https://github.com/SamuelGong/Embedded-System-Experiments/raw/master/Week%202/image/flash.png" width="80%"  /></p>

*Pay attention to the setting of the jumper in STM32 board.*

<p><img src="https://github.com/SamuelGong/Embedded-System-Experiments/raw/master/Week%202/image/blink.jpg" width="40%"  /></p>

Congratulations if you end up seeing the blinking LED light in your STM32 board as below.

Acknowledgement
----
Owing to documentation by [BForever](https://github.com/BForever), [shihaoL](https://github.com/shihaoL) and [KozakaiAya](https://github.com/KozakaiAya), I easily resolve this issue from scratch.
