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
  const int maxDistance = 100;
  NewPing eyes = NewPing(triggerPin, echoPin, maxDistance);
  unsigned int pingSpeed = 50;  //120  // How frequently are we going to send out a ping (in milliseconds). 50ms would be 20 times a second.
  unsigned long pingTimer;      // Holds the next ping time.
  unsigned long actualTime;
  unsigned long lastObservedTime = 0;
  unsigned long observeCompare = 4;  //10 // 35

  // Servomotor SG-90
  Servo neck;
  const int neckPin = A7;
  const int limiteSup = 100;
  const int limiteInf = 80;
  const int centeredPos = 90;
  int bestAngle = -1;

  bool mustAnalize = false;

  static Cabeza* instanciaActual;

  void seeArround(bool* isPetting);
  void seeArround(bool(Interaccion::*callback)(), Interaccion* interaccionObj, bool* fallback);
  int getFilteredDistance();
  void sortArray(int* arr, int size);

public:
  Cabeza();
  void begin();
  void pingInterruptionCheck();
  static void echoCheck();
  void analize(bool* isPetting);
  void analize(bool(Interaccion::*callback)(), Interaccion* interaccionObj, bool* fallback);
  int getBestAngle();
  void resetBestAngle();
  float getDistancia();
  void headPettingMovement(bool(Interaccion::*callback)(), Interaccion* interaccionObj);
  bool timeLapse();
  void setMoveTimer(unsigned long value);
  void centerNeck();
};

#endif  // !CABEZA_H