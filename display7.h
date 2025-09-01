/**************************************************************
 * 🌸🐱🌟 display7.h - Definiciones y prototipos 🌟🐱🌸
 * Función: Definir pines y prototipos para controlar
 *          display de 7 segmentos de ánodo común
 **************************************************************/

#ifndef __DISPLAY7_H__
#define __DISPLAY7_H__

#include <Arduino.h>
#include <stdint.h>

/* 🐾💡 Pines de segmentos 💡🐾 */
#define dA 23   // Segmento A 🌸
#define dB 17   // Segmento B 🌼
#define dC 33   // Segmento C 🐱
#define dD 27   // Segmento D 🌷
#define dE 26   // Segmento E ✨
#define dF 18   // Segmento F 🌟
#define dG 5    // Segmento G 🐾
#define dP 16   // Punto decimal 🔹

/* 🌸🌼 Pines de transistores (comunes de cada display) 🌼🌸 */
#define DISP1 22   // Display 1 🐱
#define DISP2 19   // Display 2 🌸
#define DISP3 21   // Display 3 🌷

/* 🐾🌟 Prototipos de funciones 🌟🐾 */
void configDisplay7(void);             // Configurar pines y displays
void mostrarNumero(int numero);        // Mostrar un dígito (0-9)
void apagarDisplays(void);             // Apagar todos los displays
void multiplexarDisplay(float valor);  // Multiplexado para 3 displays

#endif

/**************************************************************
 * 🌸🐱🌟 Fin display7.h 🌟🐱🌸
 **************************************************************/
