TARGET := temp
BUILD := build
ELF := $(TARGET).elf
BIN := $(TARGET).bin
HEX := $(TARGET).hex
# 源文件路径
SRC_PATH :=	\
	./Libraries/STM32F1xx_HAL_Driver/Src/ \
	./User/src/ \
	./Public/ \
	./app/
# 头文件路径
INC_PATH := \
	./Libraries/CMSIS/ \
	./Libraries/STM32F1xx_HAL_Driver/Inc/ \
	./User/inc/ \
	./User/ \
	./Public/ \
	./app/

INC_FLAGS = $(patsubst %, -I%, $(INC_PATH)) 
# 源文件
SRC_C = $(notdir $(wildcard $(addsuffix *.c, $(SRC_PATH))))
SRC_CPP = $(notdir $(wildcard $(addsuffix *.cpp, $(SRC_PATH))))
# 启动文件
STARTUP_ASM_PATH := ./Libraries/CMSIS/
STARTUP_ASM = $(notdir $(wildcard $(addsuffix *.s, $(STARTUP_ASM_PATH))))

OBJS = \
	$(patsubst %.cpp, $(BUILD)/%.o, $(SRC_CPP)) \
	$(patsubst %.c, $(BUILD)/%.o, $(SRC_C)) \
	$(patsubst %.s, $(BUILD)/%.o, $(STARTUP_ASM)) \
# 基于ARM-GCC编译器的链接脚本
FLASH_LD := ./Libraries/CMSIS/STM32F103XE_FLASH.ld
# 芯片的架构
MCU_FLAGS := -mcpu=cortex-m3
# 宏定义
DEFS := USE_HAL_DRIVER STM32F103xE
DEFS +=	HSE_VALUE=8000000 # 配置STM32的外部晶振频率 8MHz
DEFS +=	RUN_FROM_FLASH=1 # 指定程序从硬件的FLASH中开始运行

DEFS_FLAGS = $(addprefix -D, $(DEFS))
#编译优化方面的选项
OPT_FLAGS := -Os 
OPT_FLAGS := -u _printf_double -u _printf_float # 打印浮点数
OPT_FLAGS += -fsingle-precision-constant # 指定编译器将浮点型常量看成单精度常量
OPT_FLAGS += -fno-common # 编译器应该将未初始化的全局变量放置到.o目标文件的数据段中而不是生成它们作为公共块
OPT_FLAGS += -ffunction-sections -fdata-sections# 该选项可以让链接器执行优化来改进指令空间中引用的局部性，但代价是牺牲了目标文件和可执行文件的空间大小和速度
# 编译器所要读取的规格（specs）文件
# SPECS_FLAGS := --specs=rdimon.specs -lrdimon #启用 semihosting 支持
# SPECS_FLAGS := --specs=nosys.specs #禁用系统调用支持
SPECS_FLAGS := --specs=nano.specs # 使用更小的标准库和运行时支持，不支持浮点数打印
# FUP
FPU_FLAGS += -mfloat-abi=soft # 无fpu
FPU_FLAGS += -mfpu=fpv4-sp-d16

LIBS := -lc -lm -lnosys # -lm选项用于链接libm.so库，即math数学函数库 -lgc链接c库 -lnosys不链接c库的系统调用部分
LIBDIR :=

# 开启调试信息
DEBUG = 0
ifeq ($(DEBUG), 1)
DEBUG_FLAGS := -g -gdwarf-2# gdb调试和DWARF-2格式的调试信息
else
DEBUG_FLAGS := 
endif

# 汇编标签 -c 不进行链接 -mthumb代码能以Thumb状态运行
ASFLAGS += $(FPU_FLAGS) $(SPECS_FLAGS) $(MCU_FLAGS) $(OPT_FLAGS)  $(DEBUG_FLAGS) -c -mthumb

# C编译标签 -fomit-frame-pointer减少栈帧的切换和栈地址的保存，可提高程序性能 -Wall选项用于打开代码的所有可选警告 
CFLAGS += $(ASFLAGS) -fomit-frame-pointer -Wall $(DEFS_FLAGS) $(INC_FLAGS) -std=c99
# CFLAGS += -fverbose-asm # 用于在生成的汇编代码中加入额外的注释信息来使汇编代码更具可读性
# CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" # 生成依赖信息

# CXX编译标签
CXXFLAGS += $(ASFLAGS) -fomit-frame-pointer -Wall $(DEFS_FLAGS) $(INC_FLAGS) -std=c++17
CXXFLAGS += -fno-exceptions # 禁用异常机制
CXXFLAGS += -fno-rtti # 禁用C++运行时类型信息的生成
CXXFLAGS += -fno-threadsafe-statics # 禁用局部静态变量的线程安全初始化
CXXFLAGS += -fvisibility=hidden # 隐藏ELF格式的符号名称
# CXXFLAGS += -fverbose-asm # 用于在生成的汇编代码中加入额外的注释信息来使汇编代码更具可读性

