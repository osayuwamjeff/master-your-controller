################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Adafruit_GFX_Library_mod/Src/Adafruit_GFX_mod.cpp \
../Adafruit_GFX_Library_mod/Src/Adafruit_GrayOLED.cpp \
../Adafruit_GFX_Library_mod/Src/Adafruit_SPITFT_mod.cpp 

C_SRCS += \
../Adafruit_GFX_Library_mod/Src/glcdfont.c 

C_DEPS += \
./Adafruit_GFX_Library_mod/Src/glcdfont.d 

OBJS += \
./Adafruit_GFX_Library_mod/Src/Adafruit_GFX_mod.o \
./Adafruit_GFX_Library_mod/Src/Adafruit_GrayOLED.o \
./Adafruit_GFX_Library_mod/Src/Adafruit_SPITFT_mod.o \
./Adafruit_GFX_Library_mod/Src/glcdfont.o 

CPP_DEPS += \
./Adafruit_GFX_Library_mod/Src/Adafruit_GFX_mod.d \
./Adafruit_GFX_Library_mod/Src/Adafruit_GrayOLED.d \
./Adafruit_GFX_Library_mod/Src/Adafruit_SPITFT_mod.d 


# Each subdirectory must supply rules for building sources it contributes
Adafruit_GFX_Library_mod/Src/%.o Adafruit_GFX_Library_mod/Src/%.su Adafruit_GFX_Library_mod/Src/%.cyclo: ../Adafruit_GFX_Library_mod/Src/%.cpp Adafruit_GFX_Library_mod/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/User/STM32CubeIDE/workspace_1.15.0/TFT/Adafruit_ST7735_and_ST7789_Library_mod/Inc" -I"C:/Users/User/STM32CubeIDE/workspace_1.15.0/TFT/Adafruit_GFX_Library_mod/Inc" -I"C:/Users/User/STM32CubeIDE/workspace_1.15.0/TFT/Adafruit_GFX_Library_mod/Inc/Fonts" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Adafruit_GFX_Library_mod/Src/%.o Adafruit_GFX_Library_mod/Src/%.su Adafruit_GFX_Library_mod/Src/%.cyclo: ../Adafruit_GFX_Library_mod/Src/%.c Adafruit_GFX_Library_mod/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/User/STM32CubeIDE/workspace_1.15.0/TFT/Adafruit_GFX_Library_mod/Inc" -I"C:/Users/User/STM32CubeIDE/workspace_1.15.0/TFT/Adafruit_ST7735_and_ST7789_Library_mod/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Adafruit_GFX_Library_mod-2f-Src

clean-Adafruit_GFX_Library_mod-2f-Src:
	-$(RM) ./Adafruit_GFX_Library_mod/Src/Adafruit_GFX_mod.cyclo ./Adafruit_GFX_Library_mod/Src/Adafruit_GFX_mod.d ./Adafruit_GFX_Library_mod/Src/Adafruit_GFX_mod.o ./Adafruit_GFX_Library_mod/Src/Adafruit_GFX_mod.su ./Adafruit_GFX_Library_mod/Src/Adafruit_GrayOLED.cyclo ./Adafruit_GFX_Library_mod/Src/Adafruit_GrayOLED.d ./Adafruit_GFX_Library_mod/Src/Adafruit_GrayOLED.o ./Adafruit_GFX_Library_mod/Src/Adafruit_GrayOLED.su ./Adafruit_GFX_Library_mod/Src/Adafruit_SPITFT_mod.cyclo ./Adafruit_GFX_Library_mod/Src/Adafruit_SPITFT_mod.d ./Adafruit_GFX_Library_mod/Src/Adafruit_SPITFT_mod.o ./Adafruit_GFX_Library_mod/Src/Adafruit_SPITFT_mod.su ./Adafruit_GFX_Library_mod/Src/glcdfont.cyclo ./Adafruit_GFX_Library_mod/Src/glcdfont.d ./Adafruit_GFX_Library_mod/Src/glcdfont.o ./Adafruit_GFX_Library_mod/Src/glcdfont.su

.PHONY: clean-Adafruit_GFX_Library_mod-2f-Src

