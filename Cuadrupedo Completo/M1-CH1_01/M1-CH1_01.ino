#define LIMITE_SUP 110
#define LIMITE_INF 70
#define DISTANCIA_OBSTACULO 20
#define ANCHO_CAMINO 25
#define TIEMPO_MAX_MONITOREO 13000
#define TIEMPO_MOVIMIENTO 850
#define AFECTO_MINIMO 1

#include "Robot.h"

// Cambiar pin 6-5 del tacto al pin 2 para manejar la interrup de la caricia.
// Cambiar pin A0 del echo (output) al pin 3 para manejar la interrup de la distancia. Si no me equivoco, el echo es el que recibe la devolución de la señal enviada por el trigger.
// y cuando recibe algo, entre varías cosas, marca la entrada como HIGH
// Si uso las distintas partes como objetos tengo el problema de que se ejecuta su constructor muchas veces, y sino el problema está en que no se ejecuta su constructor nunca.

//Robot* Robot::instancia = nullptr;
Robot michi;

void setup() {
  Serial.begin(9600);

  michi.begin();
}

void loop() {
  michi.mover();
  //michi.marchaAtras();
  //michi.getTacto();
  //michi.getCabeza();
}