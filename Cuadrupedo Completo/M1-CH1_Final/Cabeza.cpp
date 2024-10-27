#include "Cabeza.h"

#define LIMITE_SUP 110
#define LIMITE_INF 70
#define OBSTACLE_DISTANCE 15
#define ROBOT_WIDTH 20

Cabeza* Cabeza::instanciaActual = nullptr;  // Inicializar el puntero estático

Cabeza::Cabeza() {
  // La inicializacion se hará en begin()
  instanciaActual = this;
}

void Cabeza::begin() {
  neck.attach(neckPin);
  neck.write(centeredPos);

  bestAngle = centeredPos;
  pingTimer = millis();
}

void Cabeza::pingInterruptionCheck() {
  if (millis() >= pingTimer) {           // pingSpeed milliseconds since last ping, do another ping.
    pingTimer += pingSpeed;              // Set the next ping time.
    eyes.ping_timer(Cabeza::echoCheck);  // Send out the ping, calls "echoCheck" function every 24uS where you can check the ping status.
  }
}

void Cabeza::echoCheck() {  // Timer2 interrupt calls this function every 24uS where you can check the ping status.
  // Don't do anything here!
  if (instanciaActual->eyes.check_timer()) {  // This is how you check to see if the ping was received.
                                              // Here's where you can add code.
                                              /*
    Serial.print("Ping: ");
    Serial.print(instanciaActual->sonarToCm());  // Ping returned, uS result in ping_result, convert to cm with US_ROUNDTRIP_CM.
    Serial.println("cm");
*/


    // Analizar si hay algun obstaculo enfrente.
    if (instanciaActual->eyes.ping_cm() < OBSTACLE_DISTANCE && instanciaActual->eyes.ping_cm() != 0) {
      instanciaActual->mustAnalize = true;
      instanciaActual->simulatedMillis += 24 / 1000;
    }
  }
  // Don't do anything here!
}

void Cabeza::analize(bool* isPetting) {
  if (this->mustAnalize) {
    actualTime = millis();
    lastObservedTime = actualTime;
    seeArround(isPetting);
  }
}

// Cuando analiza el entorno no se va a mover, por lo que lo unico que lo puede interrumpir es acariciarlo
// FALTA TENER EN CUENTA SI EL ANCHO DEL GATO PERMITE QUE PASE POR EL MEJOR CAMINO DETECTADO
void Cabeza::seeArround(bool* isPetting) {
  int anguloInicial = 30;
  int anguloFinal = 150;
  int paso = 5;
  int mejorAngulo = 90;
  int mejorDistancia = 0;

  // Mueve la cabeza para detectar el entorno
  int angulo = mejorAngulo;
  bool noObstacle = false;

  // Observa lado derecho
  while (angulo >= anguloInicial && !*isPetting) {
    actualTime = millis();
    if ((actualTime - lastObservedTime) >= observeCompare) {
      neck.write(angulo);
      int distancia = eyes.ping_cm();
      Serial.println(distancia);
      // El mejor caso es en el que no encuentra obstaculos en alguna direccion
      if ((distancia == maxDistance || distancia == 0) && !noObstacle) {
        mejorDistancia = distancia;
        mejorAngulo = angulo;
        noObstacle = true;

      } else if (distancia > mejorDistancia && !noObstacle) {  // Si no existe una direccion sin obstaculos, opta por dirigirse a la que haya detectado una mayor distancia con el obstaculo
        mejorDistancia = distancia;
        mejorAngulo = angulo;
      }
      //Serial.println("El mejor angulo es: ");
      Serial.println(distancia);
      lastObservedTime = actualTime;
      angulo--;
    }
  }
  neck.write(90);
  /*
  // En el caso de que no haya encontrado un mejor camino por el lado derecho, mira su lado izquierdo.
  if (mejorAngulo == 90) {
    // Observa lado izquierdo
    while (angulo <= anguloFinal && !*isPetting) {
      actualTime = millis();
      if ((simulatedMillis - lastObservedTime) >= observeCompare) {
        neck.write(angulo);
        Serial.println(angulo);

        int distancia = sonarToCm();
        // El mejor caso es en el que no encuentra obstaculos en alguna direccion
        if (distancia == maxDistance || distancia == 0) {
          mejorDistancia = distancia;
          mejorAngulo = angulo;
          noObstacle = true;
        } else if (distancia > mejorDistancia && !noObstacle) {  // Si no existe una direccion sin obstaculos, opta por dirigirse a la que haya detectado una mayor distancia con el obstaculo
          mejorDistancia = distancia;
          mejorAngulo = angulo;
        }
        lastObservedTime = actualTime;
        angulo++;
      }
      Serial.println("Analizando caminos...");
    }
  }
*/
  //Serial.println(mejorAngulo);
  delay(1500);
  // Usa booleano isPetting para chequear si se ejecutó la interrupción de la caricia. Si no se interrumpio el analisis, la cabeza gira a la mejor pos analizada
  if (*isPetting) {
  }

  // Si se encuentra encerrado debe retroceder y pegar la vuelta o analizar desde otra posicion
  if (mejorDistancia <= OBSTACLE_DISTANCE) {
    //Serial.println("Debe retroceder.");
    //retroceder();  -->  //cuello.write(valorInicialCuello);  // Vuelve a la posicion inicial del cuello
  }
  mustAnalize = false;
  bestAngle = mejorAngulo;
}

int Cabeza::getBestAngle() {
  return bestAngle;
}

int Cabeza::sonarToCm() {
  return (eyes.ping_result / US_ROUNDTRIP_CM);
}


float Cabeza::getDistancia() {
  return eyes.ping_cm();
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
      neck.write(angulo);

      int distancia = eyes.ping_cm();
      if (distancia > mejorDistancia) {
        mejorDistancia = distancia;
        mejorAngulo = angulo;
      }
      tObservacion = tiempoActual;
    }
  }

  // Si hay interrupcion == No termino de analizar el entorno. Por ende evita tomar una decision en base a un analisis incompleto.
  if (!interrupcion) {
    neck.write(mejorAngulo);
    delay(300);  // Espera para que el servo termine de moverse --> REEMPLAZAR POR MILLIS()

    if (mejorDistancia <= OBSTACLE_DISTANCE) {
      //retroceder();  -->  //cuello.write(valorInicialCuello);  // Vuelve a la posicion inicial del cuello
      //delay(1000);  // Espera para retroceder --> REEMPLAZAR POR MILLIS()
    }
  }
}

void Cabeza::refregarCabeza(bool (Interaccion::*callback)(), Interaccion* interaccionObj) {
  int suavizado = random(6, 13);
  bool isPetting = (interaccionObj->*callback)();  // Llamada al método pasado por parametro
  // if(alternar){} else{}
  int i = neck.read();
  while (i < LIMITE_SUP && isPetting) {
    i++;
    neck.write(i);
    isPetting = (interaccionObj->*callback)();
    delay(100);
  }
  while (i > LIMITE_INF && isPetting) {
    i--;
    neck.write(i);
    isPetting = (interaccionObj->*callback)();
    delay(100);
  }
}

void Cabeza::headPettingMovement(bool* isPetting) {
  int suavizado = random(6, 13);
  int i = neck.read();

  while (i < LIMITE_SUP && *isPetting) {
    i++;
    neck.write(i);
    delay(100);
  }
  while (i > LIMITE_INF && *isPetting) {
    i--;
    neck.write(i);
    delay(100);
  }
}