#include <SoftwareSerial.h>

// Definición de pines para los LEDs
const int ledDO = 2;       // LED para DO
const int ledRE = 3;       // LED para RE
const int ledMI = 4;       // LED para MI
const int ledFA = 5;       // LED para FA
const int ledSOL = 6;      // LED para SOL
const int ledLA = 7;       // LED para LA
const int ledSI = 8;       // LED para SI
const int ledDO_otro = 9;  // LED para DO

// Pines del sensor de distancia HC-SR04
const int trigPin = 10;
const int echoPin = 11;

// Configuración de Bluetooth
SoftwareSerial bluetooth(12, 13);    // RX, TX del módulo HC-06
bool ledsApagados = false;           // Estado de los LEDs
const float UMBRAL_DISTANCIA = 5.0;  // Umbral de 5 cm para apagar LEDs

String receivedData = "";  // Variable para almacenar datos recibidos

// Variable para almacenar qué LEDs están encendidos
bool leds[8] = {false, false, false, false, false, false, false, false};

void setup() {
  // Inicializamos los pines de los LEDs
  pinMode(ledDO, OUTPUT);
  pinMode(ledRE, OUTPUT);
  pinMode(ledMI, OUTPUT);
  pinMode(ledFA, OUTPUT);
  pinMode(ledSOL, OUTPUT);
  pinMode(ledLA, OUTPUT);
  pinMode(ledSI, OUTPUT);
  pinMode(ledDO_otro, OUTPUT);

  // Inicializamos los pines del sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Iniciamos la comunicación serial
  Serial.begin(9600);
  bluetooth.begin(9600);
}

void loop() {
  // Verificar si hay datos disponibles por Bluetooth
  while (bluetooth.available() > 0) {
    char c = bluetooth.read();
    if (c == '\n' || c == '\r') {  // Aceptar '\n' o '\r'
      processBluetoothData(receivedData);
      receivedData = "";  // Limpiar para el siguiente mensaje
    } else {
      receivedData += c;
    }
  }

  // Leer distancia del sensor
  int distancia = medirDistancia();

  if (distancia == 0) {
    apagarLEDs();
  }
}

// Función para procesar datos recibidos por Bluetooth
void processBluetoothData(String receivedData) {
  int noteNumber = receivedData.toInt();

  // Validar si el número está dentro del rango esperado
  if (noteNumber >= 0 && noteNumber <= 7) {
    // Solo enciende el LED si no está encendido
    if (!leds[noteNumber]) {
      leds[noteNumber] = true;  // Marcar el LED como encendido

      // Encender el LED correspondiente
      switch (noteNumber) {
        case 0:
          digitalWrite(ledDO, HIGH);
          Serial.println("Encendiendo LED DO");
          break;
        case 1:
          digitalWrite(ledRE, HIGH);
          Serial.println("Encendiendo LED RE");
          break;
        case 2:
          digitalWrite(ledMI, HIGH);
          Serial.println("Encendiendo LED MI");
          break;
        case 3:
          digitalWrite(ledFA, HIGH);
          Serial.println("Encendiendo LED FA");
          break;
        case 4:
          digitalWrite(ledSOL, HIGH);
          Serial.println("Encendiendo LED SOL");
          break;
        case 5:
          digitalWrite(ledLA, HIGH);
          Serial.println("Encendiendo LED LA");
          break;
        case 6:
          digitalWrite(ledSI, HIGH);
          Serial.println("Encendiendo LED SI");
          break;
        case 7:
          digitalWrite(ledDO_otro, HIGH);
          Serial.println("Encendiendo LED DO_otro");
          break;
      }
    }
  } else {
    bluetooth.println("Número fuera de rango: " + String(noteNumber));
  }
}

// Función para medir la distancia con el sensor HC-SR04
float medirDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Lee el tiempo de respuesta del sensor
  long duracion = pulseIn(echoPin, HIGH, 30000); // Espera hasta 30 ms
  float distancia = duracion * 0.034 / 2; // Convertir a cm

  if (distancia > 5) {
    return 1;
  } else {
    return 0;
  }
}

// Función para apagar todos los LEDs
void apagarLEDs() {
  digitalWrite(ledDO, LOW);
  digitalWrite(ledRE, LOW);
  digitalWrite(ledMI, LOW);
  digitalWrite(ledFA, LOW);
  digitalWrite(ledSOL, LOW);
  digitalWrite(ledLA, LOW);
  digitalWrite(ledSI, LOW);
  digitalWrite(ledDO_otro, LOW);

  // Marcar todos los LEDs como apagados
  for (int i = 0; i < 8; i++) {
    leds[i] = false;
  }

  Serial.println("Apagando todos los LEDs");
}
