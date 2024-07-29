/**
 * @file DHT.cpp
 * @brief API DHT sensor
 *  DHT Library for Digital-output Humidity and Temperature sensors
 *
 *  Tested with DHT11, DHT22
 *  Compatible with SEN11301P, SEN51035P, AM2302, HM2303
 *
 *  Copyright (C) Anthony Hinsinger
 *                Inspired from Wim De Roeve MBED library code
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "DHT.h"

//==================================================================
// Defines
//==================================================================

#define DHT_DATA_LENGTH 40 /**< Longitud de los datos de DHT */

/**
 * @brief Macro para esperar un cambio en el pin y manejar errores.
 * 
 * @param from Valor esperado del pin.
 * @param timeout Tiempo máximo para esperar el cambio en microsegundos.
 * @param err Código de error a devolver si se supera el tiempo de espera.
 */
#define WAIT_PIN_CHANGE(from, timeout, err)          \
    timer.reset();                                   \
    do {                                             \
        if(timer.elapsed_time().count() > timeout) { \
            error = err;                             \
            goto read_error;                         \
        }                                            \
    } while(dio == from);

/**
 * @brief Macro para medir el tiempo de cambio en el pin y manejar errores.
 * 
 * @param from Valor esperado del pin.
 * @param elapsed Variable para almacenar el tiempo transcurrido.
 * @param timeout Tiempo máximo para medir el cambio en microsegundos.
 * @param err Código de error a devolver si se supera el tiempo de espera.
 */
#define MEASURE_PIN_CHANGE(from, elapsed, timeout, err) \
    timer.reset();                                      \
    do {                                                \
        elapsed = timer.elapsed_time().count();         \
        if(elapsed > timeout) {                         \
            error = err;                                \
            goto read_error;                            \
        }                                               \
    } while(dio == from);

//==================================================================
// Métodos públicos
//==================================================================

/**
 * @brief Constructor de la clase DHT.
 * 
 * @param pin Pin donde está conectado el sensor.
 * @param family Tipo de sensor DHT (DHT11 o DHT22).
 */
DHT::DHT(PinName pin, Family family) : _pin(pin), _family(family), _lastReadTime(-1) {}

/**
 * @brief Destructor de la clase DHT.
 */
DHT::~DHT() {}

/**
 * @brief Lee los datos del sensor DHT.
 * 
 * @return Código de error que indica el resultado de la lectura. Ver enumeración ::DHT::Status.
 */
