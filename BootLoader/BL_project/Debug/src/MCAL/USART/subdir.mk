################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/MCAL/USART/dUART.c 

OBJS += \
./src/MCAL/USART/dUART.o 

C_DEPS += \
./src/MCAL/USART/dUART.d 


# Each subdirectory must supply rules for building sources it contributes
src/MCAL/USART/%.o: ../src/MCAL/USART/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"K:\GP\repo2\rep\BootLoader\BL_project\src\Application" -I"K:\GP\repo2\rep\BootLoader\BL_project\src\ECUAL\RCC_Handler" -I"K:\GP\repo2\rep\BootLoader\BL_project\src\ECUAL\USART_Handler" -I"K:\GP\repo2\rep\BootLoader\BL_project\src\Libraries" -I"K:\GP\repo2\rep\BootLoader\BL_project\src\Libraries\ARM_Delay" -I"K:\GP\repo2\rep\BootLoader\BL_project\src\MCAL\Flash" -I"K:\GP\repo2\rep\BootLoader\BL_project\src\MCAL\GPIO" -I"K:\GP\repo2\rep\BootLoader\BL_project\src\MCAL\NVIC" -I"K:\GP\repo2\rep\BootLoader\BL_project\src\MCAL\RCC" -I"K:\GP\repo2\rep\BootLoader\BL_project\src\MCAL\USART" -I"K:\GP\repo2\rep\BootLoader\BL_project\src\Service\TransportProtocol" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


