#ifndef INTERACCION_H
#define INTERACCION_H

#include <Arduino.h>
#include <CapacitiveSensor.h>

class Interaccion {
private:
  // Pin conectado a la base del transistor para activar el motor de vibración 3v
  const int motorPin = 8;

  const int threshold = 500;
  const int capOutPin = 10;
  const int capInPin = 7;

  const int minCatValue = 30;

  // El pin 5 es el que capta el cambio de corriente producido por el tacto humano.
  CapacitiveSensor touch = CapacitiveSensor(capOutPin, capInPin);

  float catMode = 0;

  bool checkPetting = false;
  bool isPetting;
  bool *pettingPtr;
  static bool* isPettingPtr;
  // Conviene hacer estatico el petting ptr y utilizarlo directamente en todos los metodos necesarios? 
  // porque sino deberia pasarle a casi todos los metodos el ptr por parametro.

public:
  Interaccion();
  void begin();
  void petting();
  bool isInCatMode();
  bool* getIsPetting();
  void setCheckPetting(bool value);
  bool checkInterruptPetting();
  void interruptSetPetting();
  bool getCheckPetting();
  void decreaseCatMode();
  void increaseCatMode();
};

#endif  // !INTERACCION_H