################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Micrium/BSP/bsp.c 

OBJS += \
./Micrium/BSP/bsp.o 

C_DEPS += \
./Micrium/BSP/bsp.d 


# Each subdirectory must supply rules for building sources it contributes
Micrium/BSP/bsp.o: ../Micrium/BSP/bsp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/GNU -I../Micrium/uCOS-III/Source -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Micrium/BSP/bsp.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

