#ifndef PIERNAS_H
#define PIERNAS_H

#include <Arduino.h>
#include <Servo.h>
#include "Interaccion.h"

class Piernas {
private:
  unsigned long actualTime;
  unsigned long lastExecutedMov;      // variable to save the last executed movement time
  const unsigned long movTime = 500;  // ms

  const int backLPin = A1;
  const int backRPin = A5;
  const int frontRPin = A8;
  const int frontLPin = A3;

  Servo backL;
  Servo backR;
  Servo frontL;
  Servo frontR;

  int side;

public:
  Piernas();
  void begin();
  void standUp();
  void lieDown();
  void sitDown();
  void standInTwoLegs();
  void giveHand();
  void rotateCat(int direction, int cant, bool* isPetting);
  void backwards(bool* isPetting);
  void botMovement(bool* isPetting);
  void botMovementWCb(bool(Interaccion::*callback)(), Interaccion* interaccionObj);
  void catMovement(bool* isPetting);
  void catMovementWCb(bool(Interaccion::*callback)(), Interaccion* interaccionObj);
};

#endif  // !PIERNAS_H