/**
* @file myapp_led.cpp
* @brief Implementaciones de la API LEDs
* @author Nicolás Almaraz
* @version rev 1.0
* @date 18/07/2024
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
* @brief: Enciende el led rojo
*/
static void ledsApi_prenderRojo();

/**
* @brief: Enciende el led verde
*/
static void ledsApi_prenderVerde();

/**
* @brief: Enciende el led azul
*/
static void ledsApi_prenderAzul();

/**
* @brief: Apaga el led rojo
*/
static void ledsApi_apagarRojo();

/**
* @brief: Apaga el led verde
*/
static void ledsApi_apagarVerde();

/**
* @brief: Apaga el led azul
*/
static void ledsApi_apagarAzul();

//==================================================================== 
// Implementacion de funciones  publicas
//==================================================================== 
 
void ledApi_setCaluroso() {
    ledsApi_prenderRojo();
    ledsApi_apagarVerde();
    ledsApi_apagarAzul();
}

void ledApi_setNormal() {
    ledsApi_apagarRojo();
    ledsApi_prenderVerde();
    ledsApi_apagarAzul();
}

void ledApi_setFrio(){
    ledsApi_apagarRojo();
    ledsApi_apagarVerde();
    ledsApi_prenderAzul();
}

void ledApi_init() {
    ledsApi_apagarRojo();
    ledsApi_apagarVerde();
    ledsApi_apagarAzul();

    printf("Init Api Led\r\n");
}

//====================================================================
// Implementacion de funciones privadas
//==================================================================== 

static void ledsApi_prenderRojo() {
    g_led_red = false;
    g_config_leds.red = 1;
}

static void ledsApi_prenderVerde() {
    g_led_green = false;
    g_config_leds.green = 1;
}

static void ledsApi_prenderAzul() {
    g_led_blue = false;
    g_config_leds.blue = 1;
}

static void ledsApi_apagarRojo() {
    g_led_red = true;
    g_config_leds.red = 0;
}

static void ledsApi_apagarVerde() {
    g_led_green = true;
    g_config_leds.green = 0;
}

static void ledsApi_apagarAzul() {
    g_led_blue = true;
    g_config_leds.blue = 0;
}