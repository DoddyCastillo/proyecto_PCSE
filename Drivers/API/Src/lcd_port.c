/*
 * lcd_port.c
 *
 *  Created on: Apr 13, 2025
 *      Author: doddy
 */


#include "lcd_port.h"
#include "API_uart.h"

static I2C_HandleTypeDef hi2c1;

void LCD_PortI2C_Init()
{
	  hi2c1.Instance = I2C1;
	  hi2c1.Init.ClockSpeed = 100000;
	  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	  hi2c1.Init.OwnAddress1 = 0;
	  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	  hi2c1.Init.OwnAddress2 = 0;
	  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

void LCD_PortI2C_Isready()
{
	if(HAL_I2C_IsDeviceReady(&hi2c1, LCD_ADDR, 1, HAL_MAX_DELAY) != HAL_OK) Error_Handler();
}

void LCD_PortI2C_WriteRegister(uint8_t valor)
{
	if(HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, &valor, sizeof(valor), HAL_MAX_DELAY) != HAL_OK){
		uartSendString((uint8_t*)"ERROR HANDLER LCD WRITE!\r\n");
		Error_Handler();
	}
}
