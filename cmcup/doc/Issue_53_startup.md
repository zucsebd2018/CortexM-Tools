### Issue 53 Startup

实验过程

1. 检查system.c文件，可以看到相关的宏的定义。这里说明如果是定义了**STM32F10X_MD_VL**（也就是F100系列的Medium Density类型）就会把频率设置成24MHz，否则就是72MHz。这个定义只要在Makefile里改写就好。

```c
#if defined (STM32F10X_LD_VL) || (defined STM32F10X_MD_VL) || (defined STM32F10X_HD_VL)
/* #define SYSCLK_FREQ_HSE    HSE_VALUE */
 #define SYSCLK_FREQ_24MHz  24000000
#else
/* #define SYSCLK_FREQ_HSE    HSE_VALUE */
/* #define SYSCLK_FREQ_24MHz  24000000 */ 
/* #define SYSCLK_FREQ_36MHz  36000000 */
/* #define SYSCLK_FREQ_48MHz  48000000 */
/* #define SYSCLK_FREQ_56MHz  56000000 */
#define SYSCLK_FREQ_72MHz  72000000
#endif
```

2. 对makefile进行修改：

```makefile
MACRO = -DSTM32F10X_MD_VL # For stm32f100xx 
# MACRO = -DSTM32F10X_MD  # For stm32f103c8
```

3. 经过老师的提醒之后，如果是要在100和103之间修改的话，也需要在.ld里对RAM的大小进行修改。

```c
 /* Highest address of the user mode stack */
 _estack = 0x20002000;    /* end of 8K RAM */
 /* _estack = 0x20005000; */    /* end of 20K RAM */
 
 MEMORY
 {
   FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 64K
   RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 8K
 /*  RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 20K */
   MEMORY_B1 (rx)  : ORIGIN = 0x60000000, LENGTH = 0K
 }
```