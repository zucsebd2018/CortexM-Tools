# 命令行的串口下载工具-Ubuntu

## Prerequsites

### Ubuntu CP210x Driver
* Plug in the CP2102 USB to UART Bridge
* Run `lsmod |grep cp210x` and see whether the `cp210x` module is already loaded by Ubuntu
* If not, run `modprobe cp210x` in terminal to manually install the driver

### Clone and compile the `stm32isp` tool by Weng Kai
* The code can be found [Here](https://github.com/wengkai/stm32isp)
* Clone the source code by git and run `make` under the source directory
* If you want to use this tool system-wide, run `make install` instead

## Connect the CP2102 to STM32
![](img/STM32_Pin.png)

### Inter-Connection

* RXD -> TX (Pin A9)
* TXD -> RX (Pin A10)
* VCC -> 3.3V (Bottom Right one)
* GND -> GND (Upper Left one)    **Really Important!!!**

### Intra-Connection

* Boot0 Jumper -> 1
* Boot1 Jumper -> 0

## Download the .hex program

### Usage

#### General Usage

`stm32isp <.hex path> <USBtty> <baud rate>`
##### Example

stm32isp test.hex /dev/ttyUSB0 115200

Note: `<USBtty>` could vary on different systems, on my Ubuntu, it is `/dev/ttyUSB0`

#### Options

 <table>
   <tr>
     <td> -bin </td>
     <td> for uploading binary file </td>
   </tr>
   <tr>
     <td> -reset [-]DTR|RTS </td>
     <td> reset control line, default DTR </td>
   <tr>
     <td> -bootp [-]DTR|RTS </td>
     <td> bootp control line, default RTS </td>
   </tr>
   <tr>
     <td> -term <baud> </td>
     <td> for starting terminal after upload with buadrate, default 9600 </td>
   </tr>
   <tr>
     <td> -detectonly </td>
     <td> detect chip set only </td>
   </tr>
   <tr>
     <td> -termonly </td>
     <td> for starting terminal without an upload </td>
   </tr>
   <tr>
     <td> -noreset </td>
     <td> do not reset the board in termonly mode </td>
   </tr>
   <tr>
     <td> -echo </td>
     <td> local echo in term </td>
   </tr>
   <tr>
     <td> -line </td>
     <td> local line edit in term </td>
   </tr>
   <tr>
     <td> -verbose </td>
     <td> for more information </td>
   </tr>
   <tr>
     <td> -log <file name> </td>
     <td> for enabling logging of terminal output to <file name> </td>
   </tr>
 </table>

## Example Workflow

### Find the USB Terminal
![](img/02_ttyUSB0.png)

### Download the .hex file
![](img/04_download_exp.png)