int DHT::read() {
    Status error = SUCCESS; // Código de error inicial
    int i = 0, j = 0;
    unsigned int time1, time2, time3;
    unsigned int timings[DHT_DATA_LENGTH] = {0}; // Array para almacenar los tiempos de los datos
    time_t currentTime;
    DigitalInOut dio(_pin); // Objeto para manejar el pin del sensor
    Timer timer; // Temporizador para medir tiempos

    currentTime = time(NULL); // Obtiene el tiempo actual

    // Verifica si la última lectura fue reciente
    if(_lastReadTime >= 0) {
        if(int(currentTime - _lastReadTime) < 2) {
            return ERROR_TOO_FAST; // Retorna error si la lectura es demasiado rápida
        }
    } else {
        _lastReadTime = currentTime; // Actualiza el tiempo de la última lectura
    }

    timer.start(); // Inicia el temporizador

    // Espera a que el bus sea elevado
    WAIT_PIN_CHANGE(0, 500, ERROR_BUS_BUSY);

    // Envía la señal de inicio: bajo 18ms y luego libera el bus
    dio.output();
    dio = 0;
    ThisThread::sleep_for(18ms);
    dio = 1;
    dio.input();

    // Las siguientes etapas son dependientes del tiempo
    core_util_critical_section_enter(); // Protege la sección crítica

    // Espera a que el bus se eleve de 20 a 40us
    WAIT_PIN_CHANGE(1, 60, ERROR_NOT_DETECTED);

    // Sensor inicia: 80us bajo + 80us elevado
    WAIT_PIN_CHANGE(0, 100, ERROR_BAD_START);
    WAIT_PIN_CHANGE(1, 100, ERROR_BAD_START);

    // Lee los datos (5x8 bits)
    for(i = 0; i < 5; i++) {
        for(j = 0; j < 8; j++) {
            // Sensor: 50us bajo
            WAIT_PIN_CHANGE(0, 100, ERROR_SYNC_TIMEOUT);

            // Sensor: 26-28us (0) a 70us (1) elevado
            MEASURE_PIN_CHANGE(1, timings[i * 8 + j], 100, ERROR_DATA_TIMEOUT);
        }
    }

read_error:
    // Finaliza la lectura (o manejo de errores...)
    core_util_critical_section_exit(); // Sale de la sección crítica

    timer.stop(); // Detiene el temporizador

    if(error) {
        return error; // Retorna el código de error si ocurrió un problema
    }

    // Procesa los datos leídos
    for(i = 0; i < 5; i++) {
        int val = 0;
        for(j = 0; j < 8; j++) {
            if(timings[i * 8 + j] >= 38) {
                val |= (1 << (7 - j));
            }
        }
        _data[i] = val;
    }

    // Verifica el checksum de los datos
    if(_data[4] == ((_data[0] + _data[1] + _data[2] + _data[3]) & 0xFF)) {
        _lastTemperature = calcTemperature(); // Calcula la temperatura
        _lastHumidity = calcHumidity(); // Calcula la humedad
    } else {
        return ERROR_BAD_CHECKSUM; // Retorna error si el checksum es incorrecto
    }

    return SUCCESS; // Retorna éxito si todo está bien
}

/**
 * @brief Obtiene los datos crudos leídos del sensor.
 * 
 * @return Puntero a un arreglo de enteros con los datos crudos.
 */
int* DHT::getRawData() {
    return _data;
}

/**
 * @brief Calcula la temperatura basada en el tipo de sensor.
 * 
 * @return Temperatura en grados Celsius para DHT11, o en grados Celsius con el signo para DHT22.
 */
float DHT::calcTemperature() {
    int v;

    switch(_family) {
        case DHT11:
            v = _data[2];
            return float(v);
        case DHT22:
            v = _data[2] & 0x7F;
            v *= 256;
            v += _data[3];
            if(_data[2] & 0x80)
                v *= -1;
            return float(v) / 10;
    }
    return 0;
}

/**
 * @brief Calcula la humedad basada en el tipo de sensor.
 * 
 * @return Humedad en porcentaje.
 */
float DHT::calcHumidity() {
    int v;

    switch(_family) {
        case DHT11:
            v = _data[0];
            return float(v);
        case DHT22:
            v = _data[0];
            v *= 256;
            v += _data[1];
            return float(v) / 10;
    }
    return 0;
}

/**
 * @brief Convierte grados Celsius a Fahrenheit.
 * 
 * @param celsius Temperatura en grados Celsius.
 * @return Temperatura en grados Fahrenheit.
 */
float DHT::toFarenheit(float celsius) {
    return celsius * 9 / 5 + 32;
}

/**
 * @brief Convierte grados Celsius a Kelvin.
 * 
 * @param celsius Temperatura en grados Celsius.
 * @return Temperatura en Kelvin.
 */
float DHT::toKelvin(float celsius) {
    return celsius + 273.15;
}

/**
 * @brief Obtiene la temperatura en la unidad deseada.
 * 
 * @param unit Unidad deseada para la temperatura (Celsius, Fahrenheit o Kelvin).
 * @return Temperatura en la unidad deseada.
 */
float DHT::getTemperature(Unit unit) {
    if(unit == FARENHEIT)
        return toFarenheit(_lastTemperature);
    else if(unit == KELVIN)
        return toKelvin(_lastTemperature);
    else
        return _lastTemperature;
}

/**
 * @brief Obtiene la humedad medida por el sensor.
 * 
 * @return Humedad en porcentaje.
 */
float DHT::getHumidity() {
    return _lastHumidity;
}