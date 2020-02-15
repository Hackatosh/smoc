################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Src/usbh_cdc.c 

OBJS += \
./Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Src/usbh_cdc.o 

C_DEPS += \
./Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Src/usbh_cdc.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Src/usbh_cdc.o: ../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Src/usbh_cdc.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -IDrivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -IMiddlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -IDrivers/STM32F4xx_HAL_Driver/Inc/Legacy -IMiddlewares/Third_Party/LwIP/src/include/lwip/prot -IMiddlewares/Third_Party/LwIP/system/arch -IMiddlewares/Third_Party/LwIP/src/include/lwip -IMiddlewares/Third_Party/LwIP/src/include -IDrivers/STM32F4xx_HAL_Driver/Inc -IMiddlewares/Third_Party/LwIP/src/include/lwip/priv -IMiddlewares/Third_Party/LwIP/src/include/netif/ppp -IMiddlewares/Third_Party/LwIP/src/apps/httpd -IMiddlewares/Third_Party/LwIP/src/include/lwip/apps -IMiddlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -IMiddlewares/Third_Party/LwIP/src/include/posix -I../Drivers/STM32F4xx_HAL_Driver/Inc -IDrivers/CMSIS/Device/ST/STM32F4xx/Include -IMiddlewares/Third_Party/LwIP/system -I../Src -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Inc -IMiddlewares/Third_Party/LwIP/src/include/posix/sys -I../Drivers/CMSIS/Include -IMiddlewares/Third_Party/LwIP/src/include/netif -IInc -I"C:/DevEnv/STM32Projects/Accelerometer/Drivers/BSP/STM32F4-Discovery" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Src/usbh_cdc.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

