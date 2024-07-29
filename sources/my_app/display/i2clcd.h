/**
* @file i2clcd.h
* @brief Library for I2C LCD display with mbedOS
* @author controllerstech, 2019-05-29
* https://controllerstech.com/lcd-20x4-using-i2c-with-stm32/
*
* @author (modified) Nicolas Almaraz, 2024-07-21
*
* Note Nicolas Almaraz:
* I have adapted this API for use within the mbed framework
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

/**
* @brief Inciializa LCD
*/
void lcd_init(void); 

/**
* @brief posicion el cursor en el lugar indicado y escribe un string
* @param txt String a escribir
* @param line Renglon
* @param column Posicion
* @warning Si se termina el renglón no se asegura el correcto funcionamiento
*/
void lcd_write(const char *txt, uint8_t line, uint8_t column);

/**
* @brief Borra todo el display
*/
void lcd_clear(void);

#endif