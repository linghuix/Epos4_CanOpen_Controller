################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/main.c \
../Core/Src/stm32f10x_it.c \
../Core/Src/system_stm32f1xx.c 

OBJS += \
./Core/Src/main.o \
./Core/Src/stm32f10x_it.o \
./Core/Src/system_stm32f1xx.o 

C_DEPS += \
./Core/Src/main.d \
./Core/Src/stm32f10x_it.d \
./Core/Src/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/main.o: ../Core/Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Core/Src -I../Core/BSP -I../Core/APP -I../Core/Func -I../Middlewares/CANOpen/inc -I../Middlewares/CANOpen/Cfg -I../Middlewares/CANOpen/inc/stm32 -I../Middlewares/uCOS-II/Ports -I../Middlewares/uCOS-II/Source -I../EPOS -I../Middlewares/uCOS-II/Cfg-Interface -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/main.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/Src/stm32f10x_it.o: ../Core/Src/stm32f10x_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Core/Src -I../Core/BSP -I../Core/APP -I../Core/Func -I../Middlewares/CANOpen/inc -I../Middlewares/CANOpen/Cfg -I../Middlewares/CANOpen/inc/stm32 -I../Middlewares/uCOS-II/Ports -I../Middlewares/uCOS-II/Source -I../EPOS -I../Middlewares/uCOS-II/Cfg-Interface -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/stm32f10x_it.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Core/Src/system_stm32f1xx.o: ../Core/Src/system_stm32f1xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xE -DUSE_HAL_DRIVER -DDEBUG -c -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Core/Src -I../Core/BSP -I../Core/APP -I../Core/Func -I../Middlewares/CANOpen/inc -I../Middlewares/CANOpen/Cfg -I../Middlewares/CANOpen/inc/stm32 -I../Middlewares/uCOS-II/Ports -I../Middlewares/uCOS-II/Source -I../EPOS -I../Middlewares/uCOS-II/Cfg-Interface -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/system_stm32f1xx.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

