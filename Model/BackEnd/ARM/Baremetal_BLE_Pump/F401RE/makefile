################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/STM32_BlueNRG/Utils/subdir.mk
-include Middlewares/STM32_BlueNRG/Interface/subdir.mk
-include Middlewares/STM32_BlueNRG/HCI/controller/subdir.mk
-include Middlewares/STM32_BlueNRG/HCI/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/subdir.mk
-include Drivers/CMSIS/subdir.mk
-include Drivers/BSP/X-NUCLEO-IDB0xA1/subdir.mk
-include Drivers/BSP/STM32F4xx-Nucleo/subdir.mk
-include Application/User/subdir.mk
-include Application/SW4STM32/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: SensorDemo.elf

# Tool invocations
SensorDemo.elf: $(OBJS) $(USER_OBJS) ../STM32F401RETx_FLASH.ld
	@echo 'Building target: $@'
	@echo 'Invoking: MCU GCC Linker'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -static -T"../STM32F401RETx_FLASH.ld" -Wl,-Map=output.map -Wl,--gc-sections -o "VSPM.elf" @"objects.list" $(USER_OBJS) $(LIBS) -lm
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS) SensorDemo.elf VSPM.bin
	-@echo ' '

post-build:
	-@echo 'Generating binary:'
	arm-none-eabi-objcopy -O binary "VSPM.elf" "VSPM.bin"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
