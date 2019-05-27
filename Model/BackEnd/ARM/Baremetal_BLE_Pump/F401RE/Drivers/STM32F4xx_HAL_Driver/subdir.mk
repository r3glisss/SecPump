################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c \
../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c \
../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c \
../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c 

OBJS += \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal.o \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_adc.o \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_adc_ex.o \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_cortex.o \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_dma.o \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_flash.o \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_flash_ex.o \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_gpio.o \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_pwr.o \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_pwr_ex.o \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_rcc.o \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_rcc_ex.o \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_spi.o \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_uart.o

C_DEPS += \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal.d \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_adc.d \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_adc_ex.d \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_cortex.d \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_dma.d \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_flash.d \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_flash_ex.d \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_gpio.d \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_pwr.d \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_pwr_ex.d \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_rcc.d \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_rcc_ex.d \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_spi.d \
./Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_uart.d


# Each subdirectory must supply rules for building sources it contributes
Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal.o: ../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -std=c99 -DNUCLEO_F401RE -DSTM32F401RETx -DSTM32F401xx -DSTM32F401xE -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO -I"../Application/hdr" -I"../Drivers/BSP/STM32F4xx-Nucleo" -I"../Drivers/CMSIS/Include" -I"../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../Drivers/BSP/X-NUCLEO-IDB0xA1" -I"../Middlewares/STM32_BlueNRG/SimpleBlueNRG_HCI/includes" -I"../Middlewares/STM32_BlueNRG/Interface" -Os -g -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_adc.o: ../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -std=c99 -DNUCLEO_F401RE -DSTM32F401RETx -DSTM32F401xx -DSTM32F401xE -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO -I"../Application/hdr" -I"../Drivers/BSP/STM32F4xx-Nucleo" -I"../Drivers/CMSIS/Include" -I"../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../Drivers/BSP/X-NUCLEO-IDB0xA1" -I"../Middlewares/STM32_BlueNRG/SimpleBlueNRG_HCI/includes" -I"../Middlewares/STM32_BlueNRG/Interface" -Os -g -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_adc_ex.o: ../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -std=c99 -DNUCLEO_F401RE -DSTM32F401RETx -DSTM32F401xx -DSTM32F401xE -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO -I"../Application/hdr" -I"../Drivers/BSP/STM32F4xx-Nucleo" -I"../Drivers/CMSIS/Include" -I"../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../Drivers/BSP/X-NUCLEO-IDB0xA1" -I"../Middlewares/STM32_BlueNRG/SimpleBlueNRG_HCI/includes" -I"../Middlewares/STM32_BlueNRG/Interface" -Os -g -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_cortex.o: ../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -std=c99 -DNUCLEO_F401RE -DSTM32F401RETx -DSTM32F401xx -DSTM32F401xE -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO -I"../Application/hdr" -I"../Drivers/BSP/STM32F4xx-Nucleo" -I"../Drivers/CMSIS/Include" -I"../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../Drivers/BSP/X-NUCLEO-IDB0xA1" -I"../Middlewares/STM32_BlueNRG/SimpleBlueNRG_HCI/includes" -I"../Middlewares/STM32_BlueNRG/Interface" -Os -g -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_dma.o: ../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -std=c99 -DNUCLEO_F401RE -DSTM32F401RETx -DSTM32F401xx -DSTM32F401xE -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO -I"../Application/hdr" -I"../Drivers/BSP/STM32F4xx-Nucleo" -I"../Drivers/CMSIS/Include" -I"../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../Drivers/BSP/X-NUCLEO-IDB0xA1" -I"../Middlewares/STM32_BlueNRG/SimpleBlueNRG_HCI/includes" -I"../Middlewares/STM32_BlueNRG/Interface" -Os -g -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_flash.o: ../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -std=c99 -DNUCLEO_F401RE -DSTM32F401RETx -DSTM32F401xx -DSTM32F401xE -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO -I"../Application/hdr" -I"../Drivers/BSP/STM32F4xx-Nucleo" -I"../Drivers/CMSIS/Include" -I"../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../Drivers/BSP/X-NUCLEO-IDB0xA1" -I"../Middlewares/STM32_BlueNRG/SimpleBlueNRG_HCI/includes" -I"../Middlewares/STM32_BlueNRG/Interface" -Os -g -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_flash_ex.o: ../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -std=c99 -DNUCLEO_F401RE -DSTM32F401RETx -DSTM32F401xx -DSTM32F401xE -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO -I"../Application/hdr" -I"../Drivers/BSP/STM32F4xx-Nucleo" -I"../Drivers/CMSIS/Include" -I"../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../Drivers/BSP/X-NUCLEO-IDB0xA1" -I"../Middlewares/STM32_BlueNRG/SimpleBlueNRG_HCI/includes" -I"../Middlewares/STM32_BlueNRG/Interface" -Os -g -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_gpio.o: ../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -std=c99 -DNUCLEO_F401RE -DSTM32F401RETx -DSTM32F401xx -DSTM32F401xE -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO -I"../Application/hdr" -I"../Drivers/BSP/STM32F4xx-Nucleo" -I"../Drivers/CMSIS/Include" -I"../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../Drivers/BSP/X-NUCLEO-IDB0xA1" -I"../Middlewares/STM32_BlueNRG/SimpleBlueNRG_HCI/includes" -I"../Middlewares/STM32_BlueNRG/Interface" -Os -g -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_pwr.o: ../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -std=c99 -DNUCLEO_F401RE -DSTM32F401RETx -DSTM32F401xx -DSTM32F401xE -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO -I"../Application/hdr" -I"../Drivers/BSP/STM32F4xx-Nucleo" -I"../Drivers/CMSIS/Include" -I"../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../Drivers/BSP/X-NUCLEO-IDB0xA1" -I"../Middlewares/STM32_BlueNRG/SimpleBlueNRG_HCI/includes" -I"../Middlewares/STM32_BlueNRG/Interface" -Os -g -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_pwr_ex.o: ../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -std=c99 -DNUCLEO_F401RE -DSTM32F401RETx -DSTM32F401xx -DSTM32F401xE -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO -I"../Application/hdr" -I"../Drivers/BSP/STM32F4xx-Nucleo" -I"../Drivers/CMSIS/Include" -I"../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../Drivers/BSP/X-NUCLEO-IDB0xA1" -I"../Middlewares/STM32_BlueNRG/SimpleBlueNRG_HCI/includes" -I"../Middlewares/STM32_BlueNRG/Interface" -Os -g -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_rcc.o: ../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -std=c99 -DNUCLEO_F401RE -DSTM32F401RETx -DSTM32F401xx -DSTM32F401xE -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO -I"../Application/hdr" -I"../Drivers/BSP/STM32F4xx-Nucleo" -I"../Drivers/CMSIS/Include" -I"../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../Drivers/BSP/X-NUCLEO-IDB0xA1" -I"../Middlewares/STM32_BlueNRG/SimpleBlueNRG_HCI/includes" -I"../Middlewares/STM32_BlueNRG/Interface" -Os -g -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_rcc_ex.o: ../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -std=c99 -DNUCLEO_F401RE -DSTM32F401RETx -DSTM32F401xx -DSTM32F401xE -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO -I"../Application/hdr" -I"../Drivers/BSP/STM32F4xx-Nucleo" -I"../Drivers/CMSIS/Include" -I"../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../Drivers/BSP/X-NUCLEO-IDB0xA1" -I"../Middlewares/STM32_BlueNRG/SimpleBlueNRG_HCI/includes" -I"../Middlewares/STM32_BlueNRG/Interface" -Os -g -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_spi.o: ../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -std=c99 -DNUCLEO_F401RE -DSTM32F401RETx -DSTM32F401xx -DSTM32F401xE -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO -I"../Application/hdr" -I"../Drivers/BSP/STM32F4xx-Nucleo" -I"../Drivers/CMSIS/Include" -I"../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../Drivers/BSP/X-NUCLEO-IDB0xA1" -I"../Middlewares/STM32_BlueNRG/SimpleBlueNRG_HCI/includes" -I"../Middlewares/STM32_BlueNRG/Interface" -Os -g -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/STM32F4xx_HAL_Driver/stm32f4xx_hal_uart.o: ../Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -std=c99 -DNUCLEO_F401RE -DSTM32F401RETx -DSTM32F401xx -DSTM32F401xE -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO -I"../Application/hdr" -I"../Drivers/BSP/STM32F4xx-Nucleo" -I"../Drivers/CMSIS/Include" -I"../Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"../Drivers/STM32F4xx_HAL_Driver/Inc" -I"../Drivers/BSP/X-NUCLEO-IDB0xA1" -I"../Middlewares/STM32_BlueNRG/SimpleBlueNRG_HCI/includes" -I"../Middlewares/STM32_BlueNRG/Interface" -Os -g -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

