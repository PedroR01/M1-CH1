#include "Cabeza.h"

#define LIMITE_SUP 110
#define LIMITE_INF 70
#define OBSTACLE_DISTANCE 25
#define ROBOT_WIDTH 25
#define NUM_READINGS 5  // Número de lecturas para el filtro de lecturas del Ultrasonico(US)

Cabeza* Cabeza::instanciaActual = nullptr;  // Inicializar el puntero estático

Cabeza::Cabeza() {
  // La inicializacion se hará en begin()
  instanciaActual = this;
}

void Cabeza::begin() {
  neck.attach(neckPin);
  neck.write(centeredPos);

  bestAngle = -1;
  pingTimer = millis();
}

void Cabeza::pingInterruptionCheck() {
  if (millis() >= pingTimer) {  // pingSpeed milliseconds since last ping, do another ping.
    pingTimer += pingSpeed;     // Set the next ping time.

    // Escribir el codigo acá no es para nada lo más optimo, convendría manejarlo en el echoCheck
    //Serial.println(eyes.ping_cm());
    if (!mustAnalize) {
      int distancia = getFilteredDistance();
      if (distancia <= OBSTACLE_DISTANCE && distancia != 0) {
        mustAnalize = true;
      }
    }
    //eyes.ping_timer(Cabeza::echoCheck);  // Send out the ping, calls "echoCheck" function every 24uS where you can check the ping status.
  }
}

void Cabeza::echoCheck() {  // Timer2 interrupt calls this function every 24uS where you can check the ping status.
  // Don't do anything here!
  if (instanciaActual->eyes.check_timer()) {  // This is how you check to see if the ping was received.
                                              // Here's where you can add code.

    // Analizar si hay algun obstaculo enfrente.
    /*
    // De esta forma no se genera una interferencia de señales entre la interrupcion y los metodos, algo que si pasa si se usa ping_cm
    // Por ende, con el resultado del ping y su conversion manual a CM habría que hacer un metodo que filtre un promedio de señales para eliminar los ruidos.
    Serial.print("Ping: ");
    Serial.print(instanciaActual->eyes.ping_result / US_ROUNDTRIP_CM); // Ping returned, uS result in ping_result, convert to cm with US_ROUNDTRIP_CM.
    Serial.println("cm");
    */
    /*
    int distancia = instanciaActual->getFilteredDistance();
      Serial.println(instanciaActual->eyes.ping_cm());
    if (distancia <= OBSTACLE_DISTANCE && instanciaActual->eyes.ping_cm() != 0){
      //instanciaActual->mustAnalize = true;
    }
    */
  }
  // Don't do anything here!
}

void Cabeza::analize(bool (Interaccion::*callback)(), Interaccion* interaccionObj, bool* fallback) {
  // Booleano modificado desde la detección por la interrupcion del Timer2 en el metodo echoCheck
  if (this->mustAnalize) {
    actualTime = millis();
    lastObservedTime = actualTime;
    // Segundo chequeo de obstaculos para evitar ruidos en la señal
    int distancia = getFilteredDistance();
    if (distancia <= OBSTACLE_DISTANCE && distancia != 0) {
      Serial.print("Obstaculo detectado a: ");
      Serial.println(distancia);
      if (distancia <= 6) {
        *fallback = true;
      } else
        seeArround(callback, interaccionObj, fallback);
    }
    mustAnalize = false;
  }
}

