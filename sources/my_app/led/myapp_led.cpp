/**
* @file myapp_led.cpp
* @brief Implementaciones de la API LEDs
* @author Nicolás Almaraz
* @version rev 2.0
* @date 24/07/2024
*/

//====================================================================
// Dependencias
//==================================================================== 
#include "mbed.h"

#include "myapp_led.h"

//====================================================================
// Defines privados
//==================================================================== 
#define GPIO_LED_RED D2     ///< GPIO del LED rojo
#define GPIO_LED_GREEN D3   ///< GPIO del LED verde
#define GPIO_LED_BLUE D4    ///< GPIO del LED azul

//====================================================================
// Tipos de datos privados
//==================================================================== 


//====================================================================
// Estructuras privadas
//==================================================================== 


//==================================================================== 
// Variables privadas API LED
//==================================================================== 

/**
* @brief: Objeto handler del led rojo
*/
static DigitalOut g_led_red(GPIO_LED_RED);

/**
* @brief: Objeto handler del led verde
*/
static DigitalOut g_led_green(GPIO_LED_GREEN);

/**
* @brief: Objeto handler del led azul
*/
static DigitalOut g_led_blue(GPIO_LED_BLUE);

/**
* @brief: Variable global que almacena la configuración actual del LED
*/
static rgb_color_t g_config_leds;


//==================================================================== 
// Declaracion de funciones privadas
//==================================================================== 

/**
* @brief: Enciende o apaga el led rojo
* @param estado true: encendido, false: apagado
*/
static void ledsApi_setRojo();

/**
* @brief: Enciende o apaga el led verde
* @param estado true: encendido, false: apagado
*/
static void ledsApi_setVerde();

/**
* @brief: Enciende o apaga el led azul
* @param estado true: encendido, false: apagado
*/
static void ledsApi_setAzul();

//==================================================================== 
// Implementacion de funciones  publicas
//==================================================================== 
 
void ledApi_setCaluroso() {
    ledsApi_setRojo(true);
    ledsApi_setVerde(false);
    ledsApi_setAzul(false);
}

void ledApi_setNormal() {
    ledsApi_setRojo(false);
    ledsApi_setVerde(true);
    ledsApi_setAzul(false);
}

void ledApi_setFrio(){
    ledsApi_setRojo(false);
    ledsApi_setVerde(false);
    ledsApi_setAzul(true);
}

void ledApi_init() {
    ledsApi_setRojo(false);
    ledsApi_setVerde(false);
    ledsApi_setAzul(false);

    printf("Init Api Led\r\n");
}

//====================================================================
// Implementacion de funciones privadas
//==================================================================== 

static void ledsApi_setRojo(bool estado) {
    g_led_red = !estado;
    g_config_leds.red = estado;
}

static void ledsApi_prenderVerde(bool estado) {
    g_led_green = !estado;
    g_config_leds.green = estado;
}

static void ledsApi_setAzul(bool estado) {
    g_led_blue = !estado;
    g_config_leds.blue = estado;
}