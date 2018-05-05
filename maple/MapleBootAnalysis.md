# maple-bootloader 分析

## 目录结构
### flash目录
包含了JTAG烧录flash和调试的配置文件，以及用于串口烧录bootloader的python脚本
### stm32\_lib目录
包含了链接脚本，startup汇编代码和m3的一些汇编指令宏定义封装
### usb\_lib目录
包含了USB的驱动代码
### hardware.c
系统时钟，中断和清理函数，不包括USB和EEPROM的读写函数
### usb.c
使用USB库代码对USB设备进行配置：包括USB的初始化，中断设置，设备时钟设置等，其中的中断处理已经耦合了对DFU函数的调用
### dfu.c
Device Firmware Upgrage的简称，USBDFU是一个通过USB接口与上位机通信的协议，这里实现了这个协议的Device端，构建了一个状态机用于接收并烧写用户代码。支持向FLASH和RAM写入。上位机使用[dfu-util](http://dfu-util.sourceforge.net)（基于USBDFU协议的开源上位机端工具）
>dfu-util is a host side implementation of the DFU 1.0 and DFU 1.1 specifications of the USB forum. DFU is intended to download and upload firmware to/from devices connected over USB.

### main.c
Bootloader主要逻辑代码，包括上电后的初始化、判断、等待以及用户代码入口

### conf.h
包含了相关配置，包括：

1. LED灯的GPIO端口（用于指示Bootloader的工作状态）
2. 按钮的GPIO端口
3. 用户代码的开始地址（包括FLASH中和RAM中）
4. USB设备识别号

>USB vendor IDs (VID) and product IDs (PID) are 16-bit numbers used to identify USB devices to a computer or other host. Each vendor ID is assigned by the USB Implementers Forum to a specific company, which in turn assign a PID to individual products.

## 代码逻辑分析
从目录结构中可以看出，该Bootloader分为两个部分运行，一个是DFU协议的状态机，由USB的中断驱动，通过上位机运行dfu-util工具与其进行通信；另一部分是main函数的判断和等待

main函数结构十分清晰：

```
int main() {
    systemReset(); // peripherals but not PC
    setupCLK();
    setupLED();
    setupUSB();
    setupBUTTON();
    setupFLASH();

    strobePin(LED_BANK, LED, STARTUP_BLINKS, BLINK_FAST);

    /* wait for host to upload program or halt bootloader */
    bool no_user_jump = !checkUserCode(USER_CODE_FLASH) && !checkUserCode(USER_CODE_RAM) || readPin(BUTTON_BANK, BUTTON);
    int delay_count = 0;

    while ((delay_count++ < BOOTLOADER_WAIT)
            || no_user_jump) {

        strobePin(LED_BANK, LED, 1, BLINK_SLOW);

        if (dfuUploadStarted()) {
            dfuFinishUpload(); // systemHardReset from DFU once done
        }
    }

    if (checkUserCode(USER_CODE_RAM)) {
        jumpToUser(USER_CODE_RAM);
    } else if (checkUserCode(USER_CODE_FLASH)) {
        jumpToUser(USER_CODE_FLASH);
    } else {
        // some sort of fault occurred, hard reset
        strobePin(LED_BANK, LED, 5, BLINK_FAST);
        systemHardReset();
    }

}
```
首先调用初始化函数，然后进行一次判断：

* 如果存在用户代码并且按钮没有被按下，则直接执行用户代码
* 若存在用户代码并且按钮被按下或者不存在用户代码，则进入主循环，等待2秒后退出等待
* 2秒内如果有烧写开始，则等待烧写完成后系统复位。

退出等待后，若既不存在用户代码，也没有在两秒内进行烧写，则进行系统复位。

与Bootloader配套的是leaflabs的libmaple，从libmaple的makefile里我找到了它的下载方式：

```
UPLOAD_ram   := $(SUPPORT_PATH)/scripts/reset.py && \
                sleep 1                  && \
                $(DFU) -a0 -d $(BOARD_USB_VENDOR_ID):$(BOARD_USB_PRODUCT_ID) -D $(BUILD_PATH)/$(BOARD).bin -R
UPLOAD_flash := $(SUPPORT_PATH)/scripts/reset.py && \
                sleep 1                  && \
                $(DFU) -a1 -d $(BOARD_USB_VENDOR_ID):$(BOARD_USB_PRODUCT_ID) -D $(BUILD_PATH)/$(BOARD).bin -R
```
经过查找，`DFU`即是`dfu-util`，因此到了这里我们已经可以看出，这个Bootloader的具体逻辑如下：

1. 首先将该Bootloader通过其他方式烧录进FLASH（flash文件夹中包括了JTAG方式和串口方式）
2. 单片机上电后，Bootloader启动，初始化
3. 判断用户代码是否已存在，判断按钮是否被按下（这里的按钮是指maple板子特有的一个按钮）
4. 根据条件（在main.c中），在不存在用户代码或按钮被按下时进入2秒的等待
5. 若在2秒内接收到上位机通过dfu-util发送的请求，则开始代码下载过程
6. 下载完毕后进行系统复位，重新启动


>Maple-bootloader在6年前停止维护，通过ISSUE我找到了一个第三方保持维护的基于maple-bootloader的stm32F103的bootloader：[STM32duino-bootloader](https://github.com/rogerclarkmelbourne/STM32duino-bootloader)
>
>这个Bootloader适配更多种类的STM32F103的板子，并且缩小了Bootloader的大小