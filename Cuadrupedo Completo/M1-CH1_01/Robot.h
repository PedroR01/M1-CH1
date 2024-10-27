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

  float estadoGato;
  const byte IntP2 = 2;  
  const byte IntP3 = 3;  
  const byte IntP18 = 18;  

  // ARREGLAR: Hace falta los metodos static de interrupcion? Puedo añadir otros metodos static para pasar valores entre dif clases?
public:
  // Constructor original => sin parametro
  Robot();
  void begin();  // Probando otra manera para inicializar los interrupts
  void mover();
  void marchaAtras();
  static void mimar();
  static void analizarEntorno();
  void actualizarEstadoGato(float valor);
  void getCabeza();
  void getTacto();
  float getEstadoGato();
};

#endif
