#ifndef INTERACCION_H
#define INTERACCION_H

#include <Arduino.h>
#include <CapacitiveSensor.h>

class Interaccion {
private:
  // Pin conectado a la base del transistor para activar el motor de vibración 3v
  const int motorPin = 9;

  // El pin 5 es el que capta el cambio de corriente producido por el tacto humano.
  CapacitiveSensor tacto = CapacitiveSensor(2, 5);
  const int filtro = 300;

public:
  Interaccion();
  void begin();
  void ronronear(float modoGato);
};

#endif  // !INTERACCION_H