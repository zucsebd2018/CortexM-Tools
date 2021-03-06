
PROJECT = bicycle

TOOLPREFIX = arm-none-eabi
CC = $(TOOLPREFIX)-g++
OBJCOPY = $(TOOLPREFIX)-objcopy

CMSISDEVICE = Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x
CMSISCORE = Libraries/CMSIS/CM3/CoreSupport
COMPORT = /dev/tty.SLAB_USBtoUART

LIBSRC = Libraries/STM32F10x_StdPeriph_Driver/src

OBJS = src/main.o \
	src/stm32f10x_it.o \
	src/system.o \
	src/startup.o \
	$(LIBSRC)/stm32f10x_gpio.o \
    $(LIBSRC)/stm32f10x_usart.o \
    $(LIBSRC)/stm32f10x_dma.o \
    $(LIBSRC)/stm32f10x_rcc.o \
    $(LIBSRC)/stm32f10x_tim.o \
    $(LIBSRC)/stm32f10x_exti.o \
    $(LIBSRC)/stm32f10x_pwr.o \
    $(LIBSRC)/misc.o
#OBJS = src/main.o \
#	src/stm32f10x_it.o \
#	src/system.o \
#	src/startup.o \
#	src/gpio.o\
#    $(LIBSRC)/stm32f10x_rcc.o \
#    $(LIBSRC)/stm32f10x_usart.o \
#    $(LIBSRC)/stm32f10x_dma.o \
#    $(LIBSRC)/misc.o\
#	src/uart.o

CPU = -mcpu=cortex-m3
MCU = $(CPU) -mthumb

CFLAG = -Xlinker --gc-sections -Wl,-Map,$(PROJECT).map $(MCU)

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

MACRO += -DUSE_STDPERIPH_DRIVER

INCLUDE = \
	-I$(CMSISDEVICE)\
	-I$(CMSISCORE) \
	-ILibraries/STM32F10x_StdPeriph_Driver/inc \
	-Isrc/include

all: $(PROJECT).hex

.PHONY: clean

$(PROJECT).hex: $(PROJECT).elf
	@echo 'Making $@'
	$(OBJCOPY) -O ihex $(PROJECT).elf $@
	@echo 'Finished building target: $@'

$(PROJECT).elf: $(OBJS)
	@echo 'Building : $@'
	$(CC) -specs=nosys.specs -T"stm32_flash.ld" $(INCLUDE) $(MACRO) $(CFLAG) -o $@ $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'

.c.o:
	$(CC) $(CFLAG) $(INCLUDE) $(MACRO) -c -o $@ $<

dl: $(PROJECT).hex
	stm32isp -term 9600 -verbose -bootp -RTS -reset DTR $(PROJECT).hex $(COMPORT) 115200

term: $(PROJECT).hex
	stm32isp -term 9600 -verbose -termonly  -bootp -RTS -reset DTR $(PROJECT).hex $(COMPORT) 115200

clean:
	-rm $(OBJS)
