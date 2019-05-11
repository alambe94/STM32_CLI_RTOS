################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Middlewares/Third_Party/TraceRecorder/streamports/Jlink_RTT/subdir.mk
-include Middlewares/Third_Party/TraceRecorder/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
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
STM32_CLI_FreeRTOS.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
STM32_CLI_FreeRTOS.list \

OBJCOPY_HEX += \
STM32_CLI_FreeRTOS.hex \

OBJCOPY_BIN += \
STM32_CLI_FreeRTOS.bin \


# All Target
all: STM32_CLI_FreeRTOS.elf secondary-outputs

# Tool invocations
STM32_CLI_FreeRTOS.elf: $(OBJS) $(USER_OBJS) /home/medprime/Documents/Projects/alsaad/Cube/WorkSpace/STM32_CLI/FreeRTOS/STM32F401RETX_FLASH.ld
	arm-none-eabi-gcc -o "STM32_CLI_FreeRTOS.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/home/medprime/Documents/Projects/alsaad/Cube/WorkSpace/STM32_CLI/FreeRTOS/STM32F401RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="STM32_CLI_FreeRTOS.map" -Wl,--gc-sections -static -u_printf_float  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

STM32_CLI_FreeRTOS.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "STM32_CLI_FreeRTOS.list"
	@echo 'Finished building: $@'
	@echo ' '

STM32_CLI_FreeRTOS.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "STM32_CLI_FreeRTOS.hex"
	@echo 'Finished building: $@'
	@echo ' '

STM32_CLI_FreeRTOS.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "STM32_CLI_FreeRTOS.bin"
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
