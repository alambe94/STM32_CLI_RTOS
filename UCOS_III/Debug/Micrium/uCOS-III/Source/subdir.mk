################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Micrium/uCOS-III/Source/os_cfg_app.c \
../Micrium/uCOS-III/Source/os_core.c \
../Micrium/uCOS-III/Source/os_dbg.c \
../Micrium/uCOS-III/Source/os_flag.c \
../Micrium/uCOS-III/Source/os_int.c \
../Micrium/uCOS-III/Source/os_mem.c \
../Micrium/uCOS-III/Source/os_msg.c \
../Micrium/uCOS-III/Source/os_mutex.c \
../Micrium/uCOS-III/Source/os_pend_multi.c \
../Micrium/uCOS-III/Source/os_prio.c \
../Micrium/uCOS-III/Source/os_q.c \
../Micrium/uCOS-III/Source/os_sem.c \
../Micrium/uCOS-III/Source/os_stat.c \
../Micrium/uCOS-III/Source/os_task.c \
../Micrium/uCOS-III/Source/os_tick.c \
../Micrium/uCOS-III/Source/os_time.c \
../Micrium/uCOS-III/Source/os_tmr.c \
../Micrium/uCOS-III/Source/os_var.c 

OBJS += \
./Micrium/uCOS-III/Source/os_cfg_app.o \
./Micrium/uCOS-III/Source/os_core.o \
./Micrium/uCOS-III/Source/os_dbg.o \
./Micrium/uCOS-III/Source/os_flag.o \
./Micrium/uCOS-III/Source/os_int.o \
./Micrium/uCOS-III/Source/os_mem.o \
./Micrium/uCOS-III/Source/os_msg.o \
./Micrium/uCOS-III/Source/os_mutex.o \
./Micrium/uCOS-III/Source/os_pend_multi.o \
./Micrium/uCOS-III/Source/os_prio.o \
./Micrium/uCOS-III/Source/os_q.o \
./Micrium/uCOS-III/Source/os_sem.o \
./Micrium/uCOS-III/Source/os_stat.o \
./Micrium/uCOS-III/Source/os_task.o \
./Micrium/uCOS-III/Source/os_tick.o \
./Micrium/uCOS-III/Source/os_time.o \
./Micrium/uCOS-III/Source/os_tmr.o \
./Micrium/uCOS-III/Source/os_var.o 

C_DEPS += \
./Micrium/uCOS-III/Source/os_cfg_app.d \
./Micrium/uCOS-III/Source/os_core.d \
./Micrium/uCOS-III/Source/os_dbg.d \
./Micrium/uCOS-III/Source/os_flag.d \
./Micrium/uCOS-III/Source/os_int.d \
./Micrium/uCOS-III/Source/os_mem.d \
./Micrium/uCOS-III/Source/os_msg.d \
./Micrium/uCOS-III/Source/os_mutex.d \
./Micrium/uCOS-III/Source/os_pend_multi.d \
./Micrium/uCOS-III/Source/os_prio.d \
./Micrium/uCOS-III/Source/os_q.d \
./Micrium/uCOS-III/Source/os_sem.d \
./Micrium/uCOS-III/Source/os_stat.d \
./Micrium/uCOS-III/Source/os_task.d \
./Micrium/uCOS-III/Source/os_tick.d \
./Micrium/uCOS-III/Source/os_time.d \
./Micrium/uCOS-III/Source/os_tmr.d \
./Micrium/uCOS-III/Source/os_var.d 


# Each subdirectory must supply rules for building sources it contributes
Micrium/uCOS-III/Source/os_cfg_app.o: ../Micrium/uCOS-III/Source/os_cfg_app.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Micrium/uCOS-III/Source/os_cfg_app.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Micrium/uCOS-III/Source/os_core.o: ../Micrium/uCOS-III/Source/os_core.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Micrium/uCOS-III/Source/os_core.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Micrium/uCOS-III/Source/os_dbg.o: ../Micrium/uCOS-III/Source/os_dbg.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Micrium/uCOS-III/Source/os_dbg.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Micrium/uCOS-III/Source/os_flag.o: ../Micrium/uCOS-III/Source/os_flag.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Micrium/uCOS-III/Source/os_flag.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Micrium/uCOS-III/Source/os_int.o: ../Micrium/uCOS-III/Source/os_int.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Micrium/uCOS-III/Source/os_int.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Micrium/uCOS-III/Source/os_mem.o: ../Micrium/uCOS-III/Source/os_mem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Micrium/uCOS-III/Source/os_mem.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Micrium/uCOS-III/Source/os_msg.o: ../Micrium/uCOS-III/Source/os_msg.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Micrium/uCOS-III/Source/os_msg.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Micrium/uCOS-III/Source/os_mutex.o: ../Micrium/uCOS-III/Source/os_mutex.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Micrium/uCOS-III/Source/os_mutex.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Micrium/uCOS-III/Source/os_pend_multi.o: ../Micrium/uCOS-III/Source/os_pend_multi.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Micrium/uCOS-III/Source/os_pend_multi.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Micrium/uCOS-III/Source/os_prio.o: ../Micrium/uCOS-III/Source/os_prio.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Micrium/uCOS-III/Source/os_prio.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Micrium/uCOS-III/Source/os_q.o: ../Micrium/uCOS-III/Source/os_q.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Micrium/uCOS-III/Source/os_q.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Micrium/uCOS-III/Source/os_sem.o: ../Micrium/uCOS-III/Source/os_sem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Micrium/uCOS-III/Source/os_sem.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Micrium/uCOS-III/Source/os_stat.o: ../Micrium/uCOS-III/Source/os_stat.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Micrium/uCOS-III/Source/os_stat.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Micrium/uCOS-III/Source/os_task.o: ../Micrium/uCOS-III/Source/os_task.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Micrium/uCOS-III/Source/os_task.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Micrium/uCOS-III/Source/os_tick.o: ../Micrium/uCOS-III/Source/os_tick.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Micrium/uCOS-III/Source/os_tick.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Micrium/uCOS-III/Source/os_time.o: ../Micrium/uCOS-III/Source/os_time.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Micrium/uCOS-III/Source/os_time.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Micrium/uCOS-III/Source/os_tmr.o: ../Micrium/uCOS-III/Source/os_tmr.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Micrium/uCOS-III/Source/os_tmr.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Micrium/uCOS-III/Source/os_var.o: ../Micrium/uCOS-III/Source/os_var.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Micrium/BSP -I../Micrium/Cfg -I../Micrium/uC-CPU/ARM-Cortex-M4/GNU -I../Micrium/uC-CPU -I../Micrium/uC-LIB -I../Micrium/uCOS-III/Ports/ARM-Cortex-M4/Generic/GNU -I../Micrium/uCOS-III/Source -I../Micrium/TraceRecorder/config -I../Micrium/TraceRecorder/include -I../Micrium/TraceRecorder/include/MicriumTraceHeaders -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Micrium/uCOS-III/Source/os_var.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

