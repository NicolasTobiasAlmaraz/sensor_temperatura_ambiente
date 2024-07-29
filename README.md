# Sensor Temperatura Ambiente

## TP2 - Curso Sistemas Embebidos
### Alumno:
Nicolás Almaraz - nnico.almaraz@gmail.com

### Profesores: 
- Ariel Lutenberg
- Segio Caprile
- Juan Manuel Reta
- Eduardo Filomena

## Video del Funcionamiento
[![Video de Demostración](https://img.youtube.com/vi/RLAnkBudodU/0.jpg)](https://youtu.be/RLAnkBudodU)

## Estructura de archivos fuente
```
/sources
|-- main.cpp
|-- display
| |-- myapp_display_ui.h
| |-- myapp_display_ui.cpp
| |-- i2clcd.h
| |-- i2clcd.cpp
|-- led
| |-- myapp_led.h
| |-- myapp_led.cpp
|-- sensor_temp
| |-- sensor_temp.h
| |-- sensor_temp.cpp
| |-- DHT.h
| |-- DHT.cpp
|-- app
| |-- app.h
| |-- app.cpp
```

## Documentación
En la carpeta "documentacion" está el archivo "index.html" que contiene la documentación generada con Doxygen del proyecto completo.

## Objetivo:
Hacer un mini termómetro de temperatura ambiente.

## Descripcion
La idea sería leer la temperatura con un DHT22 y mostrarlo en un display LCD de caracteres con interfaz I2C.
Por otro lado, en función de la temperatura se actualizará el color de un led RGB.
- Temperaturas frias: Azul
- Temperaturas intermedias: Verde
- Temperaturas calurosas: Rojo

Para que no haya temperaturas límite le introduje un ciclo de histéresis:
- Para pasar de intermedio a frio: bajar de 10°
- Para pasar de frio a intermedio: subir de 15°
- Para pasar de intermedio a caluroso: subir de 25°
- Para pasar de caluroso a intermedio: bajar de 20°

### Diagrama de estados y transiciones:
![image](https://github.com/user-attachments/assets/f862cdf5-48c5-4ec2-b2f6-c4ba90b4de31)


### APIs:

#### API DHT:
- Driver de sensores DHT11 y DHT22 (mediante un enlace One Wire).
- Esta API no la hice yo (la saqué de internet)
- Link al repositorio original: https://github.com/atoy40/mbed-dht

#### API Sensor Temp:
- Esta API se encarga de leer el driver del sensor de temperatura (usa la API anterior).
- Procesamiento de filtrado con una media movil.

#### API i2clcd:
- Se encarga de escribir el display de caracteres LCD mediante una interfaz I2C.
- Esta API no la hice yo (la saqué de internet)
- Estaba originalmente pensada para usarse con STM32CubeIDE y la HAL de STM32.
- Yo le hice unos pequeños cambios para utilizarla con la biblioteca "mbed.h".
- Link al repositorio original: https://github.com/ayleenw/lcd_i2c_stm32

#### API Display Ui:
- Se encarga de la escritura del display a nivel template.

#### API LED:
- Cambia el estado del led

#### API App:
- Hace uso de todas las APIs anteriores para cumplir con el objetivo

## Plataforma de desarrollo:
NUCLEO-F401RE

## Periféricos a utilizar:
- 1 One Wire: Sensor temperatura
- 3 DIGITAL OUT: Salidas led RGB
- 1 I2C: Display
- 1 UART: Api de debug (printf)

## Diagrama en bloques del Hardware:
![image](https://github.com/user-attachments/assets/7905063c-9f3a-41f7-96ab-7ea63691224c)

