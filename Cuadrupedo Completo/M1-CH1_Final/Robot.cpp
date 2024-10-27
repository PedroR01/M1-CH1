#include "Robot.h"

#define MIN_RANDOM 10000  // 10 segundos
#define MAX_RANDOM 20000  // 20 segundos
#define MIN_STOP 60000    // 1 min
#define MAX_STOP 120000   // 2 min

Robot::Robot() {
  // La inicializacion se hará en begin()
}

void Robot::begin() {
  interaccion.begin();
  cabeza.begin();
  piernas.begin();
  randomSeed(analogRead(0));  // Siempre hay que conectar un pin que no este conectado al randomSeed

  currentTime = millis();
  lastStopedTime = millis();

  randomStopTimer = random(MIN_STOP, MAX_STOP);
  randomActionTimer = random(MIN_RANDOM, MAX_RANDOM);

  fallback = false;
}

void Robot::path() {

  // Comportamiento acorde a la deteccion de petting
  checkPetting();

  // Movimiento
  move();

  // Caso de que se interrumpa por un obstaculo -- Analisis entorno
  cabeza.analize(&Interaccion::checkInterruptPetting, &interaccion, &fallback);
  checkPetting();

  // Girar y orientar hacia el mejor camino detectado sin obstaculos
  int newAnglePath = cabeza.getBestAngle();
  // Si el nuevo camino no es seguir adelante || O si no se interrumpio el analisis por caricia
  if (newAnglePath <= 85 || newAnglePath >= 95) {
    // si es esta en modo gato
    if (interaccion.isInCatMode()) {
      int direction;
      int steps;
      // Hacia donde debe girar
      if (newAnglePath <= 85) {
        direction = 0;
        map(steps, 85, 30, 1, 4);
        // A medida que el angulo sea cada vez más cerca de 0, se incrementan la cantidad de pasos necesarios para la rotacion
      } else if (newAnglePath >= 95) {
        direction = 1;
        map(steps, 95, 150, 1, 5);
      }

      // Añadir la cantidad de pasos necesarios para la rotacion como parametro del metodo.
      piernas.rotateCat(direction, steps, interaccion.getIsPetting());
    }
    // si esta en modo robot
    else {
      // Marcha atras
      piernas.backwards(&Interaccion::checkInterruptPetting, &interaccion);
    }
  }


  // Comportamiento acorde a la deteccion de petting
  checkPetting();

  // Posibles acciones randoms del gato
  int mustAct = random(0, 10);  // de 0 a 3 quiere decir que hace algo random
  if (((millis() - currentTime) > randomActionTimer) && mustAct <= 3) {
    int randomAction = random(0, 4);  // 4 acciones posibles
    // Si es gato
    if (interaccion.isInCatMode()) {
      // Dar la pata
      if (randomAction == 0 || randomAction == 2)
        piernas.giveHand();
      // O sentarse
      else
        piernas.sitDown();
    }
    // Si no es gato
    else {
      // Pararse en 2 patas
      if (randomAction == 0 || randomAction == 2)
        piernas.standInTwoLegs();
      // O quedarse quieto en una posicion rara + luz led encendida
      else
        Serial.println("Accion que falta");
    }
    // En cualquier caso también puede acostarse (segun su estado lo hace de forma distinta)
    if (randomAction == 4)
      piernas.lieDown();

    currentTime = millis();
    Serial.print("Se ejecutó la acción random: ");
    Serial.println(randomAction);
  }

  // Comportamiento acorde a la deteccion de petting
  checkPetting();

  // El robot puede llegar a parar de ejecutarse
  stopRobot(interaccion.getIsPetting());
}

void Robot::pingIntCheck() {
  cabeza.pingInterruptionCheck();
}

void Robot::move() {
  if (interaccion.isInCatMode())
    piernas.catMovementWCb(&Interaccion::checkInterruptPetting, &interaccion);
  else
    piernas.botMovementWCb(&Interaccion::checkInterruptPetting, &interaccion);
}

void Robot::setCheckState(bool value) {
  //interaccion.setCheckPetting(value);
  //interaccion.interruptSetPetting();
}

void Robot::checkPetting() {
  // Verificar si se está acariciando.
  interaccion.petting();
  if (*interaccion.getIsPetting() && interaccion.isInCatMode()) {
    piernas.sitDown();
    cabeza.headPettingMovement(&Interaccion::checkInterruptPetting, &interaccion);
    piernas.setMoveTimer(30);
    cabeza.setMoveTimer(30);
    interaccion.petting();
  } else if (!interaccion.isInCatMode()) {
    piernas.setMoveTimer(4);
    cabeza.setMoveTimer(4);
  }
}

void Robot::stopRobot(bool* isPetting) {
  // Mientras antes se lo acaricie, menos "frecuente" se parará el robot.
  if ((millis() - lastStopedTime) > randomStopTimer) {
    while (!*isPetting) {
      checkPetting();
      Serial.println("Robot en estado crítico. Acariciar!");
    }
    lastStopedTime = millis();
  }
}
