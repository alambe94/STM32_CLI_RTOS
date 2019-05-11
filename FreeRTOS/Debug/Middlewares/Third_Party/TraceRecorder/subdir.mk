################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/TraceRecorder/trcKernelPort.c \
../Middlewares/Third_Party/TraceRecorder/trcSnapshotRecorder.c \
../Middlewares/Third_Party/TraceRecorder/trcStreamingRecorder.c 

OBJS += \
./Middlewares/Third_Party/TraceRecorder/trcKernelPort.o \
./Middlewares/Third_Party/TraceRecorder/trcSnapshotRecorder.o \
./Middlewares/Third_Party/TraceRecorder/trcStreamingRecorder.o 

C_DEPS += \
./Middlewares/Third_Party/TraceRecorder/trcKernelPort.d \
./Middlewares/Third_Party/TraceRecorder/trcSnapshotRecorder.d \
./Middlewares/Third_Party/TraceRecorder/trcStreamingRecorder.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/TraceRecorder/trcKernelPort.o: ../Middlewares/Third_Party/TraceRecorder/trcKernelPort.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/TraceRecorder/config -I../Middlewares/Third_Party/TraceRecorder/include -I../Middlewares/Third_Party/TraceRecorder/streamports/Jlink_RTT/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/TraceRecorder/trcKernelPort.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/TraceRecorder/trcSnapshotRecorder.o: ../Middlewares/Third_Party/TraceRecorder/trcSnapshotRecorder.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/TraceRecorder/config -I../Middlewares/Third_Party/TraceRecorder/include -I../Middlewares/Third_Party/TraceRecorder/streamports/Jlink_RTT/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/TraceRecorder/trcSnapshotRecorder.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/TraceRecorder/trcStreamingRecorder.o: ../Middlewares/Third_Party/TraceRecorder/trcStreamingRecorder.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/TraceRecorder/config -I../Middlewares/Third_Party/TraceRecorder/include -I../Middlewares/Third_Party/TraceRecorder/streamports/Jlink_RTT/include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/TraceRecorder/trcStreamingRecorder.d" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

