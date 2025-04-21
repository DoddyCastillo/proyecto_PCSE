################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/API/Src/API_uart.c \
../Drivers/API/Src/bmp280_driver.c \
../Drivers/API/Src/bmp280_port.c \
../Drivers/API/Src/lcd_driver.c \
../Drivers/API/Src/lcd_port.c \
../Drivers/API/Src/mpu6050_driver.c \
../Drivers/API/Src/mpu6050_port.c 

OBJS += \
./Drivers/API/Src/API_uart.o \
./Drivers/API/Src/bmp280_driver.o \
./Drivers/API/Src/bmp280_port.o \
./Drivers/API/Src/lcd_driver.o \
./Drivers/API/Src/lcd_port.o \
./Drivers/API/Src/mpu6050_driver.o \
./Drivers/API/Src/mpu6050_port.o 

C_DEPS += \
./Drivers/API/Src/API_uart.d \
./Drivers/API/Src/bmp280_driver.d \
./Drivers/API/Src/bmp280_port.d \
./Drivers/API/Src/lcd_driver.d \
./Drivers/API/Src/lcd_port.d \
./Drivers/API/Src/mpu6050_driver.d \
./Drivers/API/Src/mpu6050_port.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/API/Src/%.o Drivers/API/Src/%.su Drivers/API/Src/%.cyclo: ../Drivers/API/Src/%.c Drivers/API/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I"D:/UBA/proyecto/Drivers/API/Inc" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-API-2f-Src

clean-Drivers-2f-API-2f-Src:
	-$(RM) ./Drivers/API/Src/API_uart.cyclo ./Drivers/API/Src/API_uart.d ./Drivers/API/Src/API_uart.o ./Drivers/API/Src/API_uart.su ./Drivers/API/Src/bmp280_driver.cyclo ./Drivers/API/Src/bmp280_driver.d ./Drivers/API/Src/bmp280_driver.o ./Drivers/API/Src/bmp280_driver.su ./Drivers/API/Src/bmp280_port.cyclo ./Drivers/API/Src/bmp280_port.d ./Drivers/API/Src/bmp280_port.o ./Drivers/API/Src/bmp280_port.su ./Drivers/API/Src/lcd_driver.cyclo ./Drivers/API/Src/lcd_driver.d ./Drivers/API/Src/lcd_driver.o ./Drivers/API/Src/lcd_driver.su ./Drivers/API/Src/lcd_port.cyclo ./Drivers/API/Src/lcd_port.d ./Drivers/API/Src/lcd_port.o ./Drivers/API/Src/lcd_port.su ./Drivers/API/Src/mpu6050_driver.cyclo ./Drivers/API/Src/mpu6050_driver.d ./Drivers/API/Src/mpu6050_driver.o ./Drivers/API/Src/mpu6050_driver.su ./Drivers/API/Src/mpu6050_port.cyclo ./Drivers/API/Src/mpu6050_port.d ./Drivers/API/Src/mpu6050_port.o ./Drivers/API/Src/mpu6050_port.su

.PHONY: clean-Drivers-2f-API-2f-Src

