# Sensor Temperatura Ambiente

## TP2 - Curso Sistemas Embebidos
### Alumno:
Nicolás Almaraz
### Profesores: 
- Ariel Lutenberg
- Segio Caprile
- Juan Manuel Reta
- Eduardo Filomena

## Video del Funcionamiento
TDOO: poner link

## Estructura de archivos fuente
```
/sources
|-- main.cpp
|-- display
| |-- myapp_display_ui.h
| |-- myapp_display_ui.c
| |-- i2clcd.h
| |-- i2clcd.c
|-- led
| |-- myapp_led.h
| |-- myapp_led.c
|-- sensor_temp
| |-- sensor_temp.h
| |-- sensor_temp.c
| |-- DHT.h
| |-- DHT.c
|-- main_controller
| |-- myapp_main_controller.h
| |-- myapp_main_controller.c
```

## Documentación
En la carpeta "doc/html" está el archivo "index.html" que contiene la documentación generada con Doxygen del proyecto completo.

## Objetivo:
Hacer un mini termómetro de temperatura ambiente.

## Descripcion
La idea sería leer la temperatura con un LM35 y mostrarlo en un display LCD de caracteres con interfaz I2C.
Por otro lado, en función de la temperatura se actualizará el color de un led RGB.
Temperaturas frias: celeste
Temperaturas intermedias: amarillo
Temperaturas calurosas: rojo

Para que no haya temperaturas límite le introduje un ciclo de histéresis:
Para pasar de intermedio a frio: bajar de 10°
Para pasar de frio a intermedio: subir de 15°

Para pasar de intermedio a caluroso: subir de 25°
Para pasar de caluroso a intermedio: bajar de 20°

### Diagrama de estados y transiciones:
![image](https://github.com/user-attachments/assets/f862cdf5-48c5-4ec2-b2f6-c4ba90b4de31)


### APIs:
#### API LM35:
Se encarga de leer la señal con la información de la temperatura con el ADC

#### API UART:
La utilizo como log de debug

#### API Display Driver:
Se encarga de la comunicación I2C del display.
- Inicializacion
- Posicionamiento del cursor
- Escritura
Para hacer esta API me base fuertemente en el libro.

#### API Display Subsystem:
Se encarga de la escritura en el display a nivel template.

#### API Display Subsystem:
Se encarga de la escritura en el display a nivel template.

#### API LED:
Cambia el estado del led

#### API main controller:
Hace uso de todas las APIs anteriores para cumplir con el objetivo

## Plataforma de desarrollo:
NUCLEO-F401RE

## Periféricos a utilizar:
- 1 ANALOG IN: Temperatura
- 3 DIGITAL OUT: Salidas para cada color
- 1 I2C: Display
- 1 UART: Api de debug

## Diagrama en bloques del Hardware:
![image](https://github.com/user-attachments/assets/fef095dc-f7c7-48df-9d44-925ef89aab9a)
