TAR := temp
BUILD := build
OBJ = $(BUILD)/obj
ELF = $(BUILD)/$(TAR).elf
HEX = $(BUILD)/$(TAR).hex
BIN = $(BUILD)/$(TAR).bin
MAP = $(BUILD)/$(TAR).map
DIS = $(BUILD)/$(TAR).dis
# C源文件
SRC_C := \
	lib/driver/src/apm32f10x_rcm.c \
	lib/driver/src/apm32f10x_gpio.c \
	lib/driver/src/apm32f10x_usart.c \
	lib/driver/src/apm32f10x_dma.c \
	lib/driver/src/apm32f10x_misc.c \
	$(wildcard bsp/*.c) \
	app/led/led.c
# C++源文件
SRC_CPP := 
# 启动文件
STARTUP := lib/startup/startup_apm32f10x_md.S
# 汇编源文件
SRC_BS = \
	$(STARTUP)
SRC_SS := 
# 链接脚本
FLASH_LD := lib/startup/gcc_APM32F10xx6.ld
# 头文件路径
INC_PATH := \
	lib/CMSIS/ \
	lib/driver/inc/ \
	bsp/ \
	app/led/ \
	app/
# 宏定义
DEFINE := \
	APM32F10X_MD
# 支持浮点数打印  ON/OFF
PRINTF_FLOAT := OFF
# 调试信息 ON/OFF
DEBUG := OFF
# 优化等级 Og调试
LEVEL := Os
ifeq ($(DEBUG), ON)
LEVEL := Og
endif
# C版本
VERSION_C := c99
# C++版本
VERSION_CXX := c++17
# 芯片的架构
MCU_ARCH := cortex-m3
# FUP
FPU := soft# 无fpu

SPECS := nano.specs # 使用更小的标准库和运行时支持 只支持c
SPECS += nosys.specs # 为了支持c++ ？同时bin也更小了,也不需要-lc -lnosys -lm了
# -lm选项用于链接libm.so库，即math数学函数库 -lc链接c库 -lnosys不链接c库的系统调用部分
LIB := #-lc -lnosys -lm -lgcc
LIB_PATH :=
##############################################################################
OBJS = \
	$(patsubst %.c, $(OBJ)/%.o, $(notdir $(SRC_C))) \
	$(patsubst %.cpp, $(OBJ)/%.o, $(notdir $(SRC_CPP))) \
	$(patsubst %.S, $(OBJ)/%.o, $(notdir $(SRC_BS))) \
	$(patsubst %.s, $(OBJ)/%.o, $(notdir $(SRC_SS)))
# 规定搜索路径
vpath %.c $(sort $(dir $(SRC_C)))
vpath %.s $(sort $(dir $(SRC_SS)))
vpath %.S $(sort $(dir $(SRC_BS)))
vpath %.cpp $(sort $(dir $(SRC_CPP)))
vpath $(OBJ)/%.o $(OBJ)
################################# 编译标签 ####################################
INC_FLAGS = $(addprefix -I, $(INC_PATH))
DEFINE_FLAGS = $(addprefix -D, $(DEFINE))
SPECS_FLAGS = $(addprefix --specs=, $(SPECS))
ifeq ($(DEBUG), ON)
DEBUG_FLAGS := -g -gdwarf-2
else
DEBUG_FLAGS :=
endif
LIB_FLAGS = $(LIBDIR) $(LIB)
MCU_FLAGS = -mcpu=$(MCU_ARCH)
FPU_FLAGS = -mfloat-abi=$(FPU)# 无fpu
# 通用标签
OPT_FLAGS = -mthumb -$(LEVEL) $(MCU_FLAGS) $(FPU_FLAGS)
ifeq ($(PRINTF_FLOAT), ON)
OPT_FLAGS += -u _printf_float -u _scanf_float# 打印浮点数
endif
OPT_FLAGS += -fno-common# 编译器应该将未初始化的全局变量放置到.o目标文件的数据段中而不是生成它们作为公共块
OPT_FLAGS += -ffunction-sections -fdata-sections# 该选项可以让链接器执行优化来改进指令空间中引用的局部性，但代价是牺牲了目标文件和可执行文件的空间大小和速度
# 汇编标签
ASFLAGS = -c $(OPT_FLAGS) $(DEBUG_FLAGS) # -c 不进行链接 -mthumb代码能以Thumb状态运行
# C编译标签
CFLAGS = -c $(OPT_FLAGS) $(INC_FLAGS) $(DEFINE_FLAGS)
CFLAGS += -fomit-frame-pointer # -fomit-frame-pointer减少栈帧的切换和栈地址的保存，可提高程序性能 
CFLAGS += -std=$(VERSION_C) -Wall # Wall选项用于打开代码的所有可选警告
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"# 生成依赖信息
CFLAGS += #-fverbose-asm # 用于在生成的汇编代码中加入额外的注释信息来使汇编代码更具可读性
CFLAGS += $(DEBUG_FLAGS)
# C++编译标签
CXXFLAGS = -c $(OPT_FLAGS) $(INC_FLAGS) $(DEFINE_FLAGS)
CXXFLAGS += -fomit-frame-pointer
CXXFLAGS += -std=$(VERSION_CXX) -Wall
CXXFLAGS += -fno-exceptions# 禁用异常机制
CXXFLAGS += -fno-rtti# 禁用C++运行时类型信息的生成
CXXFLAGS += -fno-threadsafe-statics# 禁用局部静态变量的线程安全初始化
CXXFLAGS += -fvisibility=hidden# 隐藏ELF格式的符号名称
CXXFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"
CXXFLAGS += #-fverbose-asm
CXXFLAGS += $(DEBUG_FLAGS)
# 链接标签
LDFLAGS = $(OPT_FLAGS) $(SPECS_FLAGS) $(LIB_FLAGS)
LDFLAGS += #-nostartfiles# -nostartfiles 在链接的时候不使用系统标准的启动文件，要自己实现__libc_init_array
LDFLAGS += -T$(FLASH_LD)# 指定使用$(FLASH_LD)变量的值作为工程的链接脚本
LDFLAGS += -Wl,-Map=$(MAP),--cref# -Map=$(MAP)选项用于将链接的映射关系打印到标准输出上 --cref选项是Cross Reference的简写，用于输出交叉引用表
LDFLAGS += -Wl,--gc-sections# 用于指定编译器不把未在可执行程序中进行调用的函数链接到可执行程序中，这样做可在一定程度上节约FLASH和内存空间
LDFLAGS += #-Wl,-u,Reset_Handler
################################# 编译工具 ####################################
CC_PREFIX := arm-none-eabi-
CC = $(CC_PREFIX)gcc
CXX = $(CC_PREFIX)g++
AS = $(CXX) -x assembler-with-cpp#-x assembler-with-cpp 指定源文件使用了哪种编程语言
CP = $(CC_PREFIX)objcopy
DP = $(CC_PREFIX)objdump -D -m arm# 反汇编
SIZE = $(CC_PREFIX)size# 查看可执行文件或目标文件中代码段、数据段和其他部分的大小信息
################################# 编译规则 ####################################
.PHONY: defualt_rule all clean
defualt_rule: all

all: clean $(ELF) $(BIN) $(HEX)
	@$(DP) $(ELF) > $(DIS)
ifneq ($(OS), Windows_NT)
	@ls -l $(BIN)
endif
	@$(SIZE) $(ELF)

clean:
ifeq ($(OS), Windows_NT)
	@-rmdir /s /q $(BUILD)
else
	@-rm -rf $(BUILD)
endif

$(BIN): $(ELF)
	@$(CP) -O binary -S $< $@

$(HEX): $(ELF)
	@$(CP) -O ihex $< $@

$(ELF): $(OBJS)
	@$(CXX) $(OBJS) $(LDFLAGS) -o $@

$(OBJ)/%.o: %.c |$(OBJ)
	@$(CC) $(CFLAGS) $< -o $@

$(OBJ)/%.o: %.cpp |$(OBJ)
	@$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ)/%.o: %.s |$(OBJ)
	@$(AS) $(ASFLAGS) $< -o $@

$(OBJ)/%.o: %.S |$(OBJ)
	@$(AS) $(ASFLAGS) $< -o $@

$(OBJ): |$(BUILD)
	@mkdir $@

$(BUILD):
	@mkdir $@
################################ 烧录规则 #####################################
NAME := apm32f103c8
PACK := pack/Geehy.APM32F1xx_DFP.1.1.0.pack
# BASE_ADDR := 0x08000000

.PHONY: pack burn erase
pack: $(PACK)
	@pyocd list --targets --pack $<

burn: $(HEX) $(PACK)
	@pyocd flash --erase chip --target $(NAME) $< --pack $(PACK)

erase: $(PACK)
	@pyocd erase -t $(NAME) --chip  --pack $<

# 安装pyocd
# git clone https://github.com/pyocd/pyOCD.git
# cd pyOCD/udev/
# sudo cp *.rules /etc/udev/rules.d
# sudo udevadm control --reload
# sudo udevadm trigger
