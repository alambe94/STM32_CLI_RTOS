################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Micrium/uC-LIB/Ports/ARM-Cortex-M4/GNU/lib_mem_a.s 

OBJS += \
./Micrium/uC-LIB/Ports/ARM-Cortex-M4/GNU/lib_mem_a.o 


# Each subdirectory must supply rules for building sources it contributes
Micrium/uC-LIB/Ports/ARM-Cortex-M4/GNU/%.o: ../Micrium/uC-LIB/Ports/ARM-Cortex-M4/GNU/%.s
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -c -I../ -x assembler-with-cpp  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

