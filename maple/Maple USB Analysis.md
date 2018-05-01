# Maple USB Driver Analysis

## Overview

Maple's USB driver depends heavily on the STM32 Standard Peripheral Library and implements a Virtual COM as a USB CDC ACM device on the STM32F103 board.

Currently, no other boards are supported because of the "historical" issues.

## STM32 USB Standard Peripheral Library

### USB core

To set up a USB device, we must define the number of `Endpoints` and configuration types. This is specified in the USB standard and we have to obey it.

In STM32, a USB device is defined as:

```C
typedef struct _DEVICE {
    uint8_t Total_Endpoint;
    uint8_t Total_Configuration;
} DEVICE;
```

Description of the structure fields:
* `Total_Endpoint` is the number of endpoints the USB application uses.
* `Total_Configuration` is the number of configurations the USB application has.

The core keeps device level information in the `Device_Table` structure. `Device_Table` is of the type: `DEVICE`.

Maple has 4 endpoints and only 1 configuration type. Endpoint 1 is used as TX line and Endpoint 3 is the RX line.

```C
#define NUM_ENDPTS                0x04
__weak DEVICE Device_Table = {
    .Total_Endpoint      = NUM_ENDPTS,
    .Total_Configuration = 1
};
```

### Device property structure

The USBcore dispatches the control to the user program whenever it is necessary. User handling procedures are given in an array of `Device_Property`. The structure has the type: `DEVICE_PROP`.

```C
typedef struct _DEVICE_PROP {
    void (*Init)(void);
    void (*Reset)(void);
    void (*Process_Status_IN)(void);
    void (*Process_Status_OUT)(void);
    RESULT (*Class_Data_Setup)(uint8_t RequestNo);
    RESULT (*Class_NoData_Setup)(uint8_t RequestNo);
    RESULT (*Class_Get_Interface_Setting)(uint8_t Interface, uint8_t AlternateSetting);
    uint8_t* (*GetDeviceDescriptor)(uint16_t Length);
    uint8_t* (*GetConfigDescriptor)(uint16_t Length);
    uint8_t* (*GetStringDescriptor)(uint16_t Length);
    void* RxEP_buffer; /* This field is not used in current library version.
    It is kept only for compatibility with previous versions */
    uint8_t MaxPacketSize;
} DEVICE_PROP;
```

In Maple, most of the callback functions are used.

```C
#define MAX_PACKET_SIZE            0x40  /* 64B, maximum for USB FS Devices */
__weak DEVICE_PROP Device_Property = {
    .Init                        = usbInit,
    .Reset                       = usbReset,
    .Process_Status_IN           = NOP_Process,
    .Process_Status_OUT          = NOP_Process,
    .Class_Data_Setup            = usbDataSetup,
    .Class_NoData_Setup          = usbNoDataSetup,
    .Class_Get_Interface_Setting = usbGetInterfaceSetting,
    .GetDeviceDescriptor         = usbGetDeviceDescriptor,
    .GetConfigDescriptor         = usbGetConfigDescriptor,
    .GetStringDescriptor         = usbGetStringDescriptor,
    .RxEP_buffer                 = NULL,
    .MaxPacketSize               = MAX_PACKET_SIZE
};
```

The device property structure fields are described below:
* ```void Init(void)```: Init procedure of the USB-FS_Device peripheral. It is called once at the start of the application to manage the initialization process.
* ```void Reset(void)```: Reset procedure of the USB peripheral. It is called when the macrocell receives a RESET signal from the bus. The user program should set up the endpoints in this procedure, in order to set the default control endpoint and enable it to
receive.
* ```void Process_Status_IN(void)```: Callback procedure, it is called when a status in a stage is finished. The user program can take control with this callback to perform class- and application-related processes. **Not Used in Maple**
* ```void Process_Status_OUT(void)```: Callback procedure, it is called when a status out stage is finished. As with Process_Status_IN, the user program can perform actions after a status out stage. **Not Used in Maple**
* ```RESULT (see note below) *(Class_Data_Setup)(uint8_t RequestNo)```: Callback procedure, it is called when a class request is recognized and this request needs a data stage. The core cannot process such requests. In this case, the user program gets the chance to use custom procedures to analyze the request, prepare the data and pass the data to the USB-FS_Device core for exchange with the host. The parameter RequestNo indicates the request number. The return parameter of this function has the type: RESULT. It indicates the result of the request processing to the core.
* ```RESULT (*Class_NoData_Setup)(uint8_t RequestNo)```: Callback procedure, it is called when a non-standard device request is recognized, that does not need a data stage. The core cannot process such requests. The user program can have the chance to use custom procedures to analyze the request and take action. The return parameter of this function has type: RESULT. It indicates the result of the request processing to the core.
* ```RESULT (*Class_GET_Interface_Setting)(uint8_t Interface, uint8_t AlternateSetting)```: This routine is used to test the received set interface standard request. The user must verify the "Interface" and "AlternateSetting" according to their own implementation and return the USB_UNSUPPORT in case of error in these two fields.
* ```uint8_t* GetDeviceDescriptor(uint16_t Length)```: The core gets the device descriptor.
* ```uint8_t* GetConfigDescriptor(uint16_t Length)```: The core gets the configuration descriptor.
* ```uint8_t* GetStringDescriptor(uint16_t Length)```: The core gets the string descriptor.
* ```uint16_t MaxPacketSize```: The maximum packet size of the device default control endpoint.

