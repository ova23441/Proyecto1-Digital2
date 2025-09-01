/**************************************************************
 * 🌸🐱🌟 Proyecto1 🌟🐱🌸
 * Función: Leer temperatura con LM35, mostrar en display
 *          de 7 segmentos, controlar LEDs tipo semáforo,
 *          mover un servo según temperatura e implementación
 *          Adafruit.
 **************************************************************/

/* 🌷🌼✨ Librerías ✨🌼🌷 */
#include <Arduino.h>
#include "display7.h"
#include "AdafruitIO_WiFi.h"

/* 🌟🐾 Configuración Adafruit IO 🐾🌟 */
#define IO_USERNAME "dulce_18"
#define IO_KEY      "..."

/* 🌻🌸 Configuración WiFi 🌸🌻 */
#define WIFI_SSID "Galaxy"
#define WIFI_PASS "okass12345678900"

/* 📡✨ Instancia Adafruit IO ✨📡 */
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

/* 🌸🌷 Feed de temperatura 🌷🌸 */
AdafruitIO_Feed *temperaturaFeed = io.feed("Proy_lector_temperatura");

/* 🐱🌼 Pines del ESP32 🌼🐱 */
#define PIN_SENSOR   34   // Pin de lectura LM35 
#define LED_VERDE    13   // Semáforo verde 🟢
#define LED_AMARILLO 12   // Semáforo amarillo 🟡
#define LED_ROJO     14   // Semáforo rojo 🔴
#define BOTON        25   // Botón de activación 
#define PIN_SERVO    32   // Servo motor 

/* 🌊💧 Configuración ADC y filtro EMA 💧🌊 */
float adcRAWEMA = 0;
float adcFiltrado = 0;
float alpha = 0.3; // Factor suavizado EMA 🌟
float tempC = 0;

/* 💡🎨 Configuración PWM LEDs 🎨💡 */
#define FREQ_LED    5000
#define RESOLUCION  8  // 0-255
#define CANAL_VERDE    0
#define CANAL_AMARILLO 1
#define CANAL_ROJO     4

/* 🎛️🌸 Configuración Servo 🌸🎛️ */
#define FREQ_SERVO       50   // 50 Hz típico
#define RESOLUCION_SERVO 16   // Mayor resolución para pulsos
#define CANAL_SERVO      3    // Canal PWM distinto a LEDs
#define PULSE_MIN  500       // Pulso mínimo µs 
#define PULSE_MAX  2400      // Pulso máximo µs 

/* ⏱️🐱 Variables antirrebote botón 🐱⏱️ */
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; // 50 ms 
bool lastButtonState = HIGH; // Botón en reposo HIGH (INPUT_PULLUP)
bool buttonState = HIGH;

/* 🌸🌷✨ Funciones ✨🌷🌸 */

/**************************************************************
 * 🌊✨ getADCEMA(): Filtro EMA para suavizar lectura LM35 🌊✨
 **************************************************************/
void getADCEMA(void) {
  adcRAWEMA = analogReadMilliVolts(PIN_SENSOR);   // leer en mV
  adcFiltrado = (alpha * adcRAWEMA + (1.0 - alpha) * adcFiltrado);
  tempC = adcFiltrado / 10.0;   // LM35: 10 mV / °C 
}

/**************************************************************
 * 🌸🌼 mostrarTemp(): Mostrar temperatura en serial y display 🌼🌸
 **************************************************************/
void mostrarTemp(float valor) {
  int temp = (int)(valor * 10);  // Incluir decimal
  int decenas = temp / 100;
  int unidades = (temp / 10) % 10;
  int decimal = temp % 10;

  Serial.print("Temperatura = ");
  Serial.print(decenas);
  Serial.print(unidades);
  Serial.print(".");
  Serial.print(decimal);
  Serial.println(" °C 🌷");
}

/**************************************************************
 * 🐾🎨 moverServo(): Mover servo según ángulo 🎨🐾
 **************************************************************/
void moverServo(int angulo) {
  int pulso = map(angulo, 0, 180, PULSE_MIN, PULSE_MAX);
  int duty = (int)((pulso / 20000.0) * ((1 << RESOLUCION_SERVO) - 1));
  ledcWrite(CANAL_SERVO, duty);
}

/**************************************************************
 * 🌈🐱 semaforoTemp(): Semáforo y servo según temperatura 🐱🌈
 **************************************************************/
void semaforoTemp(float t) {
  if (t < 22.0) { // Baja
    ledcWrite(CANAL_VERDE, 255);
    ledcWrite(CANAL_AMARILLO, 0);
    ledcWrite(CANAL_ROJO, 0);
    moverServo(135);
  }
  else if (t < 25.0) { // Media
    ledcWrite(CANAL_VERDE, 0);
    ledcWrite(CANAL_AMARILLO, 255);
    ledcWrite(CANAL_ROJO, 0);
    moverServo(90);
  }
  else { // Alta
    ledcWrite(CANAL_VERDE, 0);
    ledcWrite(CANAL_AMARILLO, 0);
    ledcWrite(CANAL_ROJO, 255);
    moverServo(45);
  }
}

/**************************************************************
 * 🌟🐾 setup(): Configuración inicial 🐾🌟
 **************************************************************/
void setup() {
  Serial.begin(115200);
  pinMode(BOTON, INPUT_PULLUP);

  // Configuración display 
  configDisplay7();

  // Configuración PWM LEDs 
  ledcSetup(CANAL_VERDE, FREQ_LED, RESOLUCION);
  ledcSetup(CANAL_AMARILLO, FREQ_LED, RESOLUCION);
  ledcSetup(CANAL_ROJO, FREQ_LED, RESOLUCION);
  ledcAttachPin(LED_VERDE, CANAL_VERDE);
  ledcAttachPin(LED_AMARILLO, CANAL_AMARILLO);
  ledcAttachPin(LED_ROJO, CANAL_ROJO);

  // Configuración PWM Servo 
  ledcSetup(CANAL_SERVO, FREQ_SERVO, RESOLUCION_SERVO);
  ledcAttachPin(PIN_SERVO, CANAL_SERVO);
  moverServo(90); // posición inicial 

  // Conexión a Adafruit IO 
  Serial.print("Conectando a Adafruit IO");
  io.connect();
  while(io.status() < AIO_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Conectado a Adafruit IO!");
}

/**************************************************************
 * 🌼✨ loop(): Loop principal ✨🌼
 **************************************************************/
void loop() {
  bool reading = digitalRead(BOTON);

  // ⏳🐾 Antirrebote 🐾⏳
  if (reading != lastButtonState) lastDebounceTime = millis();

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) { // Botón presionado 
        getADCEMA();
        mostrarTemp(tempC);
        semaforoTemp(tempC);

        // Enviar a Adafruit IO 
        temperaturaFeed->save(tempC);
        Serial.println("Temperatura enviada a Adafruit IO 🌷");
      }
    }
  }

  lastButtonState = reading;

  // Multiplexado display siempre 🌟
  multiplexarDisplay(tempC);
}

/**************************************************************
 * 🌸🐱🌟 Dulce Ovando - 23441 🌟🐱🌸
 **************************************************************/
