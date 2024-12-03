#include <SoftwareSerial.h>

// Definición de pines para los LEDs
const int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9};  // Pines para los LEDs (0-7)
const int numLeds = 8;

// Pines para el sensor de distancia HC-SR04
const int trigPin = 10;
const int echoPin = 11;

// Inicialización del puerto serial
SoftwareSerial btSerial(12, 13); // RX, TX para el módulo HC-06

// Variables para el sensor de distancia
long duration;
int distance;

void setup() {
  // Configuración de los LEDs como salida
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // Apagar todos los LEDs al inicio
  }

  // Configuración de los pines del sensor de distancia
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Configuración del puerto serial
  Serial.begin(9600);
  btSerial.begin(9600); // Configuración del HC-06
  
  Serial.println("Esperando conexión...");
}

void loop() {
  // Verificar si hay datos entrantes desde la app
  if (btSerial.available()) {
    int ledNumber = btSerial.read() - '0'; // Leer el número enviado por la app (0-7)
    
    // Asegurarse de que el número esté dentro del rango válido (0-7)
    if (ledNumber >= 0 && ledNumber < numLeds) {
      digitalWrite(ledPins[ledNumber], HIGH); // Enciende el LED correspondiente
      Serial.print("LED ");
      Serial.print(ledNumber);
      Serial.println(" encendido");
    }
  }

  // Medir la distancia con el sensor HC-SR04
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  
  // Calcular la distancia en cm
  distance = duration * 0.034 / 2;
  
  // Si la distancia es menor o igual a 5 cm, apagar todos los LEDs
  if (distance <= 5) {
    Serial.println("Mano detectada a menos de 5 cm. Apagando todos los LEDs.");
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], LOW); // Apagar todos los LEDs
    }
  }

  delay(100); // Esperar un poco antes de la siguiente lectura
}
