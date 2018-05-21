## Maple usb analysis

The objectives to use the USB peripheral in maple:

+ To receive sketch/program uploads from the IDE
+ To emulate a regular serial port for use as a terminal(text read/write) - SerialUSB

We can see the code at https://github.com/leaflabs/maple-bootloader.



### Initialization

In the `main()` function we can find the function `setupUSB()` which can set up the USB interfaces in **usb.c**.

```c
int main() {
    systemReset(); // peripherals but not PC
    setupCLK();
    setupLED();
    setupUSB();    // This is what we focus on
    setupBUTTON();
    setupFLASH();
    ...
}
```

Keep track on this function we can find the content of `setupUSB()`. 

It first pulls up some Pin of the USB registers(DISC and GPIOC).

```c
void setupUSB(void) {
    u32 rwmVal; /* read-write-modify place holder var */

    /* Setup the USB DISC Pin */
    rwmVal  = GET_REG(RCC_APB2ENR);
    rwmVal |= 0x00000010;
    SET_REG(RCC_APB2ENR, rwmVal);

    // todo, macroize usb_disc pin
    /* Setup GPIOC Pin 12 as OD out */
    rwmVal  = GET_REG(GPIO_CRH(GPIOC));
    rwmVal &= 0xFFF0FFFF;
    rwmVal |= 0x00050000;
    setPin(GPIOC, 12);
    SET_REG(GPIO_CRH(GPIOC), rwmVal);

    pRCC->APB1ENR |= 0x00800000;

    /* initialize the usb application */
    resetPin(GPIOC, 12);  /* present ourselves to the host */
    usbAppInit();
}
```

We can see the relations between some important init functions: `setupUSB()` -> `usbAppInit()` -> `USB_init()` -> `usbInit()`.  

Note that `usb_init()` is in the `Device_Property`, which integrates important function calls (via pointers) in a struct `DEVICE_PROP`.

```c
DEVICE_PROP Device_Property = {
    usbInit,
    usbReset,
    usbStatusIn,
    usbStatusOut,
    ...
};
```

Here is the function `usbInit()` which we will analyze later.

```c
void usbInit(void) {
    dfuInit();		/* 1. Init DFU */

    pInformation->Current_Configuration = 0;
    usbPowerOn();	/* 2. turn on the power */

    _SetISTR(0);
    wInterrupt_Mask = ISR_MSK;
    _SetCNTR(wInterrupt_Mask);  /* 3. Set the interrupt masks */

    usbEnbISR(); /* configure the cortex M3 private peripheral NVIC */
    bDeviceState = UNCONNECTED;
}
```



#### 1. DFU

DFU stands for **Device Firmware Upgrade**. In short, it is used for upgrading and fixing bugs.

