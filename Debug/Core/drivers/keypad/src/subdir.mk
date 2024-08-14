################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/drivers/keypad/src/keypad.c \
../Core/drivers/keypad/src/keypad_hw.c 

OBJS += \
./Core/drivers/keypad/src/keypad.o \
./Core/drivers/keypad/src/keypad_hw.o 

C_DEPS += \
./Core/drivers/keypad/src/keypad.d \
./Core/drivers/keypad/src/keypad_hw.d 


# Each subdirectory must supply rules for building sources it contributes
Core/drivers/keypad/src/%.o Core/drivers/keypad/src/%.su Core/drivers/keypad/src/%.cyclo: ../Core/drivers/keypad/src/%.c Core/drivers/keypad/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/oem/STM32CubeIDE/workspace_1.15.0/digital_clock/Core/drivers" -I"/home/oem/STM32CubeIDE/workspace_1.15.0/digital_clock/Core/drivers/LCD" -I"/home/oem/STM32CubeIDE/workspace_1.15.0/digital_clock/Core/drivers/LCD/inc" -I"/home/oem/STM32CubeIDE/workspace_1.15.0/digital_clock/Core/drivers/API/inc" -I"/home/oem/STM32CubeIDE/workspace_1.15.0/digital_clock/Drivers/BSP" -I"/home/oem/STM32CubeIDE/workspace_1.15.0/digital_clock/Drivers/BSP/STM32F4xx_Nucleo_144" -I"/home/oem/STM32CubeIDE/workspace_1.15.0/digital_clock/Core/drivers/RTC/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-drivers-2f-keypad-2f-src

clean-Core-2f-drivers-2f-keypad-2f-src:
	-$(RM) ./Core/drivers/keypad/src/keypad.cyclo ./Core/drivers/keypad/src/keypad.d ./Core/drivers/keypad/src/keypad.o ./Core/drivers/keypad/src/keypad.su ./Core/drivers/keypad/src/keypad_hw.cyclo ./Core/drivers/keypad/src/keypad_hw.d ./Core/drivers/keypad/src/keypad_hw.o ./Core/drivers/keypad/src/keypad_hw.su

.PHONY: clean-Core-2f-drivers-2f-keypad-2f-src

