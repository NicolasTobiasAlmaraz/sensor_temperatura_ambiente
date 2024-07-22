/**
* @file myapp_sensor_temp.h
* @brief API de sensor temperatura
* @author Nicolás Almaraz
* @version rev 1.0
* @date 18/07/2024
*
* Para ello utiliza una conexión One Wire. Para muestrear se hace una media movil con las ultimas
* LEN_MED_MOV muestras
*/

#ifndef MYAPP_LM35_H
#define MYAPP_LM35_H

//====================================================================
// Dependencias
//==================================================================== 

#include "mbed.h"

//====================================================================
// Defines Publicos
//==================================================================== 

//====================================================================
// Tipos de datos publicos
//==================================================================== 

//====================================================================
// Estructuras publicas
//==================================================================== 

//====================================================================
// Declaracion de funciones publicas
//==================================================================== 

/**
*@brief Toma LEN_MED_MOV muestras para entrar en régimen el vector
*/
void sensorTempApi_init();

/**
* @brief Retorna el valor de la medición en °C de promediando las ultimas LEN_MED_MOV muestras
* @retval medicion
*/
float sensorTempApi_read();

#endif