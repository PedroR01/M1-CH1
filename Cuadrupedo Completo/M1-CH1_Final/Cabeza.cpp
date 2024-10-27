#include "Cabeza.h"

#define LIMITE_SUP 110
#define LIMITE_INF 70
#define OBSTACLE_DISTANCE 15
#define ROBOT_WIDTH 20
#define NUM_READINGS 5  // Número de lecturas para el filtro de lecturas del Ultrasonico(US)

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

    // Analizar si hay algun obstaculo enfrente.
    if (instanciaActual->eyes.ping_cm() < OBSTACLE_DISTANCE && instanciaActual->eyes.ping_cm() != 0)
      instanciaActual->mustAnalize = true;
  }
  // Don't do anything here!
}

void Cabeza::analize(bool (Interaccion::*callback)(), Interaccion* interaccionObj, bool* fallback) {
  // Booleano modificado desde la detección por la interrupcion del Timer2 en el metodo echoCheck
  if (this->mustAnalize) {
    actualTime = millis();
    lastObservedTime = actualTime;
    seeArround(callback, interaccionObj, fallback);
  }
}

void Cabeza::seeArround(bool (Interaccion::*callback)(), Interaccion* interaccionObj, bool* fallback) {
  int anguloInicial = 30;
  int anguloFinal = 150;
  int paso = 5;
  int mejorAngulo = 90;
  int mejorDistancia = 0;

  // Mueve la cabeza para detectar el entorno
  int angulo = mejorAngulo;
  bool noObstacle = false;
  bool isPetting = (interaccionObj->*callback)();

  // Observa lado derecho
  while (angulo >= anguloInicial && !isPetting) {
    actualTime = millis();
    if ((actualTime - lastObservedTime) >= observeCompare) {
      neck.write(angulo);
      int distancia = getFilteredDistance();
      // El mejor caso es en el que no encuentra obstaculos en alguna direccion
      if ((distancia == maxDistance || distancia == 0) && !noObstacle) {
        mejorDistancia = distancia;
        mejorAngulo = angulo;
        noObstacle = true;

      } else if (distancia > mejorDistancia && !noObstacle) {  // Si no existe una direccion sin obstaculos, opta por dirigirse a la que haya detectado una mayor distancia con el obstaculo
        mejorDistancia = distancia;
        mejorAngulo = angulo;
      }
      lastObservedTime = actualTime;
      angulo--;
      Serial.println("Analizando caminos D...");
      isPetting = (interaccionObj->*callback)();
    }
  }

  // En el caso de que no haya encontrado un mejor camino por el lado derecho, mira su lado izquierdo.
  if (mejorAngulo == 90) {
    // Observa lado izquierdo
    while (angulo <= anguloFinal && !isPetting) {
      actualTime = millis();
      if ((actualTime - lastObservedTime) >= observeCompare) {
        neck.write(angulo);
        int distancia = getFilteredDistance();
        // El mejor caso es en el que no encuentra obstaculos en alguna direccion
        if ((distancia == maxDistance || distancia == 0) && !noObstacle) {
          mejorDistancia = distancia;
          mejorAngulo = angulo;
          noObstacle = true;
        } else if (distancia > mejorDistancia && !noObstacle) {  // Si no existe una direccion sin obstaculos, opta por dirigirse a la que haya detectado una mayor distancia con el obstaculo
          mejorDistancia = distancia;
          mejorAngulo = angulo;
        }
        lastObservedTime = actualTime;
        angulo++;
        isPetting = (interaccionObj->*callback)();
      }
      Serial.println("Analizando caminos I...");
    }
  }

  Serial.println("El mejor angulo es: ");
  Serial.println(mejorAngulo);
  Serial.println("La mejor distancia es: ");
  Serial.println(mejorDistancia);
  // Usa booleano isPetting para chequear si se ejecutó la interrupción de la caricia. Si no se interrumpio el analisis, la cabeza gira a la mejor pos analizada
  if (isPetting) {
    neck.write(90);
    mejorAngulo = 90;
  }

  // Si se encuentra encerrado debe retroceder y pegar la vuelta o analizar desde otra posicion
  if (mejorDistancia <= OBSTACLE_DISTANCE && mejorDistancia != 0) {
    Serial.println("Debe retroceder.");
    *fallback = true;
  }
  mustAnalize = false;
  bestAngle = mejorAngulo;
}


