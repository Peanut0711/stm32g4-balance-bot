################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/hw/src/imu/icm42670.c \
../App/hw/src/imu/madgwick.c 

OBJS += \
./App/hw/src/imu/icm42670.o \
./App/hw/src/imu/madgwick.o 

C_DEPS += \
./App/hw/src/imu/icm42670.d \
./App/hw/src/imu/madgwick.d 


# Each subdirectory must supply rules for building sources it contributes
App/hw/src/imu/%.o App/hw/src/imu/%.su App/hw/src/imu/%.cyclo: ../App/hw/src/imu/%.c App/hw/src/imu/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance/App/ap" -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance/App/common" -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance/App/common/include" -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance/App/hw" -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance/App/hw/include" -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance/App/ap/pid" -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance/App/ap/balance" -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance/App/ap/communication" -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance/App/ap/function" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-hw-2f-src-2f-imu

clean-App-2f-hw-2f-src-2f-imu:
	-$(RM) ./App/hw/src/imu/icm42670.cyclo ./App/hw/src/imu/icm42670.d ./App/hw/src/imu/icm42670.o ./App/hw/src/imu/icm42670.su ./App/hw/src/imu/madgwick.cyclo ./App/hw/src/imu/madgwick.d ./App/hw/src/imu/madgwick.o ./App/hw/src/imu/madgwick.su

.PHONY: clean-App-2f-hw-2f-src-2f-imu