# 链接标签
LDFLAGS += $(FPU_FLAGS) $(SPECS_FLAGS) $(MCU_FLAGS) $(OPT_FLAGS)  $(DEBUG_FLAGS)
LDFLAGS += $(LIBDIR) $(LIBS)
LDFLAGS += -mthumb# -nostartfiles # -nostartfiles 在链接的时候不使用系统标准的启动文件 __libc_init_array()
LDFLAGS += -Wl,--gc-sections # 用于指定编译器不把未在可执行程序中进行调用的函数链接到可执行程序中，这样做可在一定程度上节约FLASH和内存空间
LDFLAGS += -T$(FLASH_LD) # 指定使用$(FLASH_LD)变量的值作为工程的链接脚本
LDFLAGS += -Wl,-Map=$(TARGET).map,--cref,--no-warn-mismatch # -Map=$(TARGET).map选项用于将链接的映射关系打印到标准输出上 --cref选项是Cross Reference的简写，用于输出交叉引用表 --no-warn-mismatch选项告诉链接器当出现不匹配的问题时，需要对其进行忽略

CC_PREFIX := arm-none-eabi-
CC = $(CC_PREFIX)gcc
CXX = $(CC_PREFIX)g++
CP = $(CC_PREFIX)objcopy
GDB = $(CC_PREFIX)gdb
SIZE = $(CC_PREFIX)size # 查看可执行文件或目标文件中代码段、数据段和其他部分的大小信息
AS = $(CC) -x assembler-with-cpp #-x assembler-with-cpp 指定源文件使用了哪种编程语言


TYPE_BURN  := openocd_swd_flash
TYPE_DEBUG := openocd_swd_debug
TYPE_ERASE := openocd_swd_erase

.PHONY: all  clean
all: $(OBJS) $(ELF) $(HEX) $(BIN)
	@$(SIZE) $(ELF)

clean :
ifeq ($(OS), Windows_NT)
	@-rmdir /s /q $(BUILD)
	@-del $(ELF)
	@-del $(HEX)
	@-del $(BIN)
	@-del $(TARGET).map
else ifeq ($(OS), Linux)
	@-rm -rf $(BUILD)
endif

$(BUILD) :
ifeq ($(OS), Windows_NT)
	@mkdir $@
else ifeq ($(OS), Linux)
	@mkdir -p $@
endif

vpath %.s $(STARTUP_ASM_PATH)
$(BUILD)/%.o: %.s $(BUILD)
	@$(AS) $(ASFLAGS) $< -o $@

vpath %.c $(SRC_PATH) 
$(BUILD)/%.o: %.c $(BUILD)
	@$(CC) $(CFLAGS) $< -o $@

vpath %.cpp $(SRC_PATH)
$(BUILD)/%.o: %.cpp
	@$(CXX) $(CXXFLAGS) $< -o $@

%.elf: $(OBJS)
	@$(CC) $^ $(LDFLAGS) -o $@

%.hex: %.elf
	@$(CP) -O ihex $< $@

%.bin: %.elf
	@$(CP) -O binary -S $< $@

.PHONY: burn erase # debug 
# -f interface/cmsis-dap.cfg 指定OpenOCD使用系统interface目录下的cmsis-dap.cfg调试适配器配置文件。
# -c “transport select swd”指定调试适配器使用SWD模式进行数据传输。
# -f target/stm32f1x.cfg 指定OpenOCD使用系统target目录下的stm32f1x.cfg芯片配置文件。
# -c “init” 该命令指定OpenOCD结束配置而进入运行阶段。
# -c “reset halt” 执行硬件复位，将所有已定义的目标重置，并立即停止目标的运行。
# -c “sleep 100” 指定OpenOCD在状态恢复之前等待至少100毫秒。
# -c “wait_halt 2” 指定OpenOCD等待目标停止的时间为2毫秒。
# -c “flash write_image erase $(BIN) 0x08000000” 指定OpenOCD将通过ARM-GCC编译生成的$(BIN)镜像文件烧写到当前目标硬件的FLASH块中，其中0x08000000是STM32芯片默认的重定位偏移量，在烧写镜像的过程中，它会被自动添加到镜像文件每个段的基址上，用于确保程序可以从正确的地址上运行。最后，在erase参数被指定后，相关FLASH扇区会在编程之前被擦除掉。
# -c “verify_image $(BIN) 0x08000000” 指定OpenOCD从0x08000000地址处开始验证$(BIN)镜像文件的数据是否正确，首先尝试使用CRC校验，若失败则直接对二进制文件进行比较。
# -c “reset run” 执行硬件复位，将所有已定义的目标重置，并开始目标的运行。
# -c shutdown 该命令会关闭OpenOCD服务器，并断开所有与之相连的客户端（如GDB，Telnet等）
burn : $(BIN)
	@openocd -f interface/cmsis-dap.cfg -c "transport select swd" -f target/stm32f1x.cfg -c "init" -c "reset halt" -c "sleep 100" -c "wait_halt 2" -c "flash write_image erase $(BIN) 0x08000000" -c "sleep 100" -c "verify_image $(BIN) 0x08000000" -c "sleep 100" -c "reset run" -c shutdown
erase :
	@openocd -f interface/cmsis-dap.cfg -c "transport select swd" -f target/stm32f1x.cfg  -c "init" -c "reset halt" -c "sleep 100" -c "stm32f1x mass_erase 0" -c "sleep 100" -c shutdown
# debug :
#	xterm -e openocd -f interface/cmsis-dap.cfg -c "transport select swd" -f target/stm32f1x.cfg -c "init" -c "halt" -c "reset halt" & $(GDB) --eval-command="target extended-remote localhost:3333" $(ELF)
