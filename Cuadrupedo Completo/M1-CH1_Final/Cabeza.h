#ifndef CABEZA_H
#define CABEZA_H

#include <Arduino.h>
#include <Servo.h>
#include <NewPing.h>
#include "Interaccion.h"

class Cabeza {
private:
  // Ultrasonico HC-SR04
  const int triggerPin = 5;
  const int echoPin = 3;
  const int maxDistance = 90;
  NewPing eyes = NewPing(triggerPin, echoPin, maxDistance);
  unsigned int pingSpeed = 50;  //120  // How frequently are we going to send out a ping (in milliseconds). 50ms would be 20 times a second.
  unsigned long pingTimer;      // Holds the next ping time.
  unsigned long simulatedMillis = 0;
  unsigned long actualTime;
  unsigned long lastObservedTime = 0;
  const unsigned long observeCompare = 150;  //10 // 35

  // Servomotor SG-90
  Servo neck;
  const int neckPin = A7;
  const int limiteSup = 100;
  const int limiteInf = 80;
  const int centeredPos = 90;
  int bestAngle;

  bool mustAnalize = false;

  // Temporizadores
  int tiempoActual;
  unsigned long tCaricias;
  unsigned long tObservacion;
  int temporizador;

  static Cabeza* instanciaActual;

  int sonarToCm();
  void seeArround(bool* isPetting);

public:
  Cabeza();
  void begin();
  void pingInterruptionCheck();
  static void echoCheck();
  void analize(bool* isPetting);
  int getBestAngle();
  float getDistancia();
  void observar();
  void refregarCabeza(bool(Interaccion::*callback)(), Interaccion* interaccionObj);
  void headPettingMovement(bool* isPetting);
};

#endif  // !CABEZA_H