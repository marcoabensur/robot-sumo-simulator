include config.mk

PROJECT     := sumo_hsm

OUTPUT    := $(PROJECT)

# C source files plataform-independent
C_SRCS := $(sort $(shell find ./src -name "*.c" -not -path "./src/bsp/target*"))

# List of all include directories needed by this project plataform-independent
C_HEADERS  = $(sort $(shell find ./inc -name "*.h" -not -path "./inc/bsp/target*"))

INCLUDES  := $(addprefix -I, $(sort $(dir $(C_HEADERS))))
INCLUDES += -I$(QPC)/include

# C++ source files
CPP_SRCS :=

QP_SRCS := \
	$(QPC)/src/qf/qep_hsm.c \
	$(QPC)/src/qf/qep_msm.c \
	$(QPC)/src/qf/qf_act.c \
	$(QPC)/src/qf/qf_actq.c \
	$(QPC)/src/qf/qf_defer.c \
	$(QPC)/src/qf/qf_dyn.c \
	$(QPC)/src/qf/qf_mem.c \
	$(QPC)/src/qf/qf_ps.c \
	$(QPC)/src/qf/qf_qact.c \
	$(QPC)/src/qf/qf_qeq.c \
	$(QPC)/src/qf/qf_qmact.c \
	$(QPC)/src/qf/qf_time.c \

ifeq (spy, $(CONF)) # SPY configuration ..................................


# For POSIX hosts (Linux, MacOS), you can choose:
# - the single-threaded QP/C port (win32-qv) or
# - the multithreaded QP/C port (win32).
#
QP_PORT_DIR := $(QPC)/ports/posix-qv
#QP_PORT_DIR := $(QPC)/ports/posix


C_HEADERS_TARGET  = $(sort $(shell find ./inc/bsp/target-pc -name "*.h"))

INCLUDES  += $(addprefix -I, $(sort $(dir $(C_HEADERS_TARGET))))
INCLUDES  += -I$(QP_PORT_DIR) 

QS_SRCS := \
	$(QPC)/src/qs/qs.c \
	$(QPC)/src/qs/qs_rx.c \
	$(QPC)/src/qs/qs_fp.c \
	$(QPC)/src/qs/qs_64bit.c \
	$(QP_PORT_DIR)/qs_port.c

C_SRCS += $(sort $(shell find ./src/bsp/target-pc -name "*.c"))
C_SRCS += $(QS_SRCS)

LD_SCRIPT :=

QP_SRCS += \
	$(QP_PORT_DIR)/qf_port.c

else # uC configuration ..................................

QP_PORT_DIR := $(QPC)/ports/arm-cm/qk/gnu

# list of all source directories used by this project

C_HEADERS_TARGET  = $(sort $(shell find ./inc/bsp/target-stm32f103 -name "*.h"))	
C_HEADERS_TARGET  += $(sort $(shell find ./$(CUBE_LOCATION) -name "*.h"))	

INCLUDES  += $(addprefix -I, $(sort $(dir $(C_HEADERS_TARGET))))
INCLUDES  += -I$(QP_PORT_DIR) 

# assembler source files
ASM_SRCS := $(shell find ./$(CUBE_LOCATION)/ -name "*.s")

C_SRCS += $(sort $(shell find ./src/bsp/target-stm32f103 -name "*.c"))
C_SRCS += $(sort $(shell find ./$(CUBE_LOCATION) -name "*.c"))

LD_SCRIPT := $(LINKER_FILE)

QP_SRCS += \
	$(QPC)/src/qk/qk.c \
	$(QP_PORT_DIR)/qk_port.c

endif


QP_ASMS :=

LIB_DIRS  :=
LIBS      :=

# defines
DEFINES   := \
	-DQP_API_VERSION=9999

ifeq (spy, $(CONF)) # SPY configuration ..................................

CC    := gcc
CPP   := g++
AS    := as
LINK  := gcc    # for C programs
BIN   := objcopy
SIZE    := size
GDB     := gdb
HEX     := $(BIN) -O ihex

else # uC configuration ..................................

CC    := $(ARM_GCC_PATH)/arm-none-eabi-gcc
CPP   := $(ARM_GCC_PATH)/arm-none-eabi-g++
AS    := $(ARM_GCC_PATH)/arm-none-eabi-as
LINK  := $(ARM_GCC_PATH)/arm-none-eabi-g++
BIN   := $(ARM_GCC_PATH)/arm-none-eabi-objcopy
SIZE    := $(ARM_GCC_PATH)/arm-none-eabi-size
GDB     := ${ARM_GCC_PATH}/arm-none-eabi-gdb
HEX     := $(BIN) -O ihex

endif

STM_PROG := $(CUBE_PROGRAMMER_PATH)/STM32_Programmer_CLI


##############################################################################

MKDIR := mkdir
RM    := rm -rf

#-----------------------------------------------------------------------------
# build options for various configurations for ARM Cortex-M
#

# combine all the soruces...
C_SRCS += $(QP_SRCS)
ASM_SRCS += $(QP_ASMS)

# Specify Search directories
VPATH = $(sort $(dir $(C_SRCS)))
VPATH += $(sort $(dir $(ASM_SRCS)))

ifeq (rel, $(CONF)) # Release configuration ..................................

BIN_DIR := rel

ASFLAGS = $(ARM_CPU) $(ARM_FPU) $(ASM_CPU) $(ASM_FPU)

CFLAGS = -c $(ARM_CPU) $(ARM_FPU) $(FLOAT_ABI) -mthumb -Wall \
	-ffunction-sections -fdata-sections \
	-O0 $(INCLUDES) $(DEFINES) -DNDEBUG

CPPFLAGS = -c $(ARM_CPU) $(ARM_FPU) $(FLOAT_ABI) -mthumb -Wall \
	-ffunction-sections -fdata-sections -fno-rtti -fno-exceptions \
	-O0 $(INCLUDES) $(DEFINES) -DNDEBUG

else ifeq (spy, $(CONF))  # Spy configuration ................................

BIN_DIR := build_spy

DEFINES += -DQ_SPY -D$(PROJECT_DEFS)

CFLAGS = -c -g -O -fno-pie -std=c11 -Wall -Wextra -W -Wno-unused-result -Wno-type-limits\
	$(INCLUDES) $(DEFINES) -Wno-unused-result

CPPFLAGS = -c -g -O -fno-pie -std=c++11 -pedantic -Wall -Wextra \
	-fno-rtti -fno-exceptions \
	$(INCLUDES) $(DEFINES)

ASFLAGS :=
LINKFLAGS := -no-pie
LIBS += -lpthread -lm

else # default Debug configuration ..........................................

BIN_DIR := build

DEFINES += -D$(DEVICE_DEF) \
	-DUSE_HAL_DRIVER -D$(PROJECT_DEFS)

ASFLAGS = -g $(ARM_CPU) $(ARM_FPU) $(ASM_CPU) $(ASM_FPU)

CFLAGS = -c -g $(ARM_CPU) $(ARM_FPU) $(FLOAT_ABI) -mthumb -Wall \
	-ffunction-sections -fdata-sections \
	-O0 $(INCLUDES) $(DEFINES)

CPPFLAGS = -c -g $(ARM_CPU) $(ARM_FPU) $(FLOAT_ABI) -mthumb -Wall \
	-ffunction-sections -fdata-sections -fno-rtti -fno-exceptions \
	-O0 $(INCLUDES) $(DEFINES)

LINKFLAGS = -T$(LD_SCRIPT) $(ARM_CPU) $(ARM_FPU) $(FLOAT_ABI) -mthumb \
	-specs=nosys.specs -specs=nano.specs \
	-Wl,-Map,$(BIN_DIR)/$(OUTPUT).map,--cref,--gc-sections $(LIB_DIRS)

endif # ......................................................................



ASM_OBJS     := $(patsubst %.s,%.o,  $(notdir $(ASM_SRCS)))
C_OBJS       := $(patsubst %.c,%.o,  $(notdir $(C_SRCS)))
CPP_OBJS     := $(patsubst %.cpp,%.o,$(notdir $(CPP_SRCS)))

