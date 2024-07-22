/**
* @file           : i2clcd.h
* @brief          : Library for I2C LCD display with mbedOS
* @author controllerstech, 2019-05-29
* https://controllerstech.com/lcd-20x4-using-i2c-with-stm32/
*
* @author (modified) Nicolas Almaraz, 2024-07-21
*
* Note Nicolas Almaraz:
* I have adapted this API for use within the mbed framework
*
*/

#ifndef I2CLCD_H
#define I2CLCD_H

//=======================================================
// Dependencias
//=======================================================
#include "mbed.h"

//=======================================================
// Declaracion de funciones publicas
//=======================================================

void lcd_init(void); // initialize lcd

void lcd_send_cmd(char cmd); // send command to the lcd

void lcd_send_data(char data); // send data to the lcd

void lcd_send_string(const char *str); // send string to the lcd

void lcd_write(const char *txt, uint8_t line, uint8_t column); // write string to specific line

void lcd_clear(void);

void lcd_clear_line(uint8_t line, uint8_t column);

#endif