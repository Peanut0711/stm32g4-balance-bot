################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/ap/ap.c 

OBJS += \
./App/ap/ap.o 

C_DEPS += \
./App/ap/ap.d 


# Each subdirectory must supply rules for building sources it contributes
App/ap/%.o App/ap/%.su App/ap/%.cyclo: ../App/ap/%.c App/ap/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/work/git/stm32g4-dmc-balance/firmware/stm32g4-dmc-balance/App/ap" -I"C:/work/git/stm32g4-dmc-balance/firmware/stm32g4-dmc-balance/App/common" -I"C:/work/git/stm32g4-dmc-balance/firmware/stm32g4-dmc-balance/App/common/include" -I"C:/work/git/stm32g4-dmc-balance/firmware/stm32g4-dmc-balance/App/hw" -I"C:/work/git/stm32g4-dmc-balance/firmware/stm32g4-dmc-balance/App/hw/include" -I"C:/work/git/stm32g4-dmc-balance/firmware/stm32g4-dmc-balance/App/ap/pid" -I"C:/work/git/stm32g4-dmc-balance/firmware/stm32g4-dmc-balance/App/ap/balance" -I"C:/work/git/stm32g4-dmc-balance/firmware/stm32g4-dmc-balance/App/ap/communication" -I"C:/work/git/stm32g4-dmc-balance/firmware/stm32g4-dmc-balance/App/ap/function" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-ap

clean-App-2f-ap:
	-$(RM) ./App/ap/ap.cyclo ./App/ap/ap.d ./App/ap/ap.o ./App/ap/ap.su

.PHONY: clean-App-2f-ap

