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

  long int currentTime;
  long int lastStopedTime;

  unsigned long int randomStopTimer;
  unsigned long int stopTimerCopy;

  unsigned long randomActionTimer;

  bool fallback;
  
  void checkPetting();
  void stopRobot();
  void takeAnotherPath();

public:
  Robot();
  void begin();  // Probando otra manera para inicializar los interrupts
  void path();
  void pingIntCheck();
  void setCheckState(bool value);
  void move();

};

#endif
