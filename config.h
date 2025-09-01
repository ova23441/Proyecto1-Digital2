/**************************************************************
 * 🌸🐱🌟 Configuración Display & Adafruit IO 🌟🐱🌸
 * Función: Declaraciones de funciones para display de 7 segmentos
 *          y configuración de Adafruit IO para envío de temperatura
 **************************************************************/

/* 🌼🐾 Declaración de funciones display 7 segmentos 🐾🌼 */
void configDisplay7();               // Configura pines y parámetros del display
void multiplexarDisplay(float valor); // Muestra temperatura en el display de forma multiplexada

/**************************************************************
 * 🌟🐱 Adafruit IO Config 🌟🐱
 **************************************************************/
#define IO_USERNAME "dulce_18"       // Usuario Adafruit IO 
#define IO_KEY      "..." // Clave Adafruit IO 

/**************************************************************
 * 🌸✨ Adafruit IO WiFi Config 🌸✨                        
 **************************************************************/
#define WIFI_SSID "Galaxy"            // Nombre de la red WiFi 
#define WIFI_PASS "okass12345678900"  // Contraseña de la red WiFi 

/* 📡✨ Librería Adafruit IO WiFi ✨📡 */
#include "AdafruitIO_WiFi.h"

/* 🐱🌷 Instancia Adafruit IO 🌷🐱 */
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

/* 🌸🐾 Feed de temperatura 🌸🐾 */
AdafruitIO_Feed *temperaturaFeed = io.feed("Proy_lector_temperatura"); // Feed para enviar valores de temperatura 
