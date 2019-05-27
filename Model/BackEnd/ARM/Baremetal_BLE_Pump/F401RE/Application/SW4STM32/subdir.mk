################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Applications/SW4STM32/startup_stm32f401xe.s 

OBJS += \
./Application/SW4STM32/startup_stm32f401xe.o 


# Each subdirectory must supply rules for building sources it contributes
Application/SW4STM32/startup_stm32f401xe.o: ../Application/SW4STM32/startup_stm32f401xe.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -I"../Application/hdr" -I"../Drivers/BSP/STM32F4xx-Nucleo" -I"../Drivers/CMSIS/Include" -I"../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../Drivers/BSP/X-NUCLEO-IDB0xA1" -I"../Middlewares/STM32_BlueNRG/SimpleBlueNRG_HCI/includes" -I"../Middlewares/STM32_BlueNRG/Interface" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


