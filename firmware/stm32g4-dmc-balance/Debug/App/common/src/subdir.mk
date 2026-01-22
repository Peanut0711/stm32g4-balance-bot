################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/common/src/cli.c \
../App/common/src/cli_gui.c \
../App/common/src/qbuffer.c \
../App/common/src/util.c 

OBJS += \
./App/common/src/cli.o \
./App/common/src/cli_gui.o \
./App/common/src/qbuffer.o \
./App/common/src/util.o 

C_DEPS += \
./App/common/src/cli.d \
./App/common/src/cli_gui.d \
./App/common/src/qbuffer.d \
./App/common/src/util.d 


# Each subdirectory must supply rules for building sources it contributes
App/common/src/%.o App/common/src/%.su App/common/src/%.cyclo: ../App/common/src/%.c App/common/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance/App/ap" -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance/App/common" -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance/App/common/include" -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance/App/hw" -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance/App/hw/include" -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance/App/ap/pid" -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance/App/ap/balance" -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance/App/ap/communication" -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance/App/ap/function" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-common-2f-src

clean-App-2f-common-2f-src:
	-$(RM) ./App/common/src/cli.cyclo ./App/common/src/cli.d ./App/common/src/cli.o ./App/common/src/cli.su ./App/common/src/cli_gui.cyclo ./App/common/src/cli_gui.d ./App/common/src/cli_gui.o ./App/common/src/cli_gui.su ./App/common/src/qbuffer.cyclo ./App/common/src/qbuffer.d ./App/common/src/qbuffer.o ./App/common/src/qbuffer.su ./App/common/src/util.cyclo ./App/common/src/util.d ./App/common/src/util.o ./App/common/src/util.su

.PHONY: clean-App-2f-common-2f-src

