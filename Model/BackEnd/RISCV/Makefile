#############################################################
# Configuration
#############################################################

# If users don't specify RISCV_PATH then assume that the tools will just be in
# their path.
RISCV_PREFIX  := riscv64-unknown-elf
RISCV_GCC     := $(RISCV_PREFIX)-gcc
RISCV_GXX     := $(RISCV_PREFIX)-g++
RISCV_OBJDUMP := $(RISCV_PREFIX)-objdump
RISCV_GDB     := $(RISCV_PREFIX)-gdb
RISCV_AR      := $(RISCV_PREFIX)-ar

BSP_SUBDIR := env
BOARD := freedom-e300-arty 

PROGRAM := secpump
LINK_TARGET := flash

#############################################################
# BSP Loading
#############################################################

# Directory in which this BSP is located
board_dir := $(wildcard bsp/$(BSP_SUBDIR)/$(BOARD))

include $(board_dir)/settings.mk

ifeq ($(RISCV_ARCH),)
$(error $(board_dir)/board.mk must set RISCV_ARCH, the RISC-V ISA string to target)
endif

ifeq ($(RISCV_ABI),)
$(error $(board_dir)/board.mk must set RISCV_ABI, the ABI to target)
endif

# Determines the XLEN from the toolchain tuple
ifeq ($(patsubst rv32%,rv32,$(RISCV_ARCH)),rv32)
RISCV_XLEN := 32
else ifeq ($(patsubst rv64%,rv64,$(RISCV_ARCH)),rv64)
RISCV_XLEN := 64
else
$(error Unable to determine XLEN from $(RISCV_ARCH))
endif

#############################################################
# This Section is for Software Compilation
#############################################################
PROGRAM_ELF = software/$(PROGRAM)/$(PROGRAM)
PROGRAM_DIR=$(dir $(PROGRAM_ELF))

.PHONY: software
software: software_clean
	$(MAKE) -C $(PROGRAM_DIR) CC=$(RISCV_GCC) RISCV_ARCH=$(RISCV_ARCH) RISCV_ABI=$(RISCV_ABI) AR=$(RISCV_AR) BSP_BASE=$(abspath bsp) BOARD=$(BOARD) LINK_TARGET=$(LINK_TARGET)
.PHONY: software_clean

.PHONY: clean
clean: software_clean
software_clean:
	$(MAKE) -C $(PROGRAM_DIR) CC=$(RISCV_GCC) RISCV_ARCH=$(RISCV_ARCH) RISCV_ABI=$(RISCV_ABI) AR=$(RISCV_AR) BSP_BASE=$(abspath bsp) BOARD=$(BOARD) LINK_TARGET=$(LINK_TARGET) clean

