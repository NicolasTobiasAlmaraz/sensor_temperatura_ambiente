/**
* @file i2clcd.cpp
* @brief Library for I2C LCD display with mbedOS
* @author controllerstech, 2019-05-29
* https://controllerstech.com/lcd-20x4-using-i2c-with-stm32/
*
* @author (modified) Nicolas Almaraz, 2024-07-21
*
* Note Nicolas Almaraz:
* I have adapted this API for use within the mbed framework
*/

//=======================================================
//Dependencias
//=======================================================

#include "mbed.h"

#include "i2clcd.h"

//=======================================================
//Defines privados
//=======================================================

#define GPIO_I2C_SDA D6	//<! GPIO SDA I2C
#define GPIO_I2C_SCL D7	//<! GPIO SCL I2C

#define SLAVE_ADDRESS_LCD 0x27 << 1 //<! Direccion del display
#define LINES 4						//<! Cantidad de rengloes
#define COLUMNS 20					//<! Cantidad de espacios en un renglon

//=======================================================
// Tipos de datos privados
//=======================================================

//=======================================================
// Estructuras privadas
//=======================================================

//=======================================================
//Variables globales privadas
//=======================================================

/**
* @brief Objeto handler de la comunicacion I2C
*/
I2C g_i2c(I2C_SDA, I2C_SCL);

/**
* @brief Matriz de direcciones del display
*/
const uint8_t lcdPos[LINES][COLUMNS] = {
	{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13},
	{0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53},
	{0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27},
	{0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67}};

//=======================================================
// Declaración de funciones privadas
//=======================================================

/**
/**
 * @brief Envía un comando al display LCD.
 * 
 * Esta función toma un comando de 8 bits y lo envía al display LCD en modo de 4 bits,
 * dividiéndolo en dos nibbles (mitades) y enviándolos secuencialmente.
 * 
 * @param cmd Comando a enviar al display LCD.
 */
static void lcd_send_cmd(char cmd);

/**
 * @brief Envía un dato al display LCD.
 * 
 * Esta función toma un dato de 8 bits y lo envía al display LCD en modo de 4 bits,
 * dividiéndolo en dos nibbles (mitades) y enviándolos secuencialmente.
 * 
 * @param data Dato a enviar al display LCD.
 */
static void lcd_send_data(char data);

/**
 * @brief Envia un string al display LCD, manejando caracteres especiales.
 * 
 * Esta función recorre el string de entrada y envía cada carácter al display LCD. 
 * Si encuentra una secuencia de bytes que representa un carácter especial en UTF-8, 
 * lo reemplaza con el código correspondiente que el LCD puede interpretar.
 * 
 * @param str String a escribir en el display LCD.
 */
static void lcd_send_string(const char *str);

/**
* @brief Borra las letras que haya entre la posicion del cursor y la linea
* @param line Linea en la que esta el cursor [0-3]
* @param column Columna en la que esta el cursor [0-19]
*/
static void lcd_clear_line(uint8_t line, uint8_t column);