// Debido a la impresición del ultrasonico para captar los obstaculos, ya sea por como se implementa
// o por su prespectiva/posición desde la estructura del modelo 3D, se simplifica el codigo de "mejor Camino posible"
// a "si adelante hay un obstaculo, revisar izquierda, si en izquierda también, revisar derecha".
// Tal vez, para solventar esta falencia, sería buena solución usar sensores Infrarrojos de obstaculos a los costados del cuello.
void Cabeza::seeArround(bool (Interaccion::*callback)(), Interaccion* interaccionObj, bool* fallback) {
  int anguloDerecho = 40;
  int anguloIzquierdo = 140;
  int mejorAngulo = -1;
  int mejorDistancia = 1;          // 0 representa que no hay obstaculos captados en el rango que permite el US o para el que se configuró.
  int offsetMarginDistance = 2;    // Para tener en cuenta en el caso de que haya algun obstaculo en algún costado que este apenas más lejos que el obstaculo detectado en frente.
  int noObstacleVerification = 0;  // Contador para corroborar que no hay ningun obstaculo en la zona analizada.

  // Mueve la cabeza para detectar el entorno
  int angulo = centeredPos;
  bool noObstacle = false;
  bool isPetting = (interaccionObj->*callback)();

  // Detecta algo enfrente.
  // Mira a la izquierda --> Si detecta algo (con el filtrado de la señal) en el rango de obstaculo
  // Mira a la derecha --> Si detecta algo (con el filtrado de la señal) en el rango de obstaculo
  // Vuelve X cant de pasos hacía atras
  // Vuelve a escanear todo y repite el proceso la cant de veces que sea necesario O lo hace 1 vez más y ya luego se detiene esperando a que lo muevan de lugar.
  // Si no detecta nada, gira a la derecha, se pone con el cuello derecho y luego sigue su rutina
  // Si no detecta nada, gira a la izquierda, se pone con el cuello derecho y luego sigue su rutina

  // Observa lado izquierdo
  while (angulo <= anguloIzquierdo && !isPetting && !noObstacle) {
    actualTime = millis();
    if ((actualTime - lastObservedTime) >= observeCompare) {
      neck.write(angulo);
      int distancia = getFilteredDistance();
      if (distancia == 0 || distancia >= (OBSTACLE_DISTANCE + offsetMarginDistance)) {
        noObstacleVerification += 1;
        Serial.println(noObstacleVerification);
        if (noObstacleVerification >= ROBOT_WIDTH) {
          noObstacle = true;
          mejorAngulo = angulo;
        }
      } else if (distancia <= OBSTACLE_DISTANCE) {
        noObstacleVerification = 0;  // Me conviene reiniciar el contador o restarlo en 1, por si solamente captó una señal erronea/equivocada?
      }
      /*
      // El mejor caso es en el que no encuentra obstaculos en alguna direccion
      if ((distancia == maxDistance || distancia == 0) && !noObstacle) {
        mejorDistancia = distancia;
        mejorAngulo = angulo;
        noObstacle = true;
      } else if (distancia > mejorDistancia && !noObstacle) {  // Si no existe una direccion sin obstaculos, opta por dirigirse a la que haya detectado una mayor distancia con el obstaculo
        mejorDistancia = distancia;
        mejorAngulo = angulo;
      }
      */
      lastObservedTime = actualTime;
      // Si todavía detecta obstaculos
      if (!noObstacle) {
        angulo++;
        isPetting = (interaccionObj->*callback)();
      }
    }
  }

  noObstacleVerification = 0;  // Reinicio el contador de verificación para analizar el lado derecho

  // En el caso de que no haya un camino libre por la izquierda
  if (!noObstacle) {  // mejorAngulo == 90
    // Observa lado derecho
    while (angulo >= anguloDerecho && !isPetting && !noObstacle) {
      actualTime = millis();
      if ((actualTime - lastObservedTime) >= observeCompare) {
        neck.write(angulo);
        int distancia = getFilteredDistance();
        if (distancia == 0 || distancia > (OBSTACLE_DISTANCE + offsetMarginDistance)) {
          // Verifica el espacio para moverse una vez que el cuello rota hacia la mitad derecha.
          if (angulo < centeredPos) {
            noObstacleVerification += 1;
            Serial.println(noObstacleVerification);
            if (noObstacleVerification >= ROBOT_WIDTH) {
              noObstacle = true;
              mejorAngulo = angulo;
            }
          }
        } else if (distancia <= OBSTACLE_DISTANCE) {
          noObstacleVerification = 0;  // Me conviene reiniciar el contador o restarlo en 1, por si solamente captó una señal erronea/equivocada?
        }
        /*
        // El mejor caso es en el que no encuentra obstaculos en alguna direccion
        if ((distancia == maxDistance || distancia == 0) && !noObstacle) {
          mejorDistancia = distancia;
          mejorAngulo = angulo;
          noObstacle = true;

        } else if (distancia > mejorDistancia && !noObstacle) {  // Si no existe una direccion sin obstaculos, opta por dirigirse a la que haya detectado una mayor distancia con el obstaculo
          mejorDistancia = distancia;
          mejorAngulo = angulo;
        }
        */
        lastObservedTime = actualTime;
        // Si todavía detecta obstaculos
        if (!noObstacle) {
          angulo--;
          isPetting = (interaccionObj->*callback)();
        }
      }
    }
  }

  Serial.println("El mejor angulo es: ");
  Serial.println(mejorAngulo);

  // Usa booleano isPetting para chequear si se ejecutó la interrupción de la caricia. En dicho caso se resetean los valores como para repetir el analisis
  if (isPetting || mejorAngulo == -1) {
    neck.write(centeredPos);
    mejorAngulo = -1;
    noObstacle = false;
  }


  // Si se encuentra encerrado debe retroceder y pegar la vuelta o analizar desde otra posicion
  if (mejorAngulo == -1) {
    *fallback = true;
  }
  /*
  if ((mejorDistancia <= OBSTACLE_DISTANCE && mejorDistancia != 0) || !noObstacle) {
    Serial.println("Debe retroceder.");
    *fallback = true;
  }
*/

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
  int anguloDerecho = 30;
  int anguloIzquierdo = 150;
  int paso = 5;
  int mejorAngulo = 90;
  int mejorDistancia = 0;

  // Mueve la cabeza para detectar el entorno
  int angulo = mejorAngulo;
  bool noObstacle = false;

  // Observa lado derecho
  while (angulo >= anguloDerecho && !*isPetting) {
    actualTime = millis();
    if (timeLapse()) {
      neck.write(angulo);
      int distancia = getFilteredDistance();
      Serial.print(distancia);
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
    }
  }

  // En el caso de que no haya encontrado un mejor camino por el lado derecho, mira su lado izquierdo.
  if (mejorAngulo == 90) {
    // Observa lado izquierdo
    while (angulo <= anguloIzquierdo && !*isPetting) {
      actualTime = millis();
      if (timeLapse()) {
        neck.write(angulo);
        int distancia = getFilteredDistance();
        Serial.print(distancia);
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

void Cabeza::resetBestAngle() {
  this->bestAngle = -1;
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

bool Cabeza::timeLapse() {
  return ((millis() - lastObservedTime) >= observeCompare);
}

void Cabeza::setMoveTimer(unsigned long value) {
  this->observeCompare = value;
}

void Cabeza::centerNeck() {
  neck.write(centeredPos);
}
