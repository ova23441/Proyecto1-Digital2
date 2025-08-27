#include <Arduino.h>

// ==== Pines ====
#define PIN_SENSOR   35
#define LED_VERDE    13
#define LED_AMARILLO 12
#define LED_ROJO     33
#define BOTON        25

// ==== Configuración ADC y filtro EMA ====
float adcRAWEMA = 0;
float adcFiltrado = 0;
float alpha = 0.3; // factor de suavizado EMA
float tempC = 0;

// ==== Configuración PWM ====
#define FREQ       5000
#define RESOLUCION 8  // 0-255
#define CANAL_VERDE    0
#define CANAL_AMARILLO 1
#define CANAL_ROJO     2

// ==== Función de filtro EMA ====
void getADCEMA(void) {
  adcRAWEMA = analogReadMilliVolts(PIN_SENSOR);   // leer en mV
  adcFiltrado = (alpha * adcRAWEMA + (1.0 - alpha) * adcFiltrado);
  tempC = adcFiltrado / 10.0;   // LM35 entrega 10 mV / °C
}

// ==== Función para mostrar temp ====
void mostrarTemp(float valor) {
  int temp = (int)(valor * 10);  // multiplicar por 10 para incluir decimal
  int decenas = temp / 100;
  int unidades = (temp / 10) % 10;
  int decimal = temp % 10;

  Serial.print("Temperatura = ");
  Serial.print(decenas);
  Serial.print(unidades);
  Serial.print(".");
  Serial.print(decimal);
  Serial.println(" °C");
}

// ==== Semáforo ====
void semaforoTemp(float t) {
  if (t < 22.0) {
    ledcWrite(CANAL_VERDE, 255);
    ledcWrite(CANAL_AMARILLO, 0);
    ledcWrite(CANAL_ROJO, 0);
  }
  else if (t < 25.0) {
    ledcWrite(CANAL_VERDE, 0);
    ledcWrite(CANAL_AMARILLO, 255);
    ledcWrite(CANAL_ROJO, 0);
  }
  else {
    ledcWrite(CANAL_VERDE, 0);
    ledcWrite(CANAL_AMARILLO, 0);
    ledcWrite(CANAL_ROJO, 255);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BOTON, INPUT_PULLUP);

  // Configurar PWM
  ledcSetup(CANAL_VERDE, FREQ, RESOLUCION);
  ledcSetup(CANAL_AMARILLO, FREQ, RESOLUCION);
  ledcSetup(CANAL_ROJO, FREQ, RESOLUCION);

  ledcAttachPin(LED_VERDE, CANAL_VERDE);
  ledcAttachPin(LED_AMARILLO, CANAL_AMARILLO);
  ledcAttachPin(LED_ROJO, CANAL_ROJO);
}

void loop() {
  if (digitalRead(BOTON) == LOW) {  
    getADCEMA();       
    mostrarTemp(tempC);
    semaforoTemp(tempC); 
    delay(500);
  }
}
