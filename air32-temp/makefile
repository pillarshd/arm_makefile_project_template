TARGET := temp
OUT := obj
BUILD = build
ELF := $(OUT)/$(TARGET).elf
BIN := $(OUT)/$(TARGET).bin
HEX := $(OUT)/$(TARGET).hex
MAP := $(OUT)/$(TARGET).map

# 源文件
SRC_C := \
	library/Driver/src/air32f10x_gpio.c \
	library/Driver/src/air32f10x_rcc_ex.c \
	library/Driver/src/air32f10x_rcc.c \
	library/Driver/src/air32f10x_usart.c \
	library/Driver/src/misc.c \
	library/Driver/src/system_air32f10x.c \
	bsp/air32f10x_it.c \
	bsp/debug.c \
	bsp/main.c \
	bsp/syscalls.c \
	bsp/system.c \
	bsp/systick.c \
	app/led/led.c \
	app/usart/usart.c

SRC_CPP :=

SRC_PATH = $(sort $(dir $(SRC_C))) $(sort $(dir $(SRC_CPP)))

# 头文件路径
INC_PATH := \
	library/CMSIS/ \
	library/Driver/inc/ \
	library/Driver/lib/cryptlib/ \
	bsp/ \
	app/led/ \
	app/usart/

# 启动文件
STARTUP_PATH := library/STARTUP/
STARTUP := library/STARTUP/startup_air32f10x.s

# 基于ARM-GCC编译器的链接脚本
FLASH_LD := library/STARTUP/AIR32F103XB_FLASH.ld

OBJ = \
	$(patsubst %.c,$(BUILD)/%.o,$(notdir $(SRC_C))) \
	$(patsubst %.s,$(BUILD)/%.o,$(notdir $(STARTUP))) \
	$(patsubst %.cpp,$(BUILD)/%.o,$(notdir $(SRC_CPP)))

##############################################################################
DEF := USE_STDPERIPH_DRIVER AIR32F10X_MD

# 芯片的架构
MCU := -mcpu=cortex-m3

# FUP
FPU += -mfloat-abi=soft# 无fpu

SPECS := nano.specs# 使用更小的标准库和运行时支持

# -lm选项用于链接libm.so库，即math数学函数库 -lgc链接c库 -lnosys不链接c库的系统调用部分
LIB := -lc -lnosys -lm
LIBDIR :=

##############################################################################
LIB_FLAGS = $(LIBDIR) $(LIB)

# 开启调试信息
DEBUG_FLAGS :=# -g -gdwarf-2

INC_FLAGS = $(addprefix -I, $(INC_PATH))

DEF_FLAGS = $(addprefix -D, $(DEF))

SPECS_FLAGS = --specs=$(SPECS)

#编译优化方面的选项
OPT_FLAGS := -Os $(MCU) $(FPU)
#OPT_FLAGS += -u _printf_float# 打印浮点数
OPT_FLAGS += -fno-common# 编译器应该将未初始化的全局变量放置到.o目标文件的数据段中而不是生成它们作为公共块
OPT_FLAGS += -ffunction-sections -fdata-sections# 该选项可以让链接器执行优化来改进指令空间中引用的局部性，但代价是牺牲了目标文件和可执行文件的空间大小和速度

##############################################################################
# 汇编标签 -c 不进行链接 -mthumb代码能以Thumb状态运行
ASFLAGS = -c -mthumb $(OPT_FLAGS) $(DEBUG_FLAGS)

# C编译标签 -fomit-frame-pointer减少栈帧的切换和栈地址的保存，可提高程序性能 -Wall选项用于打开代码的所有可选警告 
CFLAGS = -c -mthumb $(OPT_FLAGS) $(DEF_FLAGS) $(INC_FLAGS)
CFLAGS += -fomit-frame-pointer -Wall -std=c99
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"# 生成依赖信息
# CFLAGS += -fverbose-asm # 用于在生成的汇编代码中加入额外的注释信息来使汇编代码更具可读性
CFLAGS += $(DEBUG_FLAGS)

# CXX编译标签
CXXFLAGS = -c -mthumb $(OPT_FLAGS) $(DEF_FLAGS) $(INC_FLAGS)
CXXFLAGS += -fomit-frame-pointer -Wall -std=c++17
CXXFLAGS += -fno-exceptions# 禁用异常机制
CXXFLAGS += -fno-rtti# 禁用C++运行时类型信息的生成
CXXFLAGS += -fno-threadsafe-statics# 禁用局部静态变量的线程安全初始化
CXXFLAGS += -fvisibility=hidden# 隐藏ELF格式的符号名称
# CXXFLAGS += -fverbose-asm # 用于在生成的汇编代码中加入额外的注释信息来使汇编代码更具可读性
CXXFLAGS += $(DEBUG_FLAGS)

# 链接标签
LDFLAGS = -mthumb# -nostartfiles# -nostartfiles 在链接的时候不使用系统标准的启动文件，要自己实现__libc_init_array
LDFLAGS += $(OPT_FLAGS) $(SPECS_FLAGS) $(LIB_FLAGS)
LDFLAGS += -T$(FLASH_LD)# 指定使用$(FLASH_LD)变量的值作为工程的链接脚本
LDFLAGS += -Wl,-Map=$(MAP),--cref# -Map=$(MAP)选项用于将链接的映射关系打印到标准输出上 --cref选项是Cross Reference的简写，用于输出交叉引用表
LDFLAGS += -Wl,--gc-sections# 用于指定编译器不把未在可执行程序中进行调用的函数链接到可执行程序中，这样做可在一定程度上节约FLASH和内存空间
LDFLAGS += -Wl,-u,Reset_Handler

##############################################################################
CC_PREFIX := arm-none-eabi-
CC = $(CC_PREFIX)gcc
CXX = $(CC_PREFIX)g++
CP = $(CC_PREFIX)objcopy
GDB = $(CC_PREFIX)gdb
SIZE = $(CC_PREFIX)size# 查看可执行文件或目标文件中代码段、数据段和其他部分的大小信息
AS = $(CC) -x assembler-with-cpp#-x assembler-with-cpp 指定源文件使用了哪种编程语言

.PHONY: all  clean
all: $(OBJ) $(ELF) $(HEX) $(BIN)
	@$(SIZE) $(ELF)
#	@make burn

clean:
ifeq ($(OS), Windows_NT)
	@-rmdir /s /q $(OUT)
	@-rmdir /s /q $(BUILD)
else
	@-rm -rf $(OUT)
	@-rm -rf $(BUILD)
endif

$(BUILD):
ifeq ($(OS), Windows_NT)
	@mkdir $@
else 
	@mkdir -p $@
endif

$(OUT):
ifeq ($(OS), Windows_NT)
	@mkdir $@
else
	@mkdir -p $@
endif

vpath %.s $(STARTUP_PATH)
$(BUILD)/%.o: %.s $(BUILD)
	@$(AS) $(ASFLAGS) $< -o $@

vpath %.c $(SRC_PATH) 
$(BUILD)/%.o: %.c $(BUILD)
	@$(CC) $(CFLAGS) $< -o $@

vpath %.cpp $(SRC_PATH)
$(BUILD)/%.o: %.cpp $(BUILD)
	@$(CXX) $(CXXFLAGS) $< -o $@

$(ELF): $(OBJ) $(OUT)
	@$(CC) $(OBJ) $(LDFLAGS) -o $@

$(HEX): $(ELF)
	@$(CP) -O ihex $< $@

$(BIN): $(ELF)
	@$(CP) -O binary -S $< $@

##############################################################################
NAME := air32f103xb
PACK := ./Keil.AIR32F103_DFP.1.1.9.pack
BASE_ADDR := 0x08000000

.PHONY: pack burn erase
pack: $(PACK)
	@pyocd list --targets --pack $<

burn: $(HEX)
	@pyocd flash --erase chip --target $(NAME) $< 

erase: 
	@pyocd erase -t $(NAME) --chip

# sudo apt install git
# cd
# git clone https://github.com/pyocd/pyOCD.git
# cd pyOCD/udev/
# sudo cp *.rules /etc/udev/rules.d
# sudo udevadm control --reload
# sudo udevadm trigger
