#ifndef CABEZA_H
#define CABEZA_H

#include <Arduino.h>
#include <Servo.h>
#include <NewPing.h>

class Cabeza {
private:
  // Ultrasonico HC-SR04
  const int triggerPin = 8;
  const int echoPin = 3;
  NewPing ojos = NewPing(triggerPin, echoPin, 100);


  // Servomotor SG-90
  Servo cuello;
  const int cuelloPin = A1;
  const int limiteSup = 100;
  const int limiteInf = 80;
  const int valorInicialCuello = 90;

  // Temporizadores
  int tiempoActual;
  unsigned long tCaricias;
  unsigned long tObservacion;
  int temporizador;

public:
  Cabeza(); 
  void begin();
  float getDistancia();
  void observar();
  void refregarCabeza();
};

#endif  // !CABEZA_H