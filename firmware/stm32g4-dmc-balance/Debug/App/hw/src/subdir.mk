################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/hw/src/button.c \
../App/hw/src/can.c \
../App/hw/src/encoder.c \
../App/hw/src/gpio.c \
../App/hw/src/i2c.c \
../App/hw/src/imu.c \
../App/hw/src/led.c \
../App/hw/src/log.c \
../App/hw/src/motor.c \
../App/hw/src/ps2.c \
../App/hw/src/pwm.c \
../App/hw/src/servo.c \
../App/hw/src/swtimer.c \
../App/hw/src/uart.c 

OBJS += \
./App/hw/src/button.o \
./App/hw/src/can.o \
./App/hw/src/encoder.o \
./App/hw/src/gpio.o \
./App/hw/src/i2c.o \
./App/hw/src/imu.o \
./App/hw/src/led.o \
./App/hw/src/log.o \
./App/hw/src/motor.o \
./App/hw/src/ps2.o \
./App/hw/src/pwm.o \
./App/hw/src/servo.o \
./App/hw/src/swtimer.o \
./App/hw/src/uart.o 

C_DEPS += \
./App/hw/src/button.d \
./App/hw/src/can.d \
./App/hw/src/encoder.d \
./App/hw/src/gpio.d \
./App/hw/src/i2c.d \
./App/hw/src/imu.d \
./App/hw/src/led.d \
./App/hw/src/log.d \
./App/hw/src/motor.d \
./App/hw/src/ps2.d \
./App/hw/src/pwm.d \
./App/hw/src/servo.d \
./App/hw/src/swtimer.d \
./App/hw/src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
App/hw/src/%.o App/hw/src/%.su App/hw/src/%.cyclo: ../App/hw/src/%.c App/hw/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance-backup/App/ap" -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance-backup/App/common" -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance-backup/App/common/include" -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance-backup/App/hw" -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance-backup/App/hw/include" -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance-backup/App/ap/pid" -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance-backup/App/ap/balance" -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance-backup/App/ap/communication" -I"C:/work/git/stm32g4-balance-bot/firmware/stm32g4-dmc-balance-backup/App/ap/function" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-hw-2f-src

clean-App-2f-hw-2f-src:
	-$(RM) ./App/hw/src/button.cyclo ./App/hw/src/button.d ./App/hw/src/button.o ./App/hw/src/button.su ./App/hw/src/can.cyclo ./App/hw/src/can.d ./App/hw/src/can.o ./App/hw/src/can.su ./App/hw/src/encoder.cyclo ./App/hw/src/encoder.d ./App/hw/src/encoder.o ./App/hw/src/encoder.su ./App/hw/src/gpio.cyclo ./App/hw/src/gpio.d ./App/hw/src/gpio.o ./App/hw/src/gpio.su ./App/hw/src/i2c.cyclo ./App/hw/src/i2c.d ./App/hw/src/i2c.o ./App/hw/src/i2c.su ./App/hw/src/imu.cyclo ./App/hw/src/imu.d ./App/hw/src/imu.o ./App/hw/src/imu.su ./App/hw/src/led.cyclo ./App/hw/src/led.d ./App/hw/src/led.o ./App/hw/src/led.su ./App/hw/src/log.cyclo ./App/hw/src/log.d ./App/hw/src/log.o ./App/hw/src/log.su ./App/hw/src/motor.cyclo ./App/hw/src/motor.d ./App/hw/src/motor.o ./App/hw/src/motor.su ./App/hw/src/ps2.cyclo ./App/hw/src/ps2.d ./App/hw/src/ps2.o ./App/hw/src/ps2.su ./App/hw/src/pwm.cyclo ./App/hw/src/pwm.d ./App/hw/src/pwm.o ./App/hw/src/pwm.su ./App/hw/src/servo.cyclo ./App/hw/src/servo.d ./App/hw/src/servo.o ./App/hw/src/servo.su ./App/hw/src/swtimer.cyclo ./App/hw/src/swtimer.d ./App/hw/src/swtimer.o ./App/hw/src/swtimer.su ./App/hw/src/uart.cyclo ./App/hw/src/uart.d ./App/hw/src/uart.o ./App/hw/src/uart.su

.PHONY: clean-App-2f-hw-2f-src

