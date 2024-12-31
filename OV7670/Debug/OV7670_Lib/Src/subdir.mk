################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../OV7670_Lib/Src/OV7670.cpp 

C_SRCS += \
../OV7670_Lib/Src/image_ops.c \
../OV7670_Lib/Src/ov7670_private.c 

C_DEPS += \
./OV7670_Lib/Src/image_ops.d \
./OV7670_Lib/Src/ov7670_private.d 

OBJS += \
./OV7670_Lib/Src/OV7670.o \
./OV7670_Lib/Src/image_ops.o \
./OV7670_Lib/Src/ov7670_private.o 

CPP_DEPS += \
./OV7670_Lib/Src/OV7670.d 


# Each subdirectory must supply rules for building sources it contributes
OV7670_Lib/Src/%.o OV7670_Lib/Src/%.su OV7670_Lib/Src/%.cyclo: ../OV7670_Lib/Src/%.cpp OV7670_Lib/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/User/STM32CubeIDE/workspace_1.15.0/OV7670/OV7670_Lib/Inc" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/User/STM32CubeIDE/workspace_1.15.0/OV7670/Core/Inc" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
OV7670_Lib/Src/%.o OV7670_Lib/Src/%.su OV7670_Lib/Src/%.cyclo: ../OV7670_Lib/Src/%.c OV7670_Lib/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/User/STM32CubeIDE/workspace_1.15.0/OV7670/OV7670_Lib/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-OV7670_Lib-2f-Src

clean-OV7670_Lib-2f-Src:
	-$(RM) ./OV7670_Lib/Src/OV7670.cyclo ./OV7670_Lib/Src/OV7670.d ./OV7670_Lib/Src/OV7670.o ./OV7670_Lib/Src/OV7670.su ./OV7670_Lib/Src/image_ops.cyclo ./OV7670_Lib/Src/image_ops.d ./OV7670_Lib/Src/image_ops.o ./OV7670_Lib/Src/image_ops.su ./OV7670_Lib/Src/ov7670_private.cyclo ./OV7670_Lib/Src/ov7670_private.d ./OV7670_Lib/Src/ov7670_private.o ./OV7670_Lib/Src/ov7670_private.su

.PHONY: clean-OV7670_Lib-2f-Src

