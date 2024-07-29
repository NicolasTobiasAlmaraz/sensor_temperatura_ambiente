/**
* @file main.cpp
* @brief Punto de entrada
* @author Nicolás Almaraz
* @version rev 1.0
* @date 18/07/2024
*
* Curso Sistemas Embebidos - TP2
* Esta aplicación permite visualiuzar la temperatura ambiente en un display de caracteres (I2C)
* Para medir temperatura se usa un sensor DHT22.
* Como complemento al display hay un led que cambia de color segun el rango de temperaturas.
* Para que no haya transiciones molestas le puse un ciclo de histéresis a cada rango.
* La medicion de temperatura está filtrada con una media movil de 5 muestras con una frecuencia de muestreo de 2 Hz (aproximadamente xq hay funciones bloqueantes).
* Como interfaz de debug hay una UART.
*
* Las APIs que conforman esta App son:
*   - ledApi
*   - uartApi
*   - displayUiApi
*       - i2clcd
*   - sensorTempApi
*       - DHT
* La api de más alto nivel que hace uso de todas las anteriores es la llamada "app"
* Esta tiene las funciones app_init() y app_loop();
*/

//==========================================
// Dependencias
//==========================================
#include "app.h"

//==========================================
// Main Loop
//==========================================

/**
* @brief Inicializa perifericos y corre la app general
*/
int main() {
    app_init();
    while (true) {
        app_loop();
    }
}
