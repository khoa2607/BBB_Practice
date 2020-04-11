################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Rev3/TeensyTalkativeWriteRead.c 

OBJS += \
./Rev3/TeensyTalkativeWriteRead.o 

C_DEPS += \
./Rev3/TeensyTalkativeWriteRead.d 


# Each subdirectory must supply rules for building sources it contributes
Rev3/%.o: ../Rev3/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


