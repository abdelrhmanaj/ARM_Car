################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/HDAC_program.c \
../src/HLEDMATRIX_program.c \
../src/HSTP_program.c \
../src/MEXTI_program.c \
../src/MGPIO_program.c \
../src/MNVIC_program.c \
../src/MRCC_program.c \
../src/MSPI_program.c \
../src/MSYSTICK_program.c \
../src/OS_program.c \
../src/main.c \
../src/smart_car_controller.c \
../src/stm32f4xx_hal_msp.c 

OBJS += \
./src/HDAC_program.o \
./src/HLEDMATRIX_program.o \
./src/HSTP_program.o \
./src/MEXTI_program.o \
./src/MGPIO_program.o \
./src/MNVIC_program.o \
./src/MRCC_program.o \
./src/MSPI_program.o \
./src/MSYSTICK_program.o \
./src/OS_program.o \
./src/main.o \
./src/smart_car_controller.o \
./src/stm32f4xx_hal_msp.o 

C_DEPS += \
./src/HDAC_program.d \
./src/HLEDMATRIX_program.d \
./src/HSTP_program.d \
./src/MEXTI_program.d \
./src/MGPIO_program.d \
./src/MNVIC_program.d \
./src/MRCC_program.d \
./src/MSPI_program.d \
./src/MSYSTICK_program.d \
./src/OS_program.d \
./src/main.d \
./src/smart_car_controller.d \
./src/stm32f4xx_hal_msp.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"D:/ARM_ITI/workspace/ARM/include" -I"D:/ARM_ITI/workspace/ARM/system/include" -I"D:/ARM_ITI/workspace/ARM/system/include/cmsis" -I"D:/ARM_ITI/workspace/ARM/system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/stm32f4xx_hal_msp.o: ../src/stm32f4xx_hal_msp.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"D:/ARM_ITI/workspace/ARM/include" -I"D:/ARM_ITI/workspace/ARM/system/include" -I"D:/ARM_ITI/workspace/ARM/system/include/cmsis" -I"D:/ARM_ITI/workspace/ARM/system/include/stm32f4-hal" -std=gnu11 -Wno-missing-prototypes -Wno-missing-declarations -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/stm32f4xx_hal_msp.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