void Cabeza::analize(bool* isPetting) {
  // Booleano modificado desde la detección por la interrupcion del Timer2 en el metodo echoCheck
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
    if (timeLapse()) {
      neck.write(angulo);
      int distancia = getFilteredDistance();
      // El mejor caso es en el que no encuentra obstaculos en alguna direccion
      if ((distancia == maxDistance || distancia == 0) && !noObstacle) {
        mejorDistancia = distancia;
        mejorAngulo = angulo;
        noObstacle = true;

      } else if (distancia > mejorDistancia && !noObstacle) {  // Si no existe una direccion sin obstaculos, opta por dirigirse a la que haya detectado una mayor distancia con el obstaculo
        mejorDistancia = distancia;
        mejorAngulo = angulo;
      }
      lastObservedTime = actualTime;
      angulo--;
      Serial.println("Analizando caminos D...");
    }
  }

  // En el caso de que no haya encontrado un mejor camino por el lado derecho, mira su lado izquierdo.
  if (mejorAngulo == 90) {
    // Observa lado izquierdo
    while (angulo <= anguloFinal && !*isPetting) {
      actualTime = millis();
      if (timeLapse()) {
        neck.write(angulo);
        int distancia = getFilteredDistance();
        // El mejor caso es en el que no encuentra obstaculos en alguna direccion
        if ((distancia == maxDistance || distancia == 0) && !noObstacle) {
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
      Serial.println("Analizando caminos I...");
    }
  }

  Serial.println("El mejor angulo es: ");
  Serial.println(mejorAngulo);
  Serial.println("La mejor distancia es: ");
  Serial.println(mejorDistancia);
  // Usa booleano isPetting para chequear si se ejecutó la interrupción de la caricia. Si no se interrumpio el analisis, la cabeza gira a la mejor pos analizada
  if (*isPetting) {
    neck.write(90);
    mejorAngulo = 90;
  }

  // Si se encuentra encerrado debe retroceder y pegar la vuelta o analizar desde otra posicion
  if (mejorDistancia <= OBSTACLE_DISTANCE && mejorDistancia != 0) {
    Serial.println("Debe retroceder.");
  }
  mustAnalize = false;
  bestAngle = mejorAngulo;
}

// Método para filtrar la distancia utilizando la mediana
int Cabeza::getFilteredDistance() {
  int readings[NUM_READINGS];

  // Realiza múltiples lecturas
  for (int i = 0; i < NUM_READINGS; i++) {
    readings[i] = eyes.ping_cm();
    delay(10);  // Pequeña espera entre lecturas
  }

  // Ordena las lecturas
  sortArray(readings, NUM_READINGS);

  // Retorna el valor mediano
  return readings[NUM_READINGS / 2];
}

// Método para ordenar un array
void Cabeza::sortArray(int* arr, int size) {
  for (int i = 0; i < size - 1; i++) {
    for (int j = i + 1; j < size; j++) {
      if (arr[i] > arr[j]) {
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
      }
    }
  }
}

int Cabeza::getBestAngle() {
  return bestAngle;
}

float Cabeza::getDistancia() {
  return eyes.ping_cm();
}

void Cabeza::headPettingMovement(bool (Interaccion::*callback)(), Interaccion* interaccionObj) {
  int offset = random(0, 15);
  bool isPetting = (interaccionObj->*callback)();  // Llamada al método pasado por parametro
  // if(alternar){} else{}
  int i = neck.read();
  while (i < (LIMITE_SUP + offset) && isPetting) {
    i++;
    neck.write(i);
    isPetting = (interaccionObj->*callback)();
    delay(25);
  }
  while (i > LIMITE_INF && isPetting) {
    i--;
    neck.write(i);
    isPetting = (interaccionObj->*callback)();
    delay(10);
  }
}

bool Cabeza::timeLapse(){
  return ((millis() - lastObservedTime) >= observeCompare);
}

void Cabeza::setMoveTimer(unsigned long value) {
  this->observeCompare = value;
}
