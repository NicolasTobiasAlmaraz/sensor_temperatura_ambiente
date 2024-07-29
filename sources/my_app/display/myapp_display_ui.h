/**
* @file myapp_display_ui.h
* @author Nicolás Almaraz
* @version rev 1.0
* @date 20/07/2024
* @brief API de la UI de Display (subsystem)
* 
* Se encarga de escribir los mensajes que la app quiere mostrar en el display.
* Para ello hay que primero inicializarlo (inicializa display via I2C).
* Ademas, va a inicializar los mensajes del display que nunca cambian.
*   void displayUiApi_init();
*
* Luego, para actualizar los mensajes que cambian en tiempo de ejecucion se usan
* las funciones:
*    void displayUiApi_actualizar_temp(float temp);
*    void displayUiApi_actualizar_estado(state_mde_t estado);
*    void displayUiApi_actualizar_mensaje_ropa(msg_ropa_t msg);
*/

#ifndef MYAPP_DISPLAY_UI
#define MYAPP_DISPLAY_UI

#include "myapp_main_controller.h"

/**
* @brief Tipos de mensajes posibles para escribir en el display
*/
typedef enum {
    CORTOS_REMERA,
    CORTOS_BUZO,
    LARGOS_REMERA,
    LARGOS_BUZO,
} msg_ropa_t;

/**
* @brief Inicializa el display y escribe los mensajes que no cambian
*/
void displayUiApi_init();

/**
* @brief Actualiza el valor de la temperatura
*/
void displayUiApi_actualizar_temp(float temp);

/**
* @brief Actualiza el texto del estado en el display
*/
void displayUiApi_actualizar_estado(state_mde_t estado);

/**
* @brief Actualiza el mensaje de que tan abrigado deberías estar
*/
void displayUiApi_actualizar_mensaje_ropa(msg_ropa_t msg);

#endif