TARGET_BIN   := $(BIN_DIR)/$(OUTPUT).bin
TARGET_ELF   := $(BIN_DIR)/$(OUTPUT).elf
TARGET_HEX   := $(BIN_DIR)/$(OUTPUT).hex

ifeq (spy, $(CONF)) # SPY configuration ..................................
TARGET_EXE   := $(BIN_DIR)/$(OUTPUT)
endif

ASM_OBJS_EXT := $(addprefix $(BIN_DIR)/, $(ASM_OBJS))
C_OBJS_EXT   := $(addprefix $(BIN_DIR)/, $(C_OBJS))
C_DEPS_EXT   := $(patsubst %.o, %.d, $(C_OBJS_EXT))
CPP_OBJS_EXT := $(addprefix $(BIN_DIR)/, $(CPP_OBJS))
CPP_DEPS_EXT := $(patsubst %.o, %.d, $(CPP_OBJS_EXT))

# create $(BIN_DIR) if it does not exist
ifeq ("$(wildcard $(BIN_DIR))","")
$(shell $(MKDIR) $(BIN_DIR))
endif

#-----------------------------------------------------------------------------
# rules
#

all: $(TARGET_BIN) $(TARGET_HEX) $(TARGET_EXE)
#all: $(TARGET_ELF)

$(TARGET_HEX): $(TARGET_ELF)
	@echo "Creating $@"
	$(HEX) $< $@

$(TARGET_BIN): $(TARGET_ELF)
	@echo "Creating $@"
	$(BIN) -O binary $< $@

$(TARGET_EXE) : $(C_OBJS_EXT) $(CPP_OBJS_EXT)
	@echo "Creating $@"
	$(CC) $(CFLAGS) $(QPC)/include/qstamp.c -o $(BIN_DIR)/qstamp.o
	$(LINK) $(LINKFLAGS) $(LIB_DIRS) -o $@ $^ $(BIN_DIR)/qstamp.o $(LIBS)

$(TARGET_ELF) : $(ASM_OBJS_EXT) $(C_OBJS_EXT) $(CPP_OBJS_EXT)
	@echo "CC $@"
	$(CC) $(CFLAGS) $(QPC)/include/qstamp.c -o $(BIN_DIR)/qstamp.o
	$(LINK) $(LINKFLAGS) -o $@ $^ $(BIN_DIR)/qstamp.o $(LIBS)
	$(AT)$(SIZE) $@

$(BIN_DIR)/%.d : %.c
	@echo "DCC $<"
	$(CC) -MM -MT $(@:.d=.o) $(CFLAGS) $< > $@

$(BIN_DIR)/%.d : %.cpp
	@echo "DPP $<"
	$(CPP) -MM -MT $(@:.d=.o) $(CPPFLAGS) $< > $@

$(BIN_DIR)/%.o : %.s Makefile
	@echo "AS $<"
	$(AS) $(ASFLAGS) $< -o $@

$(BIN_DIR)/%.o : %.c Makefile
	@echo "CC $<"
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/%.o : %.cpp Makefile
	@echo "CPP $<"
	$(CPP) $(CPPFLAGS) $< -o $@

# include dependency files only if our goal depends on their existence
ifneq ($(MAKECMDGOALS),clean)
  ifneq ($(MAKECMDGOALS),show)
-include $(C_DEPS_EXT) $(CPP_DEPS_EXT)
  endif
endif

# Flash Built files with STM32CubeProgrammer
flash load:
	@echo "Flashing $(TARGET_HEX) with STM32_Programmer_CLI"
	$(AT)$(STM_PROG) -c port=SWD -w $(TARGET_HEX) -v -rst


.PHONY : clean
clean:
	$(AT) $(RM) $(BIN_DIR)


