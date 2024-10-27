#ifndef PIERNAS_H
#define PIERNAS_H

#include <Arduino.h>
#include <Servo.h>
#include "Interaccion.h"

class Piernas {
private:
  unsigned long lastMoveTime;
  unsigned long moveTimer = 2; // 30 para animaciones en modo gato.

  const int backLPin = A1;
  const int backRPin = A5;
  const int frontRPin = A8;
  const int frontLPin = A3;

  Servo backL;
  Servo backR;
  Servo frontL;
  Servo frontR;

  int side;


  bool timeLapse();

public:
  Piernas();
  void begin();
  void standUp();
  void lieDown();
  void sitDown();
  void standInTwoLegs();
  void giveHand();
  void rotateCat(int direction, int cant, bool* isPetting);
  void backwards(bool (Interaccion::*callback)(), Interaccion* interaccionObj);
  void botMovementWCb(bool (Interaccion::*callback)(), Interaccion* interaccionObj);
  void catMovementWCb(bool (Interaccion::*callback)(), Interaccion* interaccionObj);
  void setMoveTimer(unsigned long value);
};

#endif  // !PIERNAS_H