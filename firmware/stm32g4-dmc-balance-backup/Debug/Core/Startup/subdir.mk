################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32g431cbtx.s 

OBJS += \
./Core/Startup/startup_stm32g431cbtx.o 

S_DEPS += \
./Core/Startup/startup_stm32g431cbtx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"C:/work/git/stm32g4-dmc-balance/firmware/stm32g4-dmc-balance/App/ap" -I"C:/work/git/stm32g4-dmc-balance/firmware/stm32g4-dmc-balance/App/common" -I"C:/work/git/stm32g4-dmc-balance/firmware/stm32g4-dmc-balance/App/common/include" -I"C:/work/git/stm32g4-dmc-balance/firmware/stm32g4-dmc-balance/App/hw" -I"C:/work/git/stm32g4-dmc-balance/firmware/stm32g4-dmc-balance/App/hw/include" -I"C:/work/git/stm32g4-dmc-balance/firmware/stm32g4-dmc-balance/App/ap/pid" -I"C:/work/git/stm32g4-dmc-balance/firmware/stm32g4-dmc-balance/App/ap/balance" -I"C:/work/git/stm32g4-dmc-balance/firmware/stm32g4-dmc-balance/App/ap/communication" -I"C:/work/git/stm32g4-dmc-balance/firmware/stm32g4-dmc-balance/App/ap/function" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32g431cbtx.d ./Core/Startup/startup_stm32g431cbtx.o

.PHONY: clean-Core-2f-Startup

