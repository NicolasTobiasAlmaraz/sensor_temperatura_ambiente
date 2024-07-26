/**
* @file myapp_led.h
* @brief Esta API se encarga del manejo del led
*
* Esta API se encarga del manejo del led. Para eso cuenta con los métodos set:
* void ledApi_setCaluroso();
* void ledApi_setNormal();
* void ledApi_setFrio();
*
* Por hardware los Leds son activos por 0v
*/

#ifndef MYAPP_LEDS_H
#define MYAPP_LEDS_H

// ==================== Dependencias

#include "mbed.h"

//==================================================================== 
// Defines publicos
//==================================================================== 

//==================================================================== 
// Tipos de datos publicos
//==================================================================== 

//==================================================================== 
// Estructuras publicas
//==================================================================== 

//====================================================================
// Estructuras publicas
//==================================================================== 

//TODO: Le puse uint8_t para poder escalarse en el futuro a un PWM almacenando el duty de cada color
/**
*@brief Contiene un color rgb
*/
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} rgb_color_t;

//====================================================================
// Declaracion de funciones publicas
//==================================================================== 

/**
* @brief Inicializa el led en apagado
*/
void ledApi_init();

/**
* @brief Setea color rojo
*/
void ledApi_setCaluroso();

/**
* @brief Setea color naranja
*/
void ledApi_setNormal();

/**
* @brief Setea color celeste
*/
void ledApi_setFrio();

#endif