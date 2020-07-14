################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/APP/app.c 

OBJS += \
./Core/APP/app.o 

C_DEPS += \
./Core/APP/app.d 


# Each subdirectory must supply rules for building sources it contributes
Core/APP/app.o: ../Core/APP/app.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Core/Src -I../Core/BSP -I../Core/APP -I../Core/Func -I../Middlewares/CANOpen/inc -I../Middlewares/CANOpen/Cfg -I../Middlewares/CANOpen/inc/stm32 -I../Middlewares/uCOS-II/Ports -I../Middlewares/uCOS-II/Source -I../EPOS -I../Middlewares/uCOS-II/Cfg-Interface -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/APP/app.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

