#include <Arduino.h>

#define PIN_SENSOR 35  // GPIO35 ADC1_CH7
#define NUM_MUESTRAS 100
#define FACTOR_CORRECCION 1.55  // Corrige voltaje bajo leído por el ESP32

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);          // 12 bits: 0-4095
  analogSetPinAttenuation(PIN_SENSOR, ADC_11db); // Mejor lectura para bajo voltaje
}

void loop() {
  long suma = 0;
  for (int i = 0; i < NUM_MUESTRAS; i++) {
    suma += analogRead(PIN_SENSOR);
    delayMicroseconds(200);
  }
  
  int lecturaProm = suma / NUM_MUESTRAS;
  
  // Convertir bits a voltaje
  float voltaje = lecturaProm * (5 / 4095.0);  

  // Aplicar factor de corrección
  float voltajeCorregido = voltaje * FACTOR_CORRECCION;

  // Convertir a temperatura usando tu calibración: V → °C
  float temperatura = voltajeCorregido * 82.7;

  Serial.print("Bits: ");
  Serial.print(lecturaProm);
  Serial.print("  |  Voltaje: ");
  Serial.print(voltajeCorregido, 3);
  Serial.print(" V  |  Temperatura: ");
  Serial.print(temperatura, 2);
  Serial.println(" °C");
  
  delay(500);
}
