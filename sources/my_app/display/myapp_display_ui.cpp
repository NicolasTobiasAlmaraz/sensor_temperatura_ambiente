/**
* @file myapp_display_ui.cpp
* @brief Implementaciones de la API de display (subsystem)
* @author Nicolás Almaraz
* @version rev 1.0
* @date 20/07/2024
*/

//====================================================================
// Dependencias
//====================================================================
#include "string.h"

#include "mbed.h"

#include "i2clcd.h"
#include "myapp_display_ui.h"
#include "myapp_main_controller.h"

//====================================================================
// Defines Privados
//====================================================================

#define CORTOS_REMERA_MSG   "Cortos y remera"   /*!< Mensaje para transicion */
#define CORTOS_BUZO_MSG     "Cortos y Buzo"     /*!< Mensaje para transicion */
#define LARGOS_REMERA_MSG   "Largos y remera"   /*!< Mensaje para transicion */
#define LARGOS_BUZO_MSG     "Largos y Buzo"     /*!< Mensaje para transicion */

#define LINE_0_INIT         "Nicolas Almaraz-TP2"   /*!< Mensaje display */
#define LINE_1_INIT         "Esta ?"            /*!< Mensaje display */
#define LINE_2_INIT         "Temp: XX'C -Ponete:"    /*!< Mensaje display */
#define LINE_3_INIT         CORTOS_REMERA_MSG       /*!< Mensaje display */

#define MSG_FRIO            "Hace frio"     /*!< Mensaje display */
#define MSG_NORMAL          "Esta templado" /*!< Mensaje display */
#define MSG_CALUROSO        "Hace calor"    /*!< Mensaje display */

//=======================================================
// Tipos de datos privados
//=======================================================


//=======================================================
// Estructuras privadas
//=======================================================


//=======================================================
// Declaracion de funciones privadas
//=======================================================

/**
* @brief Escribe el renglon 0 del display
* @param str String a escribir
*/
static void displayUiApi_writeLine0(const char *str);

/**
* @brief Escribe el renglon 1 del display
* @param str String a escribir
*/
static void displayUiApi_writeLine1(const char *str);

/**
* @brief Escribe el renglon 2 del display
* @param str String a escribir
*/
static void displayUiApi_writeLine2(const char *str);

/**
* @brief Escribe el renglon 3 del display
* @param str String a escribir
*/
static void displayUiApi_writeLine3(const char *str);

//=======================================================
// Implementacion de funciones publicas
//=======================================================

/**
* @brief Inicializa el display y escribe los mensajes que no cambian
*/
void displayUiApi_init() {
    //Inicializo display
    lcd_init();

    //Limpio display
    lcd_clear();

    //Escribo el template pantalla que usa mi app
    displayUiApi_writeLine0(LINE_0_INIT);
    displayUiApi_writeLine1(LINE_1_INIT);
    displayUiApi_writeLine2(LINE_2_INIT);
    displayUiApi_writeLine3(LINE_3_INIT);

    printf("Init display ui api\r\n");
}

/**
* @brief Actualiza el valor de la temperatura
*/
void displayUiApi_actualizar_temp(float temp) {
    char temp_decena = (uint8_t) temp/10;
    char temp_unidad = (uint8_t) temp%10;

    //Copio string
    char msg[20] = {0};
    for(int i=0; i<strlen(LINE_2_INIT);i++) {
        msg[i] = LINE_2_INIT[i];
    }

    //Pego temperatura
    msg[6] = temp_decena+'0';
    msg[7] = temp_unidad+'0';

    //Escribo renglon
    displayUiApi_writeLine2(msg);
}

/**
* @brief Actualiza el texto del estado en el display
*/
void displayUiApi_actualizar_estado(state_mde_t estado) {
    switch(estado) {
        default:
        case CALUROSO:
            displayUiApi_writeLine1(MSG_CALUROSO);
            break;
        case FRIO:
            displayUiApi_writeLine1(MSG_FRIO);
        break;
        case NORMAL:
            displayUiApi_writeLine1(MSG_NORMAL);
        break;
    }
}

/**
* @brief Actualiza el mensaje de que tan abrigado deberías estar
*/
void displayUiApi_actualizar_mensaje_ropa(msg_ropa_t msg) {
    switch(msg) {
        case CORTOS_REMERA:
            displayUiApi_writeLine3(CORTOS_REMERA_MSG);
            break;
        
        default:
        case CORTOS_BUZO:
            displayUiApi_writeLine3(CORTOS_BUZO_MSG);
            break;
        
        case LARGOS_REMERA:
            displayUiApi_writeLine3(LARGOS_REMERA_MSG);
            break;
        
        case LARGOS_BUZO:
            displayUiApi_writeLine3(LARGOS_BUZO_MSG);
            break;
        
    }
}

//=======================================================
// Implementacion de funciones privadas
//=======================================================

static void displayUiApi_writeLine0(const char *str) {
    lcd_write(str, 0, 0);
}

static void displayUiApi_writeLine1(const char *str) {
    lcd_write(str, 1, 0);
}

static void displayUiApi_writeLine2(const char *str) {
    lcd_write(str, 2, 0);
}

static void displayUiApi_writeLine3(const char *str) {
    lcd_write(str, 3, 0);
}
