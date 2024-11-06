#include "Interaccion.h"

bool* Interaccion::isPettingPtr = nullptr;  // Inicializar el puntero estático


Interaccion::Interaccion() {
  // La inicializacion se hará en begin()
  isPettingPtr = &isPetting;  // apunta a la direccion de isPetting --> *isPettingPtr obtiene el valor que hay guardado en esa direccion.
}

void Interaccion::begin() {
  pinMode(motorPin, OUTPUT);
  isPetting = false;
  pettingPtr = &isPetting;
}

void Interaccion::petting() {
  if (touch.capacitiveSensor(30) > threshold) {
    isPetting = true;
    //if (isInCatMode())
    //digitalWrite(motorPin, HIGH);
    //Serial.print("Estado motor vibracion: ");
    //Serial.println(digitalRead(motorPin));
    if (catMode < 100) // Máximo acumulable del modo gato = 100.
      catMode++;
  } else {
    digitalWrite(motorPin, LOW);
    isPetting = false;
    if (catMode >= 0.2)
      catMode -= 0.02;
  }
}

bool Interaccion::isInCatMode() {
  return (this->catMode > minCatValue);
}

bool* Interaccion::getIsPetting() {
  return pettingPtr;
}

void Interaccion::setCheckPetting(bool value) {
  this->checkPetting = value;
}

bool Interaccion::checkInterruptPetting() {
  petting();
  return isPetting;
}

void Interaccion::interruptSetPetting() {
  if (touch.capacitiveSensor(30) > threshold)
    isPetting = true;
  else
    isPetting = false;
}


bool Interaccion::getCheckPetting() {
  return this->checkPetting;
}

void Interaccion::decreaseCatMode() {
  this->catMode--;
}

void Interaccion::increaseCatMode() {
  this->catMode++;
}
