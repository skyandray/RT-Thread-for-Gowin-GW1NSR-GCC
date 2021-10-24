
######################################
# target
######################################
TARGET = rt_thread


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
CORE/core_cm3.c \
CORE/cpuport.c \
finsh/cmd.c \
finsh/msh.c \
finsh/shell.c \
device/device.c \
rt-thread/src/clock.c \
rt-thread/src/components.c \
rt-thread/src/cpu.c \
rt-thread/src/idle.c \
rt-thread/src/ipc.c \
rt-thread/src/irq.c \
rt-thread/src/kservice.c \
rt-thread/src/mem.c \
rt-thread/src/memheap.c \
rt-thread/src/mempool.c \
rt-thread/src/object.c \
rt-thread/src/scheduler.c \
rt-thread/src/slab.c \
rt-thread/src/thread.c \
rt-thread/src/timer.c \
PERIPHERAL/Sources/gw1ns4c_gpio.c \
PERIPHERAL/Sources/gw1ns4c_i2c.c \
PERIPHERAL/Sources/gw1ns4c_misc.c \
PERIPHERAL/Sources/gw1ns4c_rtc.c \
PERIPHERAL/Sources/gw1ns4c_spi.c \
PERIPHERAL/Sources/gw1ns4c_syscon.c \
PERIPHERAL/Sources/gw1ns4c_timer.c \
PERIPHERAL/Sources/gw1ns4c_uart.c \
PERIPHERAL/Sources/gw1ns4c_wdog.c \
PERIPHERAL/Sources/system_gw1ns4c.c  \
USER/board.c \
USER/gw1ns4c_it.c \
USER/finsh_port.c \
USER/main.c

# ASM sources
ASM_SOURCES =  \
CORE/context_gcc.s \
STARTUP/startup_gw1ns4c.s



#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m3

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi


# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS = 


# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-ICORE \
-Ifinsh \
-Irt-thread/inc \
-IPERIPHERAL/Includes \
-ISYSTEM \
-IUSER


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = gw1ns4c_flash.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***