/*
 * bmp280_driver.h
 *
 *  Created on: Apr 20, 2025
 *      Author: doddy
 */

#ifndef API_INC_BMP280_DRIVER_H_
#define API_INC_BMP280_DRIVER_H_

#include "stdint.h"

#define BMP280_CHIP_ID         0x58
#define BMP280_RESET_VALUE     0xB6
#define BMP280_REG_ID          0xD0
#define BMP280_REG_RESET       0xE0
#define BMP280_REG_CTRL_MEAS   0xF4
#define BMP280_REG_CONFIG      0xF5
#define BMP280_REG_PRESS_MSB   0xF7
#define BMP280_REG_TEMP_MSB    0xFA
#define BMP280_REG_CALIB_START 0x88

uint8_t BMP280_Read8(uint8_t reg);
void BMP280_Write8(uint8_t reg, uint8_t value);
void BMP280_Init(void);
void BMP280_Restart(void);
float BMP280_ReadTemperature(void);
float BMP280_ReadPressure(void);
float BMP280_ReadAltitude(float sea_level_hPa);


#endif /* API_INC_BMP280_DRIVER_H_ */
