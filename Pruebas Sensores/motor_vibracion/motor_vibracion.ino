const int motorPin = 9; // Pin conectado a la base del transistor

void setup() {
  pinMode(motorPin, OUTPUT);
}

void loop() {
  // Encender el motor de vibración
  digitalWrite(motorPin, HIGH);
  delay(1000); // Motor encendido por 1 segundo

  // Apagar el motor de vibración
  digitalWrite(motorPin, LOW);
  delay(1000); // Motor apagado por 1 segundo
}