//=======================================================
// Implementacion de funciones publicas
//=======================================================
void lcd_init(void)
{
    printf("Init display driver\r\n");
    
	// 4 bit initialisation
	thread_sleep_for(50); // wait for >40ms
	lcd_send_cmd(0x30);
	thread_sleep_for(5); // wait for >4.1ms
	lcd_send_cmd(0x30);
	thread_sleep_for(1); // wait for >100us
	lcd_send_cmd(0x30);
	thread_sleep_for(10);
	lcd_send_cmd(0x20); // 4bit mode
	thread_sleep_for(10);

	// display initialisation
	lcd_send_cmd(0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	thread_sleep_for(1);
	lcd_send_cmd(0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	thread_sleep_for(1);
	lcd_send_cmd(0x01); // clear display
	thread_sleep_for(2);
	lcd_send_cmd(0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	thread_sleep_for(1);
	lcd_send_cmd(0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

void lcd_clear(void) {
	//Se posiciona en el inicio
	lcd_send_cmd(0x00);
	for (int i = 0; i < 100; i++) {
		//Llena todo de espacios
		lcd_send_data(' ');
	}
}

void lcd_write(const char *txt, uint8_t line, uint8_t column) {
	//Borra la linea (desde la posicion del cursor hasta el final)
	lcd_clear_line(line, column);
	
	//Se posiciona en el lugar donde hay que empezar a escribir
	lcd_send_cmd(0x80 | lcdPos[line][column]);

	//Envia caracteres a escribir
	lcd_send_string(txt);
}

//=======================================================
// Implementacion de funciones privadas
//=======================================================

static void lcd_send_string(const char *str) {
    uint8_t rep = 195;                // Primer byte de caracteres especiales en UTF-8
    const uint8_t rep_auml = 164;     // Segundo byte para "ä"
    const uint8_t rep_ouml = 182;     // Segundo byte para "ö"
    const uint8_t rep_uuml = 188;     // Segundo byte para "ü"
    const uint8_t rep_sz = 159;       // Segundo byte para "ß"
    char auml[] = "\xE1";             // Código específico para "ä" en el LCD
    char ouml[] = "\xEF";             // Código específico para "ö" en el LCD
    char uuml[] = "\xF5";             // Código específico para "ü" en el LCD
    char sz[] = "\xE2";               // Código específico para "ß" en el LCD
    bool is_cmd = false;

    // Recorre el string de principio a fin
    while (*str) {
        if (*str != '\0') {
            // Si el primer byte de un carácter especial fue detectado
            if (is_cmd) {
                // Reemplaza el segundo byte con el código correspondiente para el LCD
                switch (*str) {
                    case rep_auml:
                        lcd_send_data(*auml); // Envia "ä"
                        break;
                    case rep_ouml:
                        lcd_send_data(*ouml); // Envia "ö"
                        break;
                    case rep_uuml:
                        lcd_send_data(*uuml); // Envia "ü"
                        break;
                    case rep_sz:
                        lcd_send_data(*sz);   // Envia "ß"
                        break;
                    default:
                        break;
                }
                *str++;  // Avanza al siguiente carácter
                is_cmd = false;  // Reinicia el indicador de comando
                continue;  // Continúa con el próximo carácter
            }
            // Si se encuentra el primer byte de un carácter especial
            if (*str == rep) {
                *str++;  // Avanza al segundo byte
                is_cmd = true;  // Indica que el siguiente byte es parte de un carácter especial
                continue;  // Continúa con el próximo byte
            }
            lcd_send_data(*str++);  // Envia el carácter actual al LCD y avanza al siguiente
        }
    }
}

static void lcd_send_cmd(char cmd) {
    char data_u, data_l;
    uint8_t data_t[4];

    // Toma los 4 bits más significativos del comando
    data_u = (cmd & 0xf0);

    // Toma los 4 bits menos significativos del comando y los desplaza a la posición alta
    data_l = ((cmd << 4) & 0xf0);

    // Crea la secuencia de datos para enviar al LCD
    data_t[0] = data_u | 0x0C; // Envia el nibble alto con en=1 y rs=0
    data_t[1] = data_u | 0x08; // Envia el nibble alto con en=0 y rs=0
    data_t[2] = data_l | 0x0C; // Envia el nibble bajo con en=1 y rs=0
    data_t[3] = data_l | 0x08; // Envia el nibble bajo con en=0 y rs=0

    // Envía los 4 bytes al display LCD a través de I2C
    g_i2c.write((uint8_t)SLAVE_ADDRESS_LCD, (const char *)data_t, 4, false);
}

static void lcd_send_data(char data) {
    char data_u, data_l;
    uint8_t data_t[4];

    // Toma los 4 bits más significativos del dato
    data_u = (data & 0xf0);

    // Toma los 4 bits menos significativos del dato y los desplaza a la posición alta
    data_l = ((data << 4) & 0xf0);

    // Crea la secuencia de datos para enviar al LCD
    data_t[0] = data_u | 0x0D; // Envia el nibble alto con en=1 y rs=1
    data_t[1] = data_u | 0x09; // Envia el nibble alto con en=0 y rs=1
    data_t[2] = data_l | 0x0D; // Envia el nibble bajo con en=1 y rs=1
    data_t[3] = data_l | 0x09; // Envia el nibble bajo con en=0 y rs=1

    // Envía los 4 bytes al display LCD a través de I2C
    g_i2c.write((uint8_t)SLAVE_ADDRESS_LCD, (const char *)data_t, 4, false);
}

static void lcd_clear_line(uint8_t line, uint8_t column)
{
	//Me posiciono en el lugar donde hay que empezar a borrar
	lcd_send_cmd(0x80 | lcdPos[line][column]);
	
	//Escribo espacios para borrar las letras (hasta el final del renglon)
	for (int i = 0; i < (COLUMNS - column); i++)
	{
		lcd_send_data(' ');
	}
}
