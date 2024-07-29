/**
* @file myapp_sensor_temp.cpp
* @brief Implementaciones API de sensor temperatura (subsystem)
* @author Nicolás Almaraz
* @version rev 1.0
* @date 28/06/2024
*/

//==================================================================== 
// Dependencias
//==================================================================== 
#include <stdbool.h>

#include "DHT.h"

#include "myapp_sensor_temp.h"

//==================================================================== 
// Defines privados
//==================================================================== 
#define LEN_MED_MOV 5               //< Largo del vector de media movil 
#define GPIO_SENSOR ARDUINO_UNO_D7  //< Pin de datos del sensor de temperatura

//==================================================================== 
// Tipos de datos privados
//==================================================================== 

//==================================================================== 
// Estructuras privadas
//==================================================================== 

//==================================================================== 
// Variables privadas
//==================================================================== 

/**
* @brief Objeto handler del sensor de temperatura;  
*/
static DHT g_dht22(GPIO_SENSOR,DHT::DHT22);  
    
//====================================================================
// Declaracion de funciones privadas
//==================================================================== 

/**
* @brief Calcula una media movil con las ultimas LEN_MED_MOV muestrsas
* @param muestra Muestra
* @retval Media movil
*/
static float sensorTempApi_getMedMov(float muestra);

/**
* @brief Lee una muestra del sensor de temperatura
* @retval Muestra
*/
static float sensorTempApi_read_sensor();

//====================================================================
// Implementacion de funciones publicas
//==================================================================== 

void sensorTempApi_init() {    
    //Inicializo muestras
    for(uint8_t i=0 ; i<LEN_MED_MOV; i++)
        sensorTempApi_read();
        
    printf("Init sensor temp api\r\n");
}

float sensorTempApi_read() {
    int temp = sensorTempApi_read_sensor();
    int temp_med_mov = sensorTempApi_getMedMov(temp);
    return temp_med_mov;
}

//====================================================================
// Implementacion de funciones privadas
//==================================================================== 

static float sensorTempApi_read_sensor() {
    g_dht22.read();
    int temp = g_dht22.getTemperature();
    return temp; 
}

static float sensorTempApi_getMedMov(float muestra) {
    static float muestras[LEN_MED_MOV];
    static uint8_t index=0;
    
    //Guardo muestra
    muestras[index] = muestra;
    index++;
    index %= LEN_MED_MOV;

    //Calculo med mov
    float suma = 0;
    for(uint8_t i=0; i<LEN_MED_MOV; i++) {
        suma += muestras[i];
    }

    //Retorno promedio
    return suma/LEN_MED_MOV;
}

