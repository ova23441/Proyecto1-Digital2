/**************************************************************
 * 🌸🐱🌟 display7.h - Control de display de 7 segmentos 🌟🐱🌸
 * Función: Configuración de pines, mostrar números y
 *          multiplexado para 3 displays de ánodo común
 **************************************************************/

#include "display7.h"

/* 🐾💡 Definición de encendido/apagado para ánodo común 💡🐾 */
// HIGH apaga el segmento, LOW lo enciende
#define SEG_ON  LOW
#define SEG_OFF HIGH

/* 🌸🌼 Tabla de segmentos (0-9) para ánodo común 🌼🌸 */
const uint8_t numeros[10][8] = {
  // A, B, C, D, E, F, G, P (P = punto decimal)
  {SEG_ON, SEG_ON, SEG_ON, SEG_ON, SEG_ON, SEG_ON, SEG_OFF, SEG_OFF}, //0
  {SEG_OFF, SEG_ON, SEG_ON, SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF}, //1
  {SEG_ON, SEG_ON, SEG_OFF, SEG_ON, SEG_ON, SEG_OFF, SEG_ON, SEG_OFF}, //2
  {SEG_ON, SEG_ON, SEG_ON, SEG_ON, SEG_OFF, SEG_OFF, SEG_ON, SEG_OFF}, //3
  {SEG_OFF, SEG_ON, SEG_ON, SEG_OFF, SEG_OFF, SEG_ON, SEG_ON, SEG_OFF}, //4
  {SEG_ON, SEG_OFF, SEG_ON, SEG_ON, SEG_OFF, SEG_ON, SEG_ON, SEG_OFF}, //5
  {SEG_ON, SEG_OFF, SEG_ON, SEG_ON, SEG_ON, SEG_ON, SEG_ON, SEG_OFF}, //6
  {SEG_ON, SEG_ON, SEG_ON, SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF, SEG_OFF}, //7
  {SEG_ON, SEG_ON, SEG_ON, SEG_ON, SEG_ON, SEG_ON, SEG_ON, SEG_OFF}, //8
  {SEG_ON, SEG_ON, SEG_ON, SEG_ON, SEG_OFF, SEG_ON, SEG_ON, SEG_OFF}  //9
};

/**************************************************************
 * 🌟🐱 configDisplay7(): Configurar pines de los displays 🌟🐱
 **************************************************************/
void configDisplay7(void) {
  // Pines de segmentos
  pinMode(dA, OUTPUT);
  pinMode(dB, OUTPUT);
  pinMode(dC, OUTPUT);
  pinMode(dD, OUTPUT);
  pinMode(dE, OUTPUT);
  pinMode(dF, OUTPUT);
  pinMode(dG, OUTPUT);
  pinMode(dP, OUTPUT);

  // Pines de displays
  pinMode(DISP1, OUTPUT);
  pinMode(DISP2, OUTPUT);
  pinMode(DISP3, OUTPUT);

  apagarDisplays(); // apagar todos al iniciar
}

/**************************************************************
 * 🌸🌼 apagarDisplays(): Apagar los 3 displays 🌼🌸
 **************************************************************/
void apagarDisplays(void) {
  digitalWrite(DISP1, LOW);
  digitalWrite(DISP2, LOW);
  digitalWrite(DISP3, LOW);
}

/**************************************************************
 * 🐾🎨 mostrarNumero(): Mostrar un dígito (0-9) 🎨🐾
 **************************************************************/
void mostrarNumero(int numero) {
  if (numero < 0 || numero > 9) return; // validar rango

  digitalWrite(dA, numeros[numero][0]);
  digitalWrite(dB, numeros[numero][1]);
  digitalWrite(dC, numeros[numero][2]);
  digitalWrite(dD, numeros[numero][3]);
  digitalWrite(dE, numeros[numero][4]);
  digitalWrite(dF, numeros[numero][5]);
  digitalWrite(dG, numeros[numero][6]);
  digitalWrite(dP, numeros[numero][7]);
}

/**************************************************************
 * 🌈✨ multiplexarDisplay(): Multiplexado continuo optimizado ✨🌈
 **************************************************************/
void multiplexarDisplay(float valor) {
  int temp = (int)(valor * 10);        // ej: 23.4 -> 234
  int decenas = temp / 100;            // 2
  int unidades = (temp / 10) % 10;     // 3
  int decimal = temp % 10;             // 4

  // ------------------- Display 1 (decenas) 🌸
  apagarDisplays();
  mostrarNumero(decenas);
  digitalWrite(DISP1, HIGH);
  delay(5);

  // ------------------- Display 2 (unidades + punto) 🌼
  apagarDisplays();
  mostrarNumero(unidades);
  digitalWrite(dP, SEG_ON); // punto decimal solo aquí
  digitalWrite(DISP2, HIGH);
  delay(5);
  digitalWrite(dP, SEG_OFF); // apagar punto para los siguientes

  // ------------------- Display 3 (decimal) 🐱
  apagarDisplays();
  mostrarNumero(decimal);
  digitalWrite(DISP3, HIGH);
  delay(5);
}

/**************************************************************
 * 🌸🐾🌟 Fin display7.h 🌟🐾🌸
 **************************************************************/
