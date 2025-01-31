/** 
 * @file DHT.h
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

#ifndef _DHT_H_
#define _DHT_H_

#include "mbed.h"

/** Read DHT11/22 humidity and temperature sensors
 * 
 * Example:
 * @code
 * // read DHT22 every 3 minutes
 *
 * #include "mbed.h"
 * #include "DHT.h"
 * 
 * DHT sensor(D8, DHT::DHT22);
 *
 * int main(void) {
 *     while(1) {
 *         wait(3);
 *         int err = sensor.read();
 *         if (err == DHT::SUCCESS) {
 *             printf("T: %.1f\r\n", sensor.getTemperature(DHT::CELCIUS));
 *         } else {
 *             printf("Error code : %d\r\n", err);
 *         }
 *     }
 * }
 * @endcode
 */


class DHT {
    public:
        /**
        * Tipos de sensores compatibles con esta API
        */
        enum Family {
            DHT11,
            DHT22
        };

        /**
        * @brief posibles estados de retorno al intentar leer el sensor
        */
        enum Status {
            SUCCESS,
            ERROR_BUS_BUSY,
            ERROR_NOT_DETECTED,
            ERROR_BAD_START,
            ERROR_SYNC_TIMEOUT,
            ERROR_DATA_TIMEOUT,
            ERROR_BAD_CHECKSUM,
            ERROR_TOO_FAST,
        };

        
        /**
        * @brief Unidades de temperatura posibles
        */
        enum Unit {
            CELCIUS,
            FARENHEIT,
            KELVIN,
        };

        /**
        * @brief Constructor de la clase DHT.
        * 
        * @param pin Pin donde está conectado el sensor.
        * @param family Tipo de sensor DHT (DHT11 o DHT22).
        */
        DHT(PinName pin, Family DHTtype);
        
        /**
        * @brief Destructor de la clase DHT
        */
        ~DHT();

        /** 
        * @brief Lee los datos del sensor
        * @returns Un status de error. Ver ::DHTError
        */
        int read(void);

        /** 
        * @brief Retorna el dato crudo (puede ser util para enviarlo a traves de una red low-power WAN como LoraWan)
        */
        int* getRawData();

        /** 
        * @brief Retorna la humedad relativa de la ultima lectura exitosa
        */
        float getHumidity(void);

        /** 
        * @brief Retorna la temperatura de la ultima lectura exitosa
        * @param unit Unidad de la temperatura que esperamos CELCIUS / FARENHEIT / KELVIN. Por default es celsious
        */
        float getTemperature(Unit unit = CELCIUS);

    private:
        PinName _pin;
        Family _family;
        time_t  _lastReadTime;
        float _lastTemperature;
        float _lastHumidity;
        int _data[5];
        float calcTemperature();
        float calcHumidity();
        float toFarenheit(float);
        float toKelvin(float);
};

#endif
