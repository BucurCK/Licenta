################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Encoder.c \
../current_protection.c \
../driver_adc.c \
../driver_pwm.c \
../io.c \
../kernel_interrupt.c \
../logger.c \
../main.c \
../motor_data_update.c \
../pi_regulator.c \
../probe_scope.c \
../reference_generator.c \
../state_machine.c \
../timer.c \
../transform.c 

OBJS += \
./Encoder.o \
./current_protection.o \
./driver_adc.o \
./driver_pwm.o \
./io.o \
./kernel_interrupt.o \
./logger.o \
./main.o \
./motor_data_update.o \
./pi_regulator.o \
./probe_scope.o \
./reference_generator.o \
./state_machine.o \
./timer.o \
./transform.o 

C_DEPS += \
./Encoder.d \
./current_protection.d \
./driver_adc.d \
./driver_pwm.d \
./io.d \
./kernel_interrupt.d \
./logger.d \
./main.d \
./motor_data_update.d \
./pi_regulator.d \
./probe_scope.d \
./reference_generator.d \
./state_machine.d \
./timer.d \
./transform.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4800_F144x2048 -I"$(PROJECT_LOC)/Libraries/XMCLib/inc" -I"$(PROJECT_LOC)/Libraries/CMSIS/Include" -I"$(PROJECT_LOC)/Libraries/CMSIS/Infineon/XMC4800_series/Include" -I"$(PROJECT_LOC)" -I"$(PROJECT_LOC)/Libraries" -O0 -ffunction-sections -fdata-sections -Wall -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.

