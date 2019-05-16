################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Micrium/uCOS-III/Source/subdir.mk
-include Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU/subdir.mk
-include Micrium/uC-LIB/Ports/ARM-Cortex-M4/GNU/subdir.mk
-include Micrium/uC-LIB/subdir.mk
-include Micrium/uC-CPU/ARM-Cortex-M4/GNU/subdir.mk
-include Micrium/uC-CPU/subdir.mk
-include Micrium/BSP/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
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
STM32_CLI_UCOS_III.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
STM32_CLI_UCOS_III.list \

OBJCOPY_HEX += \
STM32_CLI_UCOS_III.hex \

OBJCOPY_BIN += \
STM32_CLI_UCOS_III.bin \


# All Target
all: STM32_CLI_UCOS_III.elf secondary-outputs

# Tool invocations
STM32_CLI_UCOS_III.elf: $(OBJS) $(USER_OBJS) /home/medprime/Documents/Projects/alsaad/Cube/WorkSpace/STM32_CLI/UCOS_III/STM32F401RETX_FLASH.ld
	arm-none-eabi-gcc -o "STM32_CLI_UCOS_III.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/home/medprime/Documents/Projects/alsaad/Cube/WorkSpace/STM32_CLI/UCOS_III/STM32F401RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="STM32_CLI_UCOS_III.map" -Wl,--gc-sections -static -u_printf_float  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

STM32_CLI_UCOS_III.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "STM32_CLI_UCOS_III.list"
	@echo 'Finished building: $@'
	@echo ' '

STM32_CLI_UCOS_III.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "STM32_CLI_UCOS_III.hex"
	@echo 'Finished building: $@'
	@echo ' '

STM32_CLI_UCOS_III.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "STM32_CLI_UCOS_III.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
