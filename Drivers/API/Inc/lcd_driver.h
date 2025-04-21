/*
 * lcd_driver.h
 *
 *  Created on: Mar 30, 2025
 *      Author: doddy
 */

#ifndef API_INC_LCD_DRIVER_H_
#define API_INC_LCD_DRIVER_H_

#include "stm32f4xx_hal.h"
#include "stdint.h"



#define LCD_ADDR              0x27 << 1
#define LCD_BACKLIGHT         0x08
#define ENABLE                0x04
#define RW                    0x02

#define MODE_RS_IR            0x00
#define MODE_RS_DR            0x01
#define MASK                  0xF0


// CMD
#define LCD_CLEARDISPLAY        0x01
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAYCONTROL      0x08
#define LCD_CURSORSHIFT         0x10
#define LCD_FUNCTIONSET         0x20
#define LCD_SETCGRAMADDR        0x40
#define LCD_SETDDRAMADDR        0x80
// DISPLAY ENTRY
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00
// DISPLAY CONTROL
#define LCD_DISPLAYON           0x04
#define LCD_DISPLAYOFF          0x00
#define LCD_CURSORON            0x02
#define LCD_CURSOROFF           0x00
#define LCD_BLINKON             0x01
#define LCD_BLINKOFF            0x00
// CURSOR MOTION
#define LCD_DISPLAYMOVE         0x08
#define LCD_CURSORMOVE          0x00
#define LCD_MOVERIGHT           0x04
#define LCD_MOVELEFT            0x00
// FUNCTION SET
#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_1LINE               0x00
#define LCD_5x10DOTS            0x04
#define LCD_5x8DOTS             0x00

// LINE ADDRESS
#define LCD_LINE_0              0x00
#define LCD_LINE_1              0x40
#define LCD_LINE_2              0x14
#define LCD_LINE_3              0x54

#define FUNCTION_SET_8BIT       0x30

typedef struct
{
	uint8_t I2C_LCD_nCol;
	uint8_t I2C_LCD_nRow;
} I2C_LCD_Conf;

extern void Error_Handler(void);

void LCD_Begin(uint8_t cols, uint8_t row);
void LCD_SendString(char *str);
void LCD_Clear(void);
void LCD_Home(void);
void LCD_SetCursor(uint8_t col, uint8_t row);
void LCD_Print(char *str);
void LCD_PrintSensorData(int16_t temp_x100, int16_t gx_x100, int16_t ax_x100);

#endif /* API_INC_LCD_DRIVER_H_ */
