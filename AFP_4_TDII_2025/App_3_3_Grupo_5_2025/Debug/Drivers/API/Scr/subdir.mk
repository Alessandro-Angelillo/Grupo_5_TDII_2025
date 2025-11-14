################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/API/Scr/API_Delay.c \
../Drivers/API/Scr/API_GPIO.c 

OBJS += \
./Drivers/API/Scr/API_Delay.o \
./Drivers/API/Scr/API_GPIO.o 

C_DEPS += \
./Drivers/API/Scr/API_Delay.d \
./Drivers/API/Scr/API_GPIO.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/API/Scr/%.o Drivers/API/Scr/%.su Drivers/API/Scr/%.cyclo: ../Drivers/API/Scr/%.c Drivers/API/Scr/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F413xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/STM32/Grupo_5_TDII_2025/AFP_3_TDII_2025/App_3_3_Grupo_5_2025/Drivers/API/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-API-2f-Scr

clean-Drivers-2f-API-2f-Scr:
	-$(RM) ./Drivers/API/Scr/API_Delay.cyclo ./Drivers/API/Scr/API_Delay.d ./Drivers/API/Scr/API_Delay.o ./Drivers/API/Scr/API_Delay.su ./Drivers/API/Scr/API_GPIO.cyclo ./Drivers/API/Scr/API_GPIO.d ./Drivers/API/Scr/API_GPIO.o ./Drivers/API/Scr/API_GPIO.su

.PHONY: clean-Drivers-2f-API-2f-Scr

