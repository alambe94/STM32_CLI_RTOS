################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/FreeRTOS_CLI.c \
../Src/cli_commands.c \
../Src/cli_uart_interface.c \
../Src/dma.c \
../Src/gpio.c \
../Src/main.c \
../Src/printf.c \
../Src/ring_buffer.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_it.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32f4xx.c \
../Src/ucos_main.c \
../Src/usart.c 

OBJS += \
./Src/FreeRTOS_CLI.o \
./Src/cli_commands.o \
./Src/cli_uart_interface.o \
./Src/dma.o \
./Src/gpio.o \
./Src/main.o \
./Src/printf.o \
./Src/ring_buffer.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_it.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32f4xx.o \
./Src/ucos_main.o \
./Src/usart.o 

C_DEPS += \
./Src/FreeRTOS_CLI.d \
./Src/cli_commands.d \
./Src/cli_uart_interface.d \
./Src/dma.d \
./Src/gpio.d \
./Src/main.d \
./Src/printf.d \
./Src/ring_buffer.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_it.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f4xx.d \
./Src/ucos_main.d \
./Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/FreeRTOS_CLI.o: ../Src/FreeRTOS_CLI.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/FreeRTOS_CLI.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/cli_commands.o: ../Src/cli_commands.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/cli_commands.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/cli_uart_interface.o: ../Src/cli_uart_interface.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/cli_uart_interface.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/dma.o: ../Src/dma.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/dma.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/gpio.o: ../Src/gpio.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/gpio.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/main.o: ../Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/main.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/printf.o: ../Src/printf.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/printf.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ring_buffer.o: ../Src/ring_buffer.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ring_buffer.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32f4xx_hal_msp.o: ../Src/stm32f4xx_hal_msp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32f4xx_hal_msp.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/stm32f4xx_it.o: ../Src/stm32f4xx_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/stm32f4xx_it.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/syscalls.o: ../Src/syscalls.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/syscalls.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/sysmem.o: ../Src/sysmem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sysmem.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/system_stm32f4xx.o: ../Src/system_stm32f4xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/system_stm32f4xx.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ucos_main.o: ../Src/ucos_main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ucos_main.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/usart.o: ../Src/usart.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/usart.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

