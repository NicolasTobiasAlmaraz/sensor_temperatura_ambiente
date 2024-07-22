/**
* @file myapp_main_controller.h
* @brief API Main Controller
* @author Nicolás Almaraz
* @version rev 1.0
* @date 28/06/2024
*
* Esta API se encarga de ejecutar la aplicación en general haciendo uso del resto de APIs.
* Para ello cuenta con dos funciones:
* void mainControllerApi_init()
* void mainControllerApi_loop()
*/

#ifndef MYAPP_MAIN_CONTROLLER_H
#define MYAPP_MAIN_CONTROLLER_H

//==================================================================== 
// Defines publicos
//==================================================================== 

//==================================================================== 
// Tipos de datos publicos
//==================================================================== 

/**
* @brief tipo de dato con los posibles estados de la mde general
*/
typedef enum {
    CALUROSO=0,
    FRIO,
    NORMAL,
} state_mde_t;

//==================================================================== 
// Funciones publicas
//==================================================================== 

/**
* @brief: Inicializa a todos los perifericos necesarios para esta App
*/
void mainControllerApi_init();

/**
* @brief: Contiene la aplicación general
*/
void mainControllerApi_loop();

#endif