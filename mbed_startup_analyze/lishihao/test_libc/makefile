PROJECT = blink

TOOLPREFIX = arm-none-eabi
CC = $(TOOLPREFIX)-g++
OBJCOPY = $(TOOLPREFIX)-objcopy

CMSISDEVICE = CMSIS/CM3/DeviceSupport/ST/STM32F10x
#PERIPH=Libraries/STM32F10x_StdPeriph_Driver
CMSISCORE = CMSIS/CM3/CoreSupport
COMPORT = /dev/tty.SLAB_USBtoUART

OBJS = main.o \
	system.o \
	startup.o \
	gpio.o

#	src/uart.o

CPU = -mcpu=cortex-m3
MCU = $(CPU) -mthumb
#   -Xlinker --no-gc-sections -Wl,-Map,
#-Xlinker --gc-sections -Wl,-Map,$(PROJECT).map
CFLAG = -save-temps -Xlinker --no-gc-sections -Wl,-Map,$(PROJECT).map $(MCU) -fno-exceptions -fno-rtti -ffunction-sections
MACRO = -DSTM32F10X_MD
INCLUDE = \
	-I$(CMSISDEVICE)\
	-I$(CMSISCORE) 

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
	-rm $(OBJS)
