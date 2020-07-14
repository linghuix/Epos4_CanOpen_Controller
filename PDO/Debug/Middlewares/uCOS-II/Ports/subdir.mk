################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../Middlewares/uCOS-II/Ports/os_cpu_a.S 

OBJS += \
./Middlewares/uCOS-II/Ports/os_cpu_a.o 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/uCOS-II/Ports/%.o: ../Middlewares/uCOS-II/Ports/%.S
	arm-none-eabi-gcc -mcpu=cortex-m3 -g3 -c -I../ -x assembler-with-cpp --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

