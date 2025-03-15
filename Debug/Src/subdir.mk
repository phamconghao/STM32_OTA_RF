################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/dma.c \
../Src/main.c \
../Src/nrf24l01.c \
../Src/spi.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/timer.c \
../Src/uart.c 

OBJS += \
./Src/dma.o \
./Src/main.o \
./Src/nrf24l01.o \
./Src/spi.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/timer.o \
./Src/uart.o 

C_DEPS += \
./Src/dma.d \
./Src/main.d \
./Src/nrf24l01.d \
./Src/spi.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/timer.d \
./Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DSTM32F446xx -c -I../Inc -I"D:/WorkSpace/Document/STM32_Header/CMSIS/Device/ST/STM32F4xx/Include" -I"D:/WorkSpace/Document/STM32_Header/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/dma.cyclo ./Src/dma.d ./Src/dma.o ./Src/dma.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/nrf24l01.cyclo ./Src/nrf24l01.d ./Src/nrf24l01.o ./Src/nrf24l01.su ./Src/spi.cyclo ./Src/spi.d ./Src/spi.o ./Src/spi.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/timer.cyclo ./Src/timer.d ./Src/timer.o ./Src/timer.su ./Src/uart.cyclo ./Src/uart.d ./Src/uart.o ./Src/uart.su

.PHONY: clean-Src