show:
	@echo PROJECT = $(PROJECT)
	@echo ""
	@echo CONF = $(CONF)
	@echo ""
	@echo DEFINES = $(DEFINES)
	@echo ""
	@echo ASM_FPU = $(ASM_FPU)
	@echo ""
	@echo ASM_SRCS = $(ASM_SRCS)
	@echo ""
	@echo C_SRCS = $(C_SRCS)
	@echo ""
	@echo CPP_SRCS = $(CPP_SRCS)
	@echo ""
	@echo ASM_OBJS_EXT = $(ASM_OBJS_EXT)
	@echo ""
	@echo C_OBJS_EXT = $(C_OBJS_EXT)
	@echo ""
	@echo C_DEPS_EXT = $(C_DEPS_EXT)
	@echo ""
	@echo CPP_DEPS_EXT = $(CPP_DEPS_EXT)
	@echo ""
	@echo TARGET_ELF = $(TARGET_ELF)
	@echo ""
	@echo INCLUDES = $(INCLUDES)
	@echo ""
	@echo VPATH = $(VPATH)


prepare:
	@echo "Preparing cube files"
	$(AT)-mv -f $(CUBE_LOCATION)/Src/main.c $(CUBE_LOCATION)/Src/cube_main.c
	$(AT)-rm -f $(CUBE_LOCATION)/Makefile

###############################################################################
## VS Code files
###############################################################################

VSCODE_FOLDER            := .vscode
VS_LAUNCH_FILE           := $(VSCODE_FOLDER)/launch.json
VS_C_CPP_PROPERTIES_FILE := $(VSCODE_FOLDER)/c_cpp_properties.json

NULL  :=
SPACE := $(NULL) #
COMMA := ,


ifeq (spy, $(CONF)) # SPY configuration ..................................
INTELLI_SENSE_MODE := linux-gcc-x64
COMPILER_PATH := /usr/bin/$(CC)
else
INTELLI_SENSE_MODE := linux-gcc-arm
COMPILER_PATH := $(CC)
endif



define VS_LAUNCH
{
	"version": "0.2.0",
	"configurations": [
	    {
	        "type": "cortex-debug",
	        "request": "launch",
	        "servertype": "stlink",
	        "cwd": "$${workspaceRoot}",
	        "gdbPath": "${GDB}",
	        "executable": "$(TARGET_ELF)",
	        "name": "Cortex Debug (ST-Link)",
	        "device": "$(DEVICE)",
	        "v1": false
	    },
	    {
	        "type": "cortex-debug",
	        "request": "launch",
	        "servertype": "jlink",
	        "cwd": "$${workspaceRoot}",
	        "gdbPath": "${GDB}",
	        "executable": "$(TARGET_ELF)",
	        "name": "Cortex Debug (J-Link)",
	        "device": "$(DEVICE)",
	        "interface": "swd",
	    }
	]
}
endef

define VS_CPP_PROPERTIES
{
	"configurations": [
	    {
	        "name": "CONFIG",
	        "includePath": [
	            $(subst -I,$(NULL),$(subst $(SPACE),$(COMMA),$(strip $(foreach inc,$(INCLUDES),"$(inc)"))))
	        ],

			"browse": {
                "path": [$(subst -I,$(NULL),$(subst $(SPACE),$(COMMA),$(strip $(foreach inc,$(INCLUDES),"$(inc)"))))],
                "limitSymbolsToIncludedHeaders": true
            },


	        "defines": [
	            $(subst -D,$(NULL),$(subst $(SPACE),$(COMMA),$(strip $(foreach def,$(DEFINES),"$(def)"))))
	        ],

	        "compilerPath": "$(COMPILER_PATH)",
	        "cStandard": "c99",
	        "cppStandard": "c++14",
	        "intelliSenseMode": "$(INTELLI_SENSE_MODE)"
	    }
	],
	"version": 4
}
endef

export VS_LAUNCH
export VS_CPP_PROPERTIES

vs_files: $(VS_LAUNCH_FILE) $(VS_C_CPP_PROPERTIES_FILE)

$(VS_LAUNCH_FILE): config.mk Makefile | $(VSCODE_FOLDER)
	$(AT)echo "$$VS_LAUNCH" > $@

$(VS_C_CPP_PROPERTIES_FILE): config.mk Makefile | $(VSCODE_FOLDER)
	$(AT)echo "$$VS_CPP_PROPERTIES" > $@

$(VSCODE_FOLDER):
	$(AT)mkdir -p $@
