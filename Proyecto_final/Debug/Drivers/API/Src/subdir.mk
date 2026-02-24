################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/API/Src/API_ADC.c \
../Drivers/API/Src/API_GPIO.c \
../Drivers/API/Src/API_TIME.c 

OBJS += \
./Drivers/API/Src/API_ADC.o \
./Drivers/API/Src/API_GPIO.o \
./Drivers/API/Src/API_TIME.o 

C_DEPS += \
./Drivers/API/Src/API_ADC.d \
./Drivers/API/Src/API_GPIO.d \
./Drivers/API/Src/API_TIME.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/API/Src/%.o Drivers/API/Src/%.su Drivers/API/Src/%.cyclo: ../Drivers/API/Src/%.c Drivers/API/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F413xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Proyecto_final/Drivers/API_Inc -I"C:/STM32/Grupo_5_TDII_2025/Proyecto_final/Drivers/API/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-API-2f-Src

clean-Drivers-2f-API-2f-Src:
	-$(RM) ./Drivers/API/Src/API_ADC.cyclo ./Drivers/API/Src/API_ADC.d ./Drivers/API/Src/API_ADC.o ./Drivers/API/Src/API_ADC.su ./Drivers/API/Src/API_GPIO.cyclo ./Drivers/API/Src/API_GPIO.d ./Drivers/API/Src/API_GPIO.o ./Drivers/API/Src/API_GPIO.su ./Drivers/API/Src/API_TIME.cyclo ./Drivers/API/Src/API_TIME.d ./Drivers/API/Src/API_TIME.o ./Drivers/API/Src/API_TIME.su

.PHONY: clean-Drivers-2f-API-2f-Src

