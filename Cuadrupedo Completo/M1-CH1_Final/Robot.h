#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>
#include "Piernas.h"
#include "Cabeza.h"
#include "Interaccion.h"

class Robot {
private:
  Cabeza cabeza;
  Piernas piernas;
  Interaccion interaccion;

  int currentTime;
  int lastStopedTime;

  int randomStopTimer;
  int randomActionTimer;

  bool fallback;
  
  void move();
  void checkPetting();
  void stopRobot(bool* isPetting);

public:
  Robot();
  void begin();  // Probando otra manera para inicializar los interrupts
  void path();
  void pingIntCheck();
  void setCheckState(bool value);
};

#endif
