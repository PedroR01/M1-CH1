#include <CapacitiveSensor.h>

// pin 6 envia energia - pin 5 capta cambios de energia
CapacitiveSensor capSensor = CapacitiveSensor(6, 5);

// Filtro o rango a partir del que se desea efectuar algun evento
int threshold = 500;

const int motorPin = 9; // Pin conectado a la base del transistor

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(motorPin, OUTPUT);
}

void loop() {
  // Guarda el valor captado por el sensor
  long sensorValue = capSensor.capacitiveSensor(30);

  if (sensorValue > threshold)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(motorPin, HIGH);
    }
  else
    {
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(motorPin, LOW);
    }

  delay(10);
}
