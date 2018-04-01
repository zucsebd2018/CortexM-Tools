ARM GAS  C:\Users\Qian\AppData\Local\Temp\cc4EWhcg.s 			page 1


   1              		.cpu cortex-m3
   2              		.eabi_attribute 20, 1
   3              		.eabi_attribute 21, 1
   4              		.eabi_attribute 23, 3
   5              		.eabi_attribute 24, 1
   6              		.eabi_attribute 25, 1
   7              		.eabi_attribute 26, 1
   8              		.eabi_attribute 30, 1
   9              		.eabi_attribute 34, 1
  10              		.eabi_attribute 18, 4
  11              		.file	"DigitalOut.cpp"
  12              		.text
  13              	.Ltext0:
  14              		.cfi_sections	.debug_frame
  15              		.section	.text._ZN10DigitalOutC2EP12GPIO_TypeDeft,"ax",%progbits
  16              		.align	1
  17              		.global	_ZN10DigitalOutC2EP12GPIO_TypeDeft
  18              		.syntax unified
  19              		.thumb
  20              		.thumb_func
  21              		.fpu softvfp
  23              	_ZN10DigitalOutC2EP12GPIO_TypeDeft:
  24              		.fnstart
  25              	.LFB65:
  26              		.file 1 "Src/DigitalOut.cpp"
   1:Src/DigitalOut.cpp **** #include "DigitalOut.h"
   2:Src/DigitalOut.cpp **** 
   3:Src/DigitalOut.cpp **** DigitalOut::DigitalOut(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin):GPIOx(GPIOx), GPIO_Pin(GPIO_Pin)
  27              		.loc 1 3 0
  28              		.cfi_startproc
  29              		@ args = 0, pretend = 0, frame = 24
  30              		@ frame_needed = 0, uses_anonymous_args = 0
  31              	.LVL0:
  32 0000 30B5     		push	{r4, r5, lr}
  33              		.save {r4, r5, lr}
  34              	.LCFI0:
  35              		.cfi_def_cfa_offset 12
  36              		.cfi_offset 4, -12
  37              		.cfi_offset 5, -8
  38              		.cfi_offset 14, -4
  39              		.pad #28
  40 0002 87B0     		sub	sp, sp, #28
  41              	.LCFI1:
  42              		.cfi_def_cfa_offset 40
  43 0004 0446     		mov	r4, r0
  44 0006 0846     		mov	r0, r1
  45              	.LVL1:
  46 0008 1546     		mov	r5, r2
  47              	.LBB2:
  48              		.loc 1 3 0
  49 000a 0022     		movs	r2, #0
  50              	.LVL2:
  51 000c 2260     		str	r2, [r4]
  52 000e 6160     		str	r1, [r4, #4]
  53 0010 2581     		strh	r5, [r4, #8]	@ movhi
  54              	.LBB3:
  55              	.LBB4:
ARM GAS  C:\Users\Qian\AppData\Local\Temp\cc4EWhcg.s 			page 2


   4:Src/DigitalOut.cpp **** {
   5:Src/DigitalOut.cpp ****     //Init
   6:Src/DigitalOut.cpp ****     GPIO_InitTypeDef GPIO_InitStruct;
   7:Src/DigitalOut.cpp ****     __HAL_RCC_GPIOA_CLK_ENABLE();
  56              		.loc 1 7 0
  57 0012 0C4B     		ldr	r3, .L3
  58 0014 9969     		ldr	r1, [r3, #24]
  59              	.LVL3:
  60 0016 41F00401 		orr	r1, r1, #4
  61 001a 9961     		str	r1, [r3, #24]
  62 001c 9B69     		ldr	r3, [r3, #24]
  63 001e 03F00403 		and	r3, r3, #4
  64 0022 0193     		str	r3, [sp, #4]
  65 0024 019B     		ldr	r3, [sp, #4]
  66              	.LBE4:
   8:Src/DigitalOut.cpp ****     HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
  67              		.loc 1 8 0
  68 0026 2946     		mov	r1, r5
  69 0028 FFF7FEFF 		bl	HAL_GPIO_WritePin
  70              	.LVL4:
   9:Src/DigitalOut.cpp **** 
  10:Src/DigitalOut.cpp ****     GPIO_InitStruct.Pin = GPIO_Pin;
  71              		.loc 1 10 0
  72 002c 0295     		str	r5, [sp, #8]
  11:Src/DigitalOut.cpp ****     GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  73              		.loc 1 11 0
  74 002e 0123     		movs	r3, #1
  75 0030 0393     		str	r3, [sp, #12]
  12:Src/DigitalOut.cpp ****     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  76              		.loc 1 12 0
  77 0032 0223     		movs	r3, #2
  78 0034 0593     		str	r3, [sp, #20]
  13:Src/DigitalOut.cpp ****     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  79              		.loc 1 13 0
  80 0036 02A9     		add	r1, sp, #8
  81 0038 0348     		ldr	r0, .L3+4
  82 003a FFF7FEFF 		bl	HAL_GPIO_Init
  83              	.LVL5:
  84              	.LBE3:
  85              	.LBE2:
  14:Src/DigitalOut.cpp **** }
  86              		.loc 1 14 0
  87 003e 2046     		mov	r0, r4
  88 0040 07B0     		add	sp, sp, #28
  89              	.LCFI2:
  90              		.cfi_def_cfa_offset 12
  91              		@ sp needed
  92 0042 30BD     		pop	{r4, r5, pc}
  93              	.LVL6:
  94              	.L4:
  95              		.align	2
  96              	.L3:
  97 0044 00100240 		.word	1073876992
  98 0048 00080140 		.word	1073809408
  99              		.cfi_endproc
 100              	.LFE65:
 101              		.fnend
ARM GAS  C:\Users\Qian\AppData\Local\Temp\cc4EWhcg.s 			page 3


 103              		.global	_ZN10DigitalOutC1EP12GPIO_TypeDeft
 104              		.thumb_set _ZN10DigitalOutC1EP12GPIO_TypeDeft,_ZN10DigitalOutC2EP12GPIO_TypeDeft
 105              		.section	.text._ZN10DigitalOutD2Ev,"ax",%progbits
 106              		.align	1
 107              		.global	_ZN10DigitalOutD2Ev
 108              		.syntax unified
 109              		.thumb
 110              		.thumb_func
 111              		.fpu softvfp
 113              	_ZN10DigitalOutD2Ev:
 114              		.fnstart
 115              	.LFB68:
  15:Src/DigitalOut.cpp **** 
  16:Src/DigitalOut.cpp **** DigitalOut::~DigitalOut()
 116              		.loc 1 16 0
 117              		.cfi_startproc
 118              		@ args = 0, pretend = 0, frame = 0
 119              		@ frame_needed = 0, uses_anonymous_args = 0
 120              		@ link register save eliminated.
 121              	.LVL7:
  17:Src/DigitalOut.cpp **** {
  18:Src/DigitalOut.cpp **** 
  19:Src/DigitalOut.cpp **** }
 122              		.loc 1 19 0
 123 0000 7047     		bx	lr
 124              		.cfi_endproc
 125              	.LFE68:
 126              		.cantunwind
 127              		.fnend
 129              		.global	_ZN10DigitalOutD1Ev
 130              		.thumb_set _ZN10DigitalOutD1Ev,_ZN10DigitalOutD2Ev
 131              		.section	.text._ZN10DigitalOut5writeEi,"ax",%progbits
 132              		.align	1
 133              		.global	_ZN10DigitalOut5writeEi
 134              		.syntax unified
 135              		.thumb
 136              		.thumb_func
 137              		.fpu softvfp
 139              	_ZN10DigitalOut5writeEi:
 140              		.fnstart
 141              	.LFB70:
  20:Src/DigitalOut.cpp **** 
  21:Src/DigitalOut.cpp **** void DigitalOut::write(int value)
  22:Src/DigitalOut.cpp **** {
 142              		.loc 1 22 0
 143              		.cfi_startproc
 144              		@ args = 0, pretend = 0, frame = 0
 145              		@ frame_needed = 0, uses_anonymous_args = 0
 146              	.LVL8:
 147 0000 10B5     		push	{r4, lr}
 148              		.save {r4, lr}
 149              	.LCFI3:
 150              		.cfi_def_cfa_offset 8
 151              		.cfi_offset 4, -8
 152              		.cfi_offset 14, -4
 153 0002 0446     		mov	r4, r0
  23:Src/DigitalOut.cpp ****     if (value == 0)
ARM GAS  C:\Users\Qian\AppData\Local\Temp\cc4EWhcg.s 			page 4


 154              		.loc 1 23 0
 155 0004 11B1     		cbz	r1, .L10
  24:Src/DigitalOut.cpp ****     {
  25:Src/DigitalOut.cpp ****         HAL_GPIO_WritePin(this->GPIOx, this->GPIO_Pin, GPIO_PIN_RESET);
  26:Src/DigitalOut.cpp ****         this->state = 0;
  27:Src/DigitalOut.cpp ****     }
  28:Src/DigitalOut.cpp ****     else if (value == 1)
 156              		.loc 1 28 0
 157 0006 0129     		cmp	r1, #1
 158 0008 08D0     		beq	.L11
 159              	.LVL9:
 160              	.L6:
  29:Src/DigitalOut.cpp ****     {
  30:Src/DigitalOut.cpp ****         HAL_GPIO_WritePin(this->GPIOx, this->GPIO_Pin, GPIO_PIN_SET);
  31:Src/DigitalOut.cpp ****         this->state = 1;
  32:Src/DigitalOut.cpp ****     }
  33:Src/DigitalOut.cpp **** }
 161              		.loc 1 33 0
 162 000a 10BD     		pop	{r4, pc}
 163              	.LVL10:
 164              	.L10:
  25:Src/DigitalOut.cpp ****         this->state = 0;
 165              		.loc 1 25 0
 166 000c 0022     		movs	r2, #0
 167 000e 0189     		ldrh	r1, [r0, #8]
 168              	.LVL11:
 169 0010 4068     		ldr	r0, [r0, #4]
 170              	.LVL12:
 171 0012 FFF7FEFF 		bl	HAL_GPIO_WritePin
 172              	.LVL13:
  26:Src/DigitalOut.cpp ****     }
 173              		.loc 1 26 0
 174 0016 0023     		movs	r3, #0
 175 0018 2360     		str	r3, [r4]
 176 001a F6E7     		b	.L6
 177              	.LVL14:
 178              	.L11:
  30:Src/DigitalOut.cpp ****         this->state = 1;
 179              		.loc 1 30 0
 180 001c 0122     		movs	r2, #1
 181 001e 0189     		ldrh	r1, [r0, #8]
 182              	.LVL15:
 183 0020 4068     		ldr	r0, [r0, #4]
 184              	.LVL16:
 185 0022 FFF7FEFF 		bl	HAL_GPIO_WritePin
 186              	.LVL17:
  31:Src/DigitalOut.cpp ****     }
 187              		.loc 1 31 0
 188 0026 0123     		movs	r3, #1
 189 0028 2360     		str	r3, [r4]
 190              		.loc 1 33 0
 191 002a EEE7     		b	.L6
 192              		.cfi_endproc
 193              	.LFE70:
 194              		.fnend
 196              		.section	.text._ZN10DigitalOut4readEv,"ax",%progbits
 197              		.align	1
ARM GAS  C:\Users\Qian\AppData\Local\Temp\cc4EWhcg.s 			page 5


 198              		.global	_ZN10DigitalOut4readEv
 199              		.syntax unified
 200              		.thumb
 201              		.thumb_func
 202              		.fpu softvfp
 204              	_ZN10DigitalOut4readEv:
 205              		.fnstart
 206              	.LFB71:
  34:Src/DigitalOut.cpp **** 
  35:Src/DigitalOut.cpp **** int DigitalOut::read()
  36:Src/DigitalOut.cpp **** {
 207              		.loc 1 36 0
 208              		.cfi_startproc
 209              		@ args = 0, pretend = 0, frame = 0
 210              		@ frame_needed = 0, uses_anonymous_args = 0
 211              		@ link register save eliminated.
 212              	.LVL18:
  37:Src/DigitalOut.cpp ****     return this->state;
  38:Src/DigitalOut.cpp **** }
 213              		.loc 1 38 0
 214 0000 0068     		ldr	r0, [r0]
 215              	.LVL19:
 216 0002 7047     		bx	lr
 217              		.cfi_endproc
 218              	.LFE71:
 219              		.cantunwind
 220              		.fnend
 222              		.section	.text._ZN10DigitalOutaSEi,"ax",%progbits
 223              		.align	1
 224              		.global	_ZN10DigitalOutaSEi
 225              		.syntax unified
 226              		.thumb
 227              		.thumb_func
 228              		.fpu softvfp
 230              	_ZN10DigitalOutaSEi:
 231              		.fnstart
 232              	.LFB72:
  39:Src/DigitalOut.cpp **** 
  40:Src/DigitalOut.cpp **** DigitalOut& DigitalOut::operator= (int value)
  41:Src/DigitalOut.cpp **** {
 233              		.loc 1 41 0
 234              		.cfi_startproc
 235              		@ args = 0, pretend = 0, frame = 0
 236              		@ frame_needed = 0, uses_anonymous_args = 0
 237              	.LVL20:
 238 0000 10B5     		push	{r4, lr}
 239              		.save {r4, lr}
 240              	.LCFI4:
 241              		.cfi_def_cfa_offset 8
 242              		.cfi_offset 4, -8
 243              		.cfi_offset 14, -4
 244 0002 0446     		mov	r4, r0
  42:Src/DigitalOut.cpp ****     if (value == 0)
 245              		.loc 1 42 0
 246 0004 19B1     		cbz	r1, .L17
  43:Src/DigitalOut.cpp ****     {
  44:Src/DigitalOut.cpp ****         HAL_GPIO_WritePin(this->GPIOx, this->GPIO_Pin, GPIO_PIN_RESET);
ARM GAS  C:\Users\Qian\AppData\Local\Temp\cc4EWhcg.s 			page 6


  45:Src/DigitalOut.cpp ****         this->state = 0;
  46:Src/DigitalOut.cpp ****     }
  47:Src/DigitalOut.cpp ****     else if (value == 1)
 247              		.loc 1 47 0
 248 0006 0129     		cmp	r1, #1
 249 0008 09D0     		beq	.L18
 250              	.LVL21:
 251              	.L15:
  48:Src/DigitalOut.cpp ****     {
  49:Src/DigitalOut.cpp ****         HAL_GPIO_WritePin(this->GPIOx, this->GPIO_Pin, GPIO_PIN_SET);
  50:Src/DigitalOut.cpp ****         this->state = 1;
  51:Src/DigitalOut.cpp ****     }
  52:Src/DigitalOut.cpp ****     return *this;
  53:Src/DigitalOut.cpp **** }
 252              		.loc 1 53 0
 253 000a 2046     		mov	r0, r4
 254 000c 10BD     		pop	{r4, pc}
 255              	.LVL22:
 256              	.L17:
  44:Src/DigitalOut.cpp ****         this->state = 0;
 257              		.loc 1 44 0
 258 000e 0022     		movs	r2, #0
 259 0010 0189     		ldrh	r1, [r0, #8]
 260              	.LVL23:
 261 0012 4068     		ldr	r0, [r0, #4]
 262              	.LVL24:
 263 0014 FFF7FEFF 		bl	HAL_GPIO_WritePin
 264              	.LVL25:
  45:Src/DigitalOut.cpp ****     }
 265              		.loc 1 45 0
 266 0018 0023     		movs	r3, #0
 267 001a 2360     		str	r3, [r4]
 268 001c F5E7     		b	.L15
 269              	.LVL26:
 270              	.L18:
  49:Src/DigitalOut.cpp ****         this->state = 1;
 271              		.loc 1 49 0
 272 001e 0122     		movs	r2, #1
 273 0020 0189     		ldrh	r1, [r0, #8]
 274              	.LVL27:
 275 0022 4068     		ldr	r0, [r0, #4]
 276              	.LVL28:
 277 0024 FFF7FEFF 		bl	HAL_GPIO_WritePin
 278              	.LVL29:
  50:Src/DigitalOut.cpp ****     }
 279              		.loc 1 50 0
 280 0028 0123     		movs	r3, #1
 281 002a 2360     		str	r3, [r4]
 282 002c EDE7     		b	.L15
 283              		.cfi_endproc
 284              	.LFE72:
 285              		.fnend
 287              		.text
 288              	.Letext0:
 289              		.file 2 "d:\\program files (x86)\\gnu tools arm embedded\\7 2017-q4-major\\arm-none-eabi\\include\
 290              		.file 3 "d:\\program files (x86)\\gnu tools arm embedded\\7 2017-q4-major\\arm-none-eabi\\include\
 291              		.file 4 "Drivers/CMSIS/Include/core_cm3.h"
ARM GAS  C:\Users\Qian\AppData\Local\Temp\cc4EWhcg.s 			page 7


 292              		.file 5 "Drivers/CMSIS/Device/ST/STM32F1xx/Include/system_stm32f1xx.h"
 293              		.file 6 "Drivers/CMSIS/Device/ST/STM32F1xx/Include/stm32f103xb.h"
 294              		.file 7 "d:\\program files (x86)\\gnu tools arm embedded\\7 2017-q4-major\\arm-none-eabi\\include\
 295              		.file 8 "d:\\program files (x86)\\gnu tools arm embedded\\7 2017-q4-major\\arm-none-eabi\\include\
 296              		.file 9 "d:\\program files (x86)\\gnu tools arm embedded\\7 2017-q4-major\\lib\\gcc\\arm-none-eabi
 297              		.file 10 "d:\\program files (x86)\\gnu tools arm embedded\\7 2017-q4-major\\arm-none-eabi\\include
 298              		.file 11 "Drivers/STM32F1xx_HAL_Driver/Inc/stm32f1xx_hal_gpio.h"
 299              		.file 12 "Inc/DigitalOut.h"
 300              		.file 13 "<built-in>"
ARM GAS  C:\Users\Qian\AppData\Local\Temp\cc4EWhcg.s 			page 8


DEFINED SYMBOLS
                            *ABS*:00000000 DigitalOut.cpp
C:\Users\Qian\AppData\Local\Temp\cc4EWhcg.s:16     .text._ZN10DigitalOutC2EP12GPIO_TypeDeft:00000000 $t
C:\Users\Qian\AppData\Local\Temp\cc4EWhcg.s:23     .text._ZN10DigitalOutC2EP12GPIO_TypeDeft:00000000 _ZN10DigitalOutC2EP12GPIO_TypeDeft
C:\Users\Qian\AppData\Local\Temp\cc4EWhcg.s:97     .text._ZN10DigitalOutC2EP12GPIO_TypeDeft:00000044 $d
.ARM.exidx.text._ZN10DigitalOutC2EP12GPIO_TypeDeft:00000000 $d
C:\Users\Qian\AppData\Local\Temp\cc4EWhcg.s:23     .text._ZN10DigitalOutC2EP12GPIO_TypeDeft:00000000 _ZN10DigitalOutC1EP12GPIO_TypeDeft
C:\Users\Qian\AppData\Local\Temp\cc4EWhcg.s:106    .text._ZN10DigitalOutD2Ev:00000000 $t
C:\Users\Qian\AppData\Local\Temp\cc4EWhcg.s:113    .text._ZN10DigitalOutD2Ev:00000000 _ZN10DigitalOutD2Ev
.ARM.exidx.text._ZN10DigitalOutD2Ev:00000000 $d
C:\Users\Qian\AppData\Local\Temp\cc4EWhcg.s:113    .text._ZN10DigitalOutD2Ev:00000000 _ZN10DigitalOutD1Ev
C:\Users\Qian\AppData\Local\Temp\cc4EWhcg.s:132    .text._ZN10DigitalOut5writeEi:00000000 $t
C:\Users\Qian\AppData\Local\Temp\cc4EWhcg.s:139    .text._ZN10DigitalOut5writeEi:00000000 _ZN10DigitalOut5writeEi
.ARM.exidx.text._ZN10DigitalOut5writeEi:00000000 $d
C:\Users\Qian\AppData\Local\Temp\cc4EWhcg.s:197    .text._ZN10DigitalOut4readEv:00000000 $t
C:\Users\Qian\AppData\Local\Temp\cc4EWhcg.s:204    .text._ZN10DigitalOut4readEv:00000000 _ZN10DigitalOut4readEv
.ARM.exidx.text._ZN10DigitalOut4readEv:00000000 $d
C:\Users\Qian\AppData\Local\Temp\cc4EWhcg.s:223    .text._ZN10DigitalOutaSEi:00000000 $t
C:\Users\Qian\AppData\Local\Temp\cc4EWhcg.s:230    .text._ZN10DigitalOutaSEi:00000000 _ZN10DigitalOutaSEi
.ARM.exidx.text._ZN10DigitalOutaSEi:00000000 $d

UNDEFINED SYMBOLS
HAL_GPIO_WritePin
HAL_GPIO_Init
__aeabi_unwind_cpp_pr0