RESULT type:

```C
typedef enum _RESULT { 
    USB_SUCCESS = 0,/* request process sucessfully */
    USB_ERROR, /* error
    USB_UNSUPPORT, /* request not supported
    USB_NOT_READY/* The request process has not been finished,*/
    /* endpoint will be NAK to further requests*/
} RESULT;
```

### User standard request structure

The User Standard Request Structure is the interface between the user code and the management of the standard request. The structure has the type: `USER_STANDARD_REQUESTS`:
```C
typedef struct _USER_STANDARD_REQUESTS {
    void(*User_GetConfiguration)(void);
    void(*User_SetConfiguration)(void);
    void(*User_GetInterface)(void);
    void(*User_SetInterface)(void);
    void(*User_GetStatus)(void);
    void(*User_ClearFeature)(void);
    void(*User_SetEndPointFeature)(void);
    void(*User_SetDeviceFeature)(void);
    void(*User_SetDeviceAddress)(void);
} USER_STANDARD_REQUESTS;
```

If the user wants to implement specific code after receiving a standard USB Device request he has to use the corresponding functions in this structure.

The Maple only used a small part of the standard requests.

```C
__weak USER_STANDARD_REQUESTS User_Standard_Requests = {
    .User_GetConfiguration   = NOP_Process,
    .User_SetConfiguration   = usbSetConfiguration,
    .User_GetInterface       = NOP_Process,
    .User_SetInterface       = NOP_Process,
    .User_GetStatus          = NOP_Process,
    .User_ClearFeature       = NOP_Process,
    .User_SetEndPointFeature = NOP_Process,
    .User_SetDeviceFeature   = NOP_Process,
    .User_SetDeviceAddress   = usbSetDeviceAddress
};
```

I will only explain the used function here:

* ```void (*User_GetConfiguration)(void)```: Called after receiving the Get Configuration Standard request.
* ```void (*User_SetConfiguration)(void)```: Called after receiving the Set Configuration Standard request.
* ```void (*User_SetDeviceAddress)(void)```: Called after receiving the set Address Standard request.

## Maple USB CDC ACM Module

### Interfaces

```C
void usb_cdcacm_enable(gpio_dev *disc_dev, uint8 disc_bit)
```

This function initialize the CDC ACM and do the low level USB initializations.

This function delegates most of its work to `usb_init_usblib` function, which is similar to the `USB_Init` funciton in the Standard Peripheral Library.

```C
void usb_cdcacm_disable(gpio_dev *disc_dev, uint8 disc_bit)
```

This function disables the USB interruption and send proper bits to the host according to the USB standard.

```C
void usb_cdcacm_putc(char ch)
```

A simple warp of TX function.

```C
uint32 usb_cdcacm_tx(const uint8* buf, uint32 len)
```

The non-blocking transmit function. It checks the status of the USB port first (busy or not), copy the data to the PMA buffer and then set up Endpoint Registers to let the low leve l driver do their work.

```C
uint32 usb_cdcacm_data_available(void)

uint8 usb_cdcacm_is_transmitting(void)

uint16 usb_cdcacm_get_pending(void)
```

Status functions.

```C
uint32 usb_cdcacm_rx(uint8* buf, uint32 len)
```

Non-blocking RX function. It copies the data into its internal buffer and mark the bytes as read.

```C
uint32 usb_cdcacm_peek(uint8* buf, uint32 len)
```

Nonblocking byte lookahead. Looks at unread bytes without marking them as read.

```C
uint8 usb_cdcacm_get_dtr()
uint8 usb_cdcacm_get_rts()
```

Get DTR and RTS signal, used in `NoData` interaction mode.

```C
void usb_cdcacm_get_line_coding(usb_cdcacm_line_coding *ret)

int usb_cdcacm_get_baud(void)

int usb_cdcacm_get_stop_bits(void)

int usb_cdcacm_get_parity(void)

int usb_cdcacm_get_n_data_bits(void)
```

Property functions.

### Callback functions

```C
static void vcomDataTxCb(void)
```

Callback when TX finishes. Mark the unsent bytes as zero and unset the transmisstting flag.

```C
static void vcomDataRxCb(void)
```

RX callback. Copy the data from PMA to buffer and set the unread bytes count.

```C
static uint8* vcomGetSetLineCoding(uint16 length)
```

COM property set and get function.

```C
static void usbInit(void)
```

Initialize the USB device. Set the IRQ mask, CNTR & ISTR registers and clear the port state.

```C
static void usbReset(void)
```

Reset all the USB related flags. Setup control endpoint 0, management endpoint 1, RX and TX endpoints. Clear the transmitting flag, unread and unsent counter.

```C
static RESULT usbDataSetup(uint8 request)
```

This is a callback in the Device Property structure. It preprocesses the data interaction between the host and the client.

```C
static RESULT usbNoDataSetup(uint8 request)
```

This is a callback to handle the NoData interactions. In Maple, it is used to handle the RTS and CTR signal.

```C
static RESULT usbGetInterfaceSetting(uint8 interface, uint8 alt_setting)

static uint8* usbGetDeviceDescriptor(uint16 length)

static uint8* usbGetConfigDescriptor(uint16 length)

static uint8* usbGetStringDescriptor(uint16 length)

static void usbSetConfiguration(void) 

static void usbSetDeviceAddress(void)
```

Property functions.


