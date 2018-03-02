# 生成的文件名<
PROJECT                  = CortexM-Tools_TEST

# 定义文件格式和文件名
TARGET                  := $(PROJECT)
TARGET_OUT              := $(TARGET).out
TARGET_HEX              := $(TARGET).hex
OBJCPFLAGS_ELF_TO_HEX    = -Oihex

# 定义路径
TOP_DIR      = .
SCRIPT_DIR  := $(TOP_DIR)/Scripts
STARTUP_DIR := $(TOP_DIR)/Startup
INC_DIR     := -I $(TOP_DIR)/Lib/inc -I $(TOP_DIR) -I $(TOP_DIR)/CMSIS

# 设置shell环境变量
#export LD_LIBRARY_PATH = $LD_LIBRARY_PATH:/Users/ch-yanghl/gcc-arm-none-eabi/arm-none-eabi/lib/thumb/

# 设置ld链接脚本文件
LDSCRIPT    := $(SCRIPT_DIR)/stm32_flash.ld

# 定义编译工具
CC          = arm-none-eabi-gcc
OBJCP       = arm-none-eabi-objcopy

# .c文件
CCFILE		:= ./CMSIS/*.c ./Lib/src/*.c *.c 

# 定义编译标志
CCFLAGS     += -mthumb -mcpu=cortex-m3 -mfloat-abi=soft -march=armv7-m

# .c文件编译时定义宏
CCFLAGS     += -D STM32F10X_MD -D USE_STDPERIPH_DRIVER

# 添加启动文件
SOURCE_ASM      += $(STARTUP_DIR)/startup_stm32f10x_md.S

# 替换文件后缀
C_OBJS      := $(SOURCE:%.c=%.o)
ASM_OBJS    := $(SOURCE_ASM:%.s=%.o)

# 编译命令的定义
COMPILE     = $(CC) $(INC_DIR) $(CCFLAGS) $(CCFILE) $(SOURCE_ASM) -Wl,--gc-sections -T $(LDSCRIPT) -o $@ 
ELF_TO_HEX  = $(OBJCP) $(OBJCPFLAGS_ELF_TO_HEX) $(TARGET_OUT) $@

#COMPILE		= arm-none-eabi-gcc -I ./Lib/inc -I . -I ./CMSIS -mthumb -mcpu=cortex-m3 -mfloat-abi=soft -march=armv7-m -D STM32F10X_MD -D USE_STDPERIPH_DRIVER ./CMSIS/*.c ./Lib/src/*.c *.c ./Startup/startup_stm32f10x_md.S -Wl,--gc-sections -T ./Scripts/stm32_flash.ld -o $(TARGET_OUT) 
#ELF_TO_HEX 	= arm-none-eabi-objcopy -Oihex $(TARGET_OUT) $(TARGET_HEX)
# 定义伪目标
.PHONY: all clean printf

# 定义规则
all: $(TARGET_HEX)
	@echo "build done"

$(TARGET_HEX): $(TARGET_OUT)
	$(ELF_TO_HEX)

$(TARGET_OUT): $(CCFILE) $(SOURCE_ASM)                                                                                              
	$(COMPILE)

# 清理项
clean:
	rm -f $(TARGET_HEX)
	rm -f $(TARGET_OUT)
	rm -f $(C_OBJS)
	@echo "clean done"