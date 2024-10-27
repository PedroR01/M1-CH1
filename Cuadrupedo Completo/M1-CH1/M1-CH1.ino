#define LIMITE_SUP 110
#define LIMITE_INF 70
#define DISTANCIA_OBSTACULO 20
#define ANCHO_CAMINO 25
#define TIEMPO_MAX_MONITOREO 13000
#define TIEMPO_MOVIMIENTO 850
#define AFECTO_MINIMO 1

#include "Robot.h"
//#include "Cabeza.h"

Robot* Robot::instancia = nullptr;
Robot michi;

//Cabeza cabeza;

void setup() {
  Serial.begin(9600);

  michi.begin();

 // Robot michi(&cabeza);
}

void loop() {
 
}