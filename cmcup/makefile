
PROJECT = blink

TOOLPREFIX = arm-none-eabi
CC = $(TOOLPREFIX)-g++
OBJCOPY = $(TOOLPREFIX)-objcopy

CMSISDEVICE = Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x
CMSISCORE = Libraries/CMSIS/CM3/CoreSupport
COMPORT = /dev/tty.SLAB_USBtoUART

OBJS = src/main.o \
	src/stm32f10x_it.o \
	src/system.o \
	src/startup.o \
<<<<<<< HEAD
	src/gpio.o
	#src/uart.o
=======
	src/gpio.o 
>>>>>>> 0dd09610fa64868acf6379d6abb0a9eea5c0fc16

CPU = -mcpu=cortex-m3
MCU = $(CPU) -mthumb

CFLAG = -Xlinker --gc-sections -Wl,-Map,$(PROJECT).map $(MCU)
<<<<<<< HEAD

#MACRO += -DUSE_STDPERIPH_DRIVER

# Select target device
#MACRO += -DSTM32F10X_LD    # STM32F10[1,2,3]x[4-6]  16 and   32 Kbytes
#MACRO += -DSTM32F10X_LD_VL # STM32F100x[4-6]        16 and   32 Kbytes
MACRO += -DSTM32F10X_MD    # STM32F10[1,2,3]x[8-B]  64 and  128 Kbytes
#MACRO += -DSTM32F10X_MD_VL # STM32F100x[8-B]        64 and  128 Kbytes
#MACRO += -DSTM32F10X_HD    # STM32F10[1,3]x[C-E]   256 and  512 Kbytes
#MACRO += -DSTM32F10X_HD_VL # STM32F100x[C-E]       256 and  512 Kbytes
#MACRO += -DSTM32F10X_XL    # STM32F10[1,3]x[C-E]   512 and 1024 Kbytes
#MACRO += -DSTM32F10X_CL    # STM32F10[5,7]

# Select one of the frequency
#MACRO += -SYSCLK_FREQ_HSE  # F105,F107 are 25Mhz, others are 8Mhz
#MACRO += -DSYSCLK_FREQ_24MHz # F100 is limited to 24Mhz
#MACRO += -DSYSCLK_FREQ_36MHz
#MACRO += -DSYSCLK_FREQ_48MHz
#MACRO += -DSYSCLK_FREQ_56MHz
MACRO += -DSYSCLK_FREQ_72MHz

=======
MACRO = -DSTM32F10X_MD_VL # For stm32f100xx 
# MACRO = -DSTM32F10X_MD  # For stm32f103c8
>>>>>>> 0dd09610fa64868acf6379d6abb0a9eea5c0fc16
INCLUDE = \
	-I$(CMSISDEVICE) -I$(CMSISCORE) \
	-Isrc/include

all: $(PROJECT).hex

.PHONY: clean

$(PROJECT).hex: $(PROJECT).elf
	@echo 'Making $@'
	$(OBJCOPY) -O ihex blink.elf $@
	@echo 'Finished building target: $@'

$(PROJECT).elf: $(OBJS)
	@echo 'Building : $@'
	$(CC) -specs=nosys.specs -specs=nano.specs -T"stm32_flash.ld" $(INCLUDE) $(MACRO) $(CFLAG) -o $@ $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'

.c.o:
	$(CC) $(CFLAG) $(INCLUDE) $(MACRO) -c -o $@ $<

dl: $(PROJECT).hex
	stm32isp -term 9600 -verbose -bootp RTS -reset DTR $(PROJECT).hex $(COMPORT) 115200

term: $(PROJECT).hex
	stm32isp -term 9600 -termonly -verbose -bootp RTS -reset DTR $(PROJECT).hex $(COMPORT) 115200

clean:
	-rm $(OBJS) blink.*
