/**
* @file myapp_main_controller.cpp
* @brief Implementaciones de la API de main controller
* @author Nicolás Almaraz
* @version rev 1.0
* @date 28/06/2024
*/

//====================================================================
// Dependencias
//====================================================================
#include "mbed.h"

#include "myapp_main_controller.h"
#include "myapp_led.h"
#include "myapp_display_ui.h"
#include "myapp_sensor_temp.h"

//==================================================================== 
// Defines Privados
//==================================================================== 

#define TEMP_CALUROSO_A_NORMAL  20  /*<Para pasar de caluroso a normal la temperatura debe bajar de 20°C*/
#define TEMP_NORMAL_A_CALUROSO  25  /*<Para pasar de normal a caluroso la temperatura debe subir de 25°C*/
#define TEMP_NORMAL_A_FRIO      10  /*<Para pasar de normal a frio la temperatura debe bajar de 10°C*/
#define TEMP_FRIO_A_NORMAL      15  /*<Para pasar de frio a normal la temperatura debe subir de 15°C*/

#define TIEMPO_DELAY_MS         500 /*<Tiempo de muestreo sensor y update pantalla*/

//====================================================================
// Tipos de datos privados
//==================================================================== 

//====================================================================
// Estructuras privadas
//==================================================================== 

//====================================================================
// Declaracion de funciones privadas
//==================================================================== 

//====================================================================
// Implementacion de funciones publicas
//==================================================================== 

/**
 * @brief Inicializa todos los perifericos y recursos de hardware
 */
void mainControllerApi_init() {
    ledApi_init();
    displayUiApi_init();
    sensorTempApi_init();
    
    printf("Init Main Controller\r\n");
}

/**
 * @brief Contiene la aplicacion
 */
void mainControllerApi_loop() {
    static state_mde_t state = CALUROSO;
    
    //Leo temperatura
    int temp = sensorTempApi_read();
    
    //Maquina de estado sistema
    switch(state) {
        default:
        case CALUROSO:
            ledApi_setCaluroso();
            if(temp<TEMP_CALUROSO_A_NORMAL) {
                displayUiApi_actualizar_mensaje_ropa(LARGOS_REMERA);
                state = NORMAL;
            }
            break;
        case NORMAL:
            ledApi_setNormal();
            if(temp>TEMP_NORMAL_A_CALUROSO) {
                displayUiApi_actualizar_mensaje_ropa(CORTOS_REMERA);
                state = CALUROSO;
            }
            if(temp<TEMP_NORMAL_A_FRIO) {
                displayUiApi_actualizar_mensaje_ropa(LARGOS_BUZO);
                state = FRIO;
            }
            break;
        case FRIO:
            ledApi_setFrio();
            if(temp>TEMP_FRIO_A_NORMAL) {
                displayUiApi_actualizar_mensaje_ropa(CORTOS_BUZO);
                state = NORMAL;
            }
            break;
    }

    //Actualizo display
    displayUiApi_actualizar_temp(temp);
    displayUiApi_actualizar_estado(state);

    //Espero tiempo de muestreo
    thread_sleep_for(TIEMPO_DELAY_MS);
}

//====================================================================
// Implementacion de funciones privadas
//==================================================================== 
