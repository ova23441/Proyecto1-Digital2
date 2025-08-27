#include <Arduino.h>

// ==== Pines ====
#define PIN_SENSOR   35
#define LED_VERDE    13
#define LED_AMARILLO 12
#define LED_ROJO     33
#define BOTON        25
#define PIN_SERVO    32   // servo en pin 32

// ==== Configuración ADC y filtro EMA ====
float adcRAWEMA = 0;
float adcFiltrado = 0;
float alpha = 0.3; // factor de suavizado EMA
float tempC = 0;

// ==== Configuración PWM LEDs ====
#define FREQ_LED    5000
#define RESOLUCION  8  // 0-255
#define CANAL_VERDE    0
#define CANAL_AMARILLO 1
#define CANAL_ROJO     4

// ==== Configuración Servo ====
#define FREQ_SERVO     50   // 50 Hz típico para servo
#define RESOLUCION_SERVO 16 // más resolución para pulsos precisos
#define CANAL_SERVO    3    // canal distinto a LEDs

// rango de pulsos para servo (ajustable según modelo)
#define PULSE_MIN  500   // µs
#define PULSE_MAX  2400  // µs

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

// ==== Función para mover servo con ángulo ====
void moverServo(int angulo) {
  // Mapear ángulo (0-180) a pulsos en µs
  int pulso = map(angulo, 0, 180, PULSE_MIN, PULSE_MAX);

  // Convertir µs a "duty" según resolución y frecuencia
  int duty = (int)((pulso / 20000.0) * ((1 << RESOLUCION_SERVO) - 1)); 
  // (20000 µs = 20 ms = periodo de 50 Hz)

  ledcWrite(CANAL_SERVO, duty);
}

// ==== Semáforo + Servo ====
void semaforoTemp(float t) {
  if (t < 22.0) {
    ledcWrite(CANAL_VERDE, 255);
    ledcWrite(CANAL_AMARILLO, 0);
    ledcWrite(CANAL_ROJO, 0);
    moverServo(135);   // posición verde
  }
  else if (t < 25.0) {
    ledcWrite(CANAL_VERDE, 0);
    ledcWrite(CANAL_AMARILLO, 255);
    ledcWrite(CANAL_ROJO, 0);
    moverServo(90);    // posición amarilla
  }
  else {
    ledcWrite(CANAL_VERDE, 0);
    ledcWrite(CANAL_AMARILLO, 0);
    ledcWrite(CANAL_ROJO, 255);
    moverServo(45);    // posición roja
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BOTON, INPUT_PULLUP);

  // Configurar PWM LEDs
  ledcSetup(CANAL_VERDE, FREQ_LED, RESOLUCION);
  ledcSetup(CANAL_AMARILLO, FREQ_LED, RESOLUCION);
  ledcSetup(CANAL_ROJO, FREQ_LED, RESOLUCION);

  ledcAttachPin(LED_VERDE, CANAL_VERDE);
  ledcAttachPin(LED_AMARILLO, CANAL_AMARILLO);
  ledcAttachPin(LED_ROJO, CANAL_ROJO);

  // Configurar PWM Servo
  ledcSetup(CANAL_SERVO, FREQ_SERVO, RESOLUCION_SERVO);
  ledcAttachPin(PIN_SERVO, CANAL_SERVO);

  moverServo(90); // posición inicial al centro
}

void loop() {
  if (digitalRead(BOTON) == LOW) {  // solo medir con botón
    getADCEMA();        // aplica el filtro EMA
    mostrarTemp(tempC); // imprime
    semaforoTemp(tempC);// controla LEDs + servo
    delay(500);
  }
}
