/*
 * lcd_port.h
 *
 *  Created on: Apr 13, 2025
 *      Author: doddy
 */

#ifndef API_INC_LCD_PORT_H_
#define API_INC_LCD_PORT_H_

#include "stm32f4xx_hal.h"
#include "lcd_driver.h"
#include "stdint.h"

extern void Error_Handler(void);

void LCD_PortI2C_Init();
void LCD_PortI2C_Isready();
void LCD_PortI2C_WriteRegister(uint8_t  valor);
void BMP280_SPI_CS_Select(void);
void BMP280_SPI_CS_Deselect(void);


#endif /* API_INC_LCD_PORT_H_ */
