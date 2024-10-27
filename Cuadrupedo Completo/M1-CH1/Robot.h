#pragma once
#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>
#include "Piernas.h"
#include "Cabeza.h"
#include "Interaccion.h"

class Robot {
private:
  static Robot* instancia;
  Cabeza* cabeza;
  Piernas piernas = Piernas(); // Esto podria ser simplemente un puntero?

  float estadoGato;
  const byte IntP2 = 2;  // O valor 0 para interrumpir el pin 2
  const byte IntP3 = 3;  // O valor 1 para interrumpir el pin 3

  // ARREGLAR: Hace falta los metodos static de interrupcion? Puedo añadir otros metodos static para pasar valores entre dif clases?
public:
  // Constructor alternativo => static con cabeza por referencia
  Robot(Cabeza* _cabeza):cabeza(_cabeza){
    instancia = this;
  }
  // Constructor original => sin parametro
  Robot();
  void begin();  // Probando otra manera para inicializar los interrupts
  void mover();
  static void mimar();
  static void analizarEntorno();
  void actualizarEstadoGato(float valor);
  float getEstadoGato();
};

#endif  // !ROBOT_H
