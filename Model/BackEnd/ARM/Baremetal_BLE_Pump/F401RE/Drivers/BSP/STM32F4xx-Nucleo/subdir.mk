################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/STM32F4xx-Nucleo/stm32f4xx_nucleo.c 

OBJS += \
./Drivers/BSP/STM32F4xx-Nucleo/stm32f4xx_nucleo.o 

C_DEPS += \
./Drivers/BSP/STM32F4xx-Nucleo/stm32f4xx_nucleo.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32F4xx-Nucleo/stm32f4xx_nucleo.o: ../Drivers/BSP/STM32F4xx-Nucleo/stm32f4xx_nucleo.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -std=c99 -DNUCLEO_F401RE -DSTM32F401RETx -DSTM32F401xx -DSTM32F401xE -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO -I"../Application/hdr" -I"../Drivers/BSP/STM32F4xx-Nucleo" -I"../Drivers/CMSIS/Include" -I"../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../Drivers/BSP/X-NUCLEO-IDB0xA1" -I"../Middlewares/STM32_BlueNRG/SimpleBlueNRG_HCI/includes" -I"../Middlewares/STM32_BlueNRG/Interface" -Os -g -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


