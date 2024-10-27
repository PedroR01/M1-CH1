#include "Robot.h"

Robot::Robot() {
  // La inicializacion se hará en begin()
}

void Robot::begin() {
  // Se puede priorirzar una interrupcion?? -- El 4to parametro es la prioridad y funciona con valores de menor a mayor. ¿Evita interrup en simultaneo?
  interaccion.begin();
  cabeza.begin();

  // Pines de interrupcion con Arduino Mega2560 => 2, 3, 18, 19.
  //attachInterrupt(digitalPinToInterrupt(IntP2), mimar, RISING);  // Usar CHANGE(Cualq cambio) o RISING(L to H).
  //attachInterrupt(digitalPinToInterrupt(IntP3), analizarEntorno, RISING);
  
  //attachInterrupt(digitalPinToInterrupt(IntP18), analizarEntorno, RISING);

  piernas.begin();
  estadoGato = 0;
}

void Robot::mover() {
  if (getEstadoGato() > 5)
    piernas.catMovement();
  else
    piernas.botMovement();
}

void Robot::marchaAtras(){
  int direc = 0;
  piernas.rotateCat(direc);
}

void Robot::mimar() {
  /*if (instancia != nullptr) 
    instancia->interaccion.ronronear(instancia->getEstadoGato());*/
  Serial.println("Interrupcion en Pin 2: Evento mimar().");
}

void Robot::analizarEntorno() {
  /* if (instancia != nullptr) 
    instancia->cabeza->observar();*/
  Serial.println("Interrupcion en Pin 3: Evento analizarEntorno().");
}

void Robot::actualizarEstadoGato(float valor) {
  this->estadoGato += valor;
}

void Robot::getTacto() {
  interaccion.ronronear(1);
}

void Robot::getCabeza(){
  Serial.println(cabeza.getDistancia());
}

float Robot::getEstadoGato() {
  return estadoGato;
}
