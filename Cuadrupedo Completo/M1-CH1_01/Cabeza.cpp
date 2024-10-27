#define LIMITE_SUP 110
#define LIMITE_INF 70
#define DISTANCIA_OBSTACULO 20

#include "Cabeza.h"

Cabeza::Cabeza() {
  // La inicializacion se hará en begin()
}

void Cabeza::begin() {
  cuello.attach(cuelloPin);
  cuello.write(valorInicialCuello);

  temporizador = 200;
  tiempoActual = millis();
  tCaricias = 0;
  tObservacion = 0;
}

float Cabeza::getDistancia(){
  return ojos.ping_cm();
}

void Cabeza::observar() {
  int anguloInicial = 45;
  int anguloFinal = 135;
  int paso = 5;
  int mejorAngulo = 90;
  int mejorDistancia = 0;

  bool interrupcion = false;

  // Mueve la cabeza para detectar el entorno
  for (int angulo = anguloInicial; angulo <= anguloFinal; angulo += paso) {
    tiempoActual = millis();
    if (tiempoActual - tObservacion >= temporizador) {
      cuello.write(angulo);

      int distancia = ojos.ping_cm();
      if (distancia > mejorDistancia) {
        mejorDistancia = distancia;
        mejorAngulo = angulo;
      }
      tObservacion = tiempoActual;
    }
  }

  // Si hay interrupcion == No termino de analizar el entorno. Por ende evita tomar una decision en base a un analisis incompleto.
  if (!interrupcion) {
    cuello.write(mejorAngulo);
    delay(300);  // Espera para que el servo termine de moverse --> REEMPLAZAR POR MILLIS()

    if (mejorDistancia <= DISTANCIA_OBSTACULO) {
      //retroceder();  -->  //cuello.write(valorInicialCuello);  // Vuelve a la posicion inicial del cuello
      //delay(1000);  // Espera para retroceder --> REEMPLAZAR POR MILLIS()
    }
  }
}

void Cabeza::refregarCabeza() {
  int i;
  int suavizado = random(6, 13);
  // if(alternar){} else{}
  for (i = cuello.read(); i < LIMITE_SUP; i++) {
    cuello.write(i);
    if (i >= LIMITE_SUP - suavizado)
      delay(random(60, 100));
    else
      delay(random(35, 75));
  }
  for (i = cuello.read(); i > LIMITE_INF; i--) {
    cuello.write(i);
    if (i <= LIMITE_INF + suavizado)
      delay(random(60, 100));
    else
      delay(random(35, 75));
  }
}