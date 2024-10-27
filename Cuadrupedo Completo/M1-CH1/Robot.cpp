#include "Robot.h"

Robot::Robot() {
  instancia = this;
  estadoGato = 0;
  // O puede que haya que reemplazar el 2 = 0 y el 3 = 1
  // Cuando cambia el valor del pin 5 al interactuar, se interrumpe cualquier funcion en marcha y ejecuta la interacción.
  attachInterrupt(digitalPinToInterrupt(IntP2), mimar, CHANGE);  // Usar CHANGE(Cualq cambio) o RISING(L to H).

  // Cuando se detecta un objeto en el pin trigger del ultrasonico, se interrumpe para mirar alrededor. || PROBLEMA --> 2 interrupciones en simultaneo? Cuando se acaricia siendo detectado por los ojos.
  attachInterrupt(digitalPinToInterrupt(IntP3), analizarEntorno, CHANGE);
}

void Robot::begin() {
  attachInterrupt(digitalPinToInterrupt(IntP2), mimar, RISING);  // Usar CHANGE(Cualq cambio) o RISING(L to H).
  attachInterrupt(digitalPinToInterrupt(IntP3), analizarEntorno, CHANGE);
}

void Robot::mover() {
  if (getEstadoGato() > 5)
    piernas.movimientoGatuno();
  else
    piernas.movimientoRobotico();
}

void Robot::mimar() {
  if (instancia != nullptr) 
    instancia->cabeza->acariciar(instancia->getEstadoGato());
  
}

void Robot::analizarEntorno() {
  if (instancia != nullptr) 
    instancia->cabeza->observar();
  
  //cabeza.observar();
}

void Robot::actualizarEstadoGato(float valor) {
  this->estadoGato += valor;
}

float Robot::getEstadoGato() {
  return estadoGato;
}
