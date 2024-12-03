#include <SoftwareSerial.h>

// Definir los pines de los LEDs
const int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9}; // 8 LEDs (ajustar según la cantidad de LEDs que uses)
const int numLeds = 8;

// Definir pines para el sensor HC-SR04
const int trigPin = 12;
const int echoPin = 13;

// Configurar la comunicación Bluetooth
SoftwareSerial BTSerial(10, 11); // RX, TX (puedes ajustar los pines según tu configuración)

// Variable para almacenar la distancia
long duration, distance;

void setup() {
  // Iniciar los pines de los LEDs
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // Asegurarse de que todos los LEDs estén apagados al inicio
  }

  // Inicializar comunicación serial y Bluetooth
  Serial.begin(9600);
  BTSerial.begin(9600); // Velocidad del HC-06
  
  // Configurar el sensor HC-SR04
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Mostrar en la consola de Arduino que el sistema está listo
  Serial.println("Sistema iniciado");
}

void loop() {
  // Revisar si hay datos del Bluetooth
  if (BTSerial.available()) {
    // Leer un byte de datos del Bluetooth
    char data = BTSerial.read();
    Serial.print("Dato recibido: ");
    Serial.println(data);

    // Validar si el dato es un número entre 0 y 7 (caracteres '0' a '7')
    if (data >= '0' && data <= '7') {
      int ledIndex = data - '0'; // Convertir el carácter recibido a un número
      Serial.print("Encendiendo LED: ");
      Serial.println(ledIndex);
      digitalWrite(ledPins[ledIndex], HIGH); // Encender el LED correspondiente
    } else {
      Serial.println("Dato no válido recibido");
    }
  }

  // Medir la distancia del sensor HC-SR04
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1; // Calcular la distancia en cm

  // Si la distancia es menor a 5 cm, apagar todos los LEDs
  if (distance < 5) {
    Serial.println("Distancia menor a 5 cm, apagando LEDs...");
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], LOW); // Apagar todos los LEDs
    }
  }

  delay(100); // Pequeña espera antes de la siguiente lectura
}