> *Device Firmware Upgrade* (DFU) is a vendor- and device-independent mechanism for upgrading the [firmware](https://en.wikipedia.org/wiki/Firmware) of USB devices with improved versions provided by their manufacturers, offering (for example) a way to deploy firmware bug fixes. During the firmware upgrade operation, USB devices change their operating mode effectively becoming a [PROM](https://en.wikipedia.org/wiki/Programmable_read-only_memory) programmer. Any class of USB device can implement this capability by following the official DFU specifications.
>
> In addition to its intended legitimate purposes, DFU can also be exploited by uploading maliciously crafted firmware that causes USB devices to spoof various other device types; one such exploiting approach is known as [BadUSB](https://en.wikipedia.org/wiki/BadUSB).
>
> -- From Wikipedia



#### 2. usbPowerOn()

It sets the registers: **CNTR and ISTR**.  Actually CNTR and ISTR are just macros of some pointers which points to specific registers. It seems that ISTR will be set to 0 only when some interrupt masks values will be set to CNTR.

> CNTR = Control Register
>
> ISTR = Interrupt Status Register

**CNTR** is set for 3 times here: **CNTR_FRES**, 0 and **CNTR_RESETM | CNTR_SUSPM | CNTR_WKUPM**.

> FRES = Force USB RESet
>
> RESETM = RESET Mask
>
> SUSPM = SUSPend Mask
>
> WKUPM = WaKe UP Mask

```c
RESULT usbPowerOn(void) {
    u16 wRegVal;

    wRegVal = CNTR_FRES;
    _SetCNTR(wRegVal);  		/* SetCNTR */

    wInterrupt_Mask = 0;
    _SetCNTR(wInterrupt_Mask); /* SetCNTR */
    _SetISTR(0); 				/* SetISTR */
    wInterrupt_Mask = CNTR_RESETM | CNTR_SUSPM | CNTR_WKUPM; /* the bare minimum */
    _SetCNTR(wInterrupt_Mask); 	/* SetCNTR */

    return USB_SUCCESS;
}
```



#### 3. Set interrupt masks 

The interrupt mask `ISR_MSK` is related to these masks:

> CNTR_CTRM = Correct TRansfer Mask
>
> CNTR_WKUPM = WaKe UP Mask
>
> CNTR_SUSPM = SUSPend Mask
>
> CNTR_ERRM = ERRor Mask
>
> CNTR_SOFM = Start Of Frame Mask
>
> CNTR_ESOFM = Expected Start Of Frame Mask
>
> CNTR_RESETM



#### 4. usbEnbISR, enable ISRs 

It configures the cortex M3 private peripheral NVIC, and enables the ISR of USB. Recall that NVIC is something in CMSIS related to exceptions and interrupts.

```c
void usbEnbISR(void) {
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_IRQ;  /* LP = Low Priority */ 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = TRUE;
    nvicInit(&NVIC_InitStructure);
}
```



### Reset

We can find the `usbReset()` function in **usb.c**.

#### 1. DFU reset

#### 2. 

### Interrupts

Interrupt entrances, stored in arrays `pEpInt_IN` and `pEpInt_OUT`.

```c
void (*pEpInt_IN[7])(void) = {
    nothingProc,
    nothingProc,
    nothingProc,
    nothingProc,
    nothingProc,
    nothingProc,
    nothingProc,
};

void (*pEpInt_OUT[7])(void) = {
    nothingProc,
    nothingProc,
    nothingProc,
    nothingProc,
    nothingProc,
    nothingProc,
    nothingProc,
};
```



### Some questions

1. What does `do ... while(0)` mean?

```c
#define SET_REG(addr,val) do { *(vu32*)(addr)=val; } while(0)   // HERE
#define GET_REG(addr)     (*(vu32*)(addr))
```

2. Something interesting is that when we focus on `APB1ENR` and `RCC_APB2ENR` , it uses different ways to set the value. Why it(the error describe in the comments) happens?

```c
    rwmVal  = GET_REG(RCC_APB2ENR);
    rwmVal |= 0x00000010;
    SET_REG(RCC_APB2ENR, rwmVal);
    ...
    pRCC->APB1ENR |= 0x00800000;   // No "SET_REG"
```

​	In **hardware.h**:

```c
/* todo: there must be some major misunderstanding in how we access
   regs. The direct access approach (GET_REG) causes the usb init to
   fail upon trying to activate RCC_APB1 |= 0x00800000. However, using
   the struct approach from ST, it works fine...temporarily switching
   to that approach */
typedef struct {
    vu32 CR;
    vu32 CFGR;
    vu32 CIR;
    vu32 APB2RSTR;
    vu32 APB1RSTR;
    vu32 AHBENR;
    vu32 APB2ENR;
    vu32 APB1ENR;
    vu32 BDCR;
    vu32 CSR;
} RCC_RegStruct;
#define pRCC ((RCC_RegStruct *) RCC)
```

3. In function: `usbPowerOn()`, can I just use: `_SetCNTR(0)` and `_SetCNTR(CNTR_RESETM | CNTR_SUSPM | CNTR_WKUPM)`?



References:

+ http://docs.leaflabs.com/static.leaflabs.com/pub/leaflabs/maple-docs/latest/usb.html