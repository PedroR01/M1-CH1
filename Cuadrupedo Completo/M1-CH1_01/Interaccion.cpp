#include "Interaccion.h"

Interaccion::Interaccion(){
  // La inicializacion se hará en begin()
}

void Interaccion::begin(){
  pinMode(motorPin, OUTPUT);
}

void Interaccion::ronronear(float modoGato) {
  while (tacto.capacitiveSensor(30) >= filtro) {
    modoGato++;
    digitalWrite(motorPin, HIGH);
  }
  digitalWrite(motorPin, LOW);
  Serial.print("Valor modoGato en mimos: ");
  Serial.println(modoGato);
}