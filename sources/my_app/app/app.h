/**
* @file app.h
* @brief API App
* @author Nicolás Almaraz
* @version rev 1.0
* @date 28/06/2024
*
* Esta API se encarga de ejecutar la aplicación en general haciendo uso del resto de APIs.
* Para ello cuenta con dos funciones:
* void app_init()
* void app_loop()
*/

#ifndef MYAPP_MAIN_CONTROLLER_H
#define MYAPP_MAIN_CONTROLLER_H

//==================================================================== 
// Defines publicos
//==================================================================== 

//==================================================================== 
// Tipos de datos publicos
//==================================================================== 

//==================================================================== 
// Funciones publicas
//==================================================================== 

/**
* @brief Inicializa a todos los perifericos necesarios para esta App
*/
void app_init();

/**
* @brief Contiene la aplicación general
*/
void app_loop();

#endif