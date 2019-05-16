################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Micrium/uC-CPU/ARM-Cortex-M4/GNU/cpu_a.s 

C_SRCS += \
../Micrium/uC-CPU/ARM-Cortex-M4/GNU/cpu_c.c 

OBJS += \
./Micrium/uC-CPU/ARM-Cortex-M4/GNU/cpu_a.o \
./Micrium/uC-CPU/ARM-Cortex-M4/GNU/cpu_c.o 

C_DEPS += \
./Micrium/uC-CPU/ARM-Cortex-M4/GNU/cpu_c.d 


# Each subdirectory must supply rules for building sources it contributes
Micrium/uC-CPU/ARM-Cortex-M4/GNU/%.o: ../Micrium/uC-CPU/ARM-Cortex-M4/GNU/%.s
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -c -I../ -x assembler-with-cpp  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Micrium/uC-CPU/ARM-Cortex-M4/GNU/cpu_c.o: ../Micrium/uC-CPU/ARM-Cortex-M4/GNU/cpu_c.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Micrium/uC-CPU/ARM-Cortex-M4/GNU/cpu_c.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

