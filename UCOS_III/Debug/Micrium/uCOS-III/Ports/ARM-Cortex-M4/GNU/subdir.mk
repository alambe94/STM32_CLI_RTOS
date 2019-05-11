################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Micrium/uCOS-III/Ports/ARM-Cortex-M4/GNU/os_cpu_c.c 

S_UPPER_SRCS += \
../Micrium/uCOS-III/Ports/ARM-Cortex-M4/GNU/os_cpu_a.S 

OBJS += \
./Micrium/uCOS-III/Ports/ARM-Cortex-M4/GNU/os_cpu_a.o \
./Micrium/uCOS-III/Ports/ARM-Cortex-M4/GNU/os_cpu_c.o 

C_DEPS += \
./Micrium/uCOS-III/Ports/ARM-Cortex-M4/GNU/os_cpu_c.d 


# Each subdirectory must supply rules for building sources it contributes
Micrium/uCOS-III/Ports/ARM-Cortex-M4/GNU/%.o: ../Micrium/uCOS-III/Ports/ARM-Cortex-M4/GNU/%.S
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -c -I../ -x assembler-with-cpp  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Micrium/uCOS-III/Ports/ARM-Cortex-M4/GNU/os_cpu_c.o: ../Micrium/uCOS-III/Ports/ARM-Cortex-M4/GNU/os_cpu_c.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/GNU -I../Micrium/uCOS-III/Source -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Micrium/uCOS-III/Ports/ARM-Cortex-M4/GNU/os_cpu_c.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

