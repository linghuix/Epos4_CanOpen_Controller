################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Middlewares/uCOS-II/Source/subdir.mk
-include Middlewares/uCOS-II/Ports/subdir.mk
-include Middlewares/uCOS-II/Cfg-Interface/subdir.mk
-include Middlewares/CANOpen/src/subdir.mk
-include Middlewares/CANOpen/Cfg/subdir.mk
-include EPOS/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Core/Src/subdir.mk
-include Core/Func/subdir.mk
-include Core/BSP/subdir.mk
-include Core/APP/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
cc_1.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
cc_1.list \


# All Target
all: cc_1.elf secondary-outputs

# Tool invocations
cc_1.elf: $(OBJS) $(USER_OBJS) /home/test/STM32CubeIDE/workspace_1.0.2/cc_1/STM32F103ZETX_FLASH.ld
	arm-none-eabi-gcc -o "cc_1.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"/home/test/STM32CubeIDE/workspace_1.0.2/cc_1/STM32F103ZETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="cc_1.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

cc_1.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "cc_1.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
