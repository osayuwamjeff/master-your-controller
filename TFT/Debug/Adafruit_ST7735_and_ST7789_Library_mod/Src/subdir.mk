################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Adafruit_ST7735_and_ST7789_Library_mod/Src/Adafruit_ST7735_mod.cpp \
../Adafruit_ST7735_and_ST7789_Library_mod/Src/Adafruit_ST7789_mod.cpp \
../Adafruit_ST7735_and_ST7789_Library_mod/Src/Adafruit_ST77xx_mod.cpp 

OBJS += \
./Adafruit_ST7735_and_ST7789_Library_mod/Src/Adafruit_ST7735_mod.o \
./Adafruit_ST7735_and_ST7789_Library_mod/Src/Adafruit_ST7789_mod.o \
./Adafruit_ST7735_and_ST7789_Library_mod/Src/Adafruit_ST77xx_mod.o 

CPP_DEPS += \
./Adafruit_ST7735_and_ST7789_Library_mod/Src/Adafruit_ST7735_mod.d \
./Adafruit_ST7735_and_ST7789_Library_mod/Src/Adafruit_ST7789_mod.d \
./Adafruit_ST7735_and_ST7789_Library_mod/Src/Adafruit_ST77xx_mod.d 


# Each subdirectory must supply rules for building sources it contributes
Adafruit_ST7735_and_ST7789_Library_mod/Src/%.o Adafruit_ST7735_and_ST7789_Library_mod/Src/%.su Adafruit_ST7735_and_ST7789_Library_mod/Src/%.cyclo: ../Adafruit_ST7735_and_ST7789_Library_mod/Src/%.cpp Adafruit_ST7735_and_ST7789_Library_mod/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/User/STM32CubeIDE/workspace_1.15.0/TFT/Adafruit_ST7735_and_ST7789_Library_mod/Inc" -I"C:/Users/User/STM32CubeIDE/workspace_1.15.0/TFT/Adafruit_GFX_Library_mod/Inc" -I"C:/Users/User/STM32CubeIDE/workspace_1.15.0/TFT/Adafruit_GFX_Library_mod/Inc/Fonts" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Adafruit_ST7735_and_ST7789_Library_mod-2f-Src

clean-Adafruit_ST7735_and_ST7789_Library_mod-2f-Src:
	-$(RM) ./Adafruit_ST7735_and_ST7789_Library_mod/Src/Adafruit_ST7735_mod.cyclo ./Adafruit_ST7735_and_ST7789_Library_mod/Src/Adafruit_ST7735_mod.d ./Adafruit_ST7735_and_ST7789_Library_mod/Src/Adafruit_ST7735_mod.o ./Adafruit_ST7735_and_ST7789_Library_mod/Src/Adafruit_ST7735_mod.su ./Adafruit_ST7735_and_ST7789_Library_mod/Src/Adafruit_ST7789_mod.cyclo ./Adafruit_ST7735_and_ST7789_Library_mod/Src/Adafruit_ST7789_mod.d ./Adafruit_ST7735_and_ST7789_Library_mod/Src/Adafruit_ST7789_mod.o ./Adafruit_ST7735_and_ST7789_Library_mod/Src/Adafruit_ST7789_mod.su ./Adafruit_ST7735_and_ST7789_Library_mod/Src/Adafruit_ST77xx_mod.cyclo ./Adafruit_ST7735_and_ST7789_Library_mod/Src/Adafruit_ST77xx_mod.d ./Adafruit_ST7735_and_ST7789_Library_mod/Src/Adafruit_ST77xx_mod.o ./Adafruit_ST7735_and_ST7789_Library_mod/Src/Adafruit_ST77xx_mod.su

.PHONY: clean-Adafruit_ST7735_and_ST7789_Library_mod-2f-Src

