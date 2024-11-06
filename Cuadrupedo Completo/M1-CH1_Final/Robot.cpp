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

  // Mantener el codigo de la rotacion. Quitar que solo rote en modo gato.
  // En el caso de tener que retroceder, volver a ejecutar el analisis una vez que haya retrocedido
  // Si debe volver a retroceder, entonces "apagar" gato hasta que deje de detectar un obstaculo (por ende alguien lo tiene que mover) + añadir un temporizador o la espera de una caricia para reanudar el recorrido.

  // Medio raro como es este bloque de codigo para limitar la cant de veces que retrocede y su "desconexión" en caso extremo. Lo podría resolver con un while tranquilamente.
  if (fallback) {
    piernas.backwards(&Interaccion::checkInterruptPetting, &interaccion);
    fallback = false;
    cabeza.analize(&Interaccion::checkInterruptPetting, &interaccion, &fallback);
    checkPetting();
    if (fallback) {
      piernas.backwards(&Interaccion::checkInterruptPetting, &interaccion);
      fallback = false;
      cabeza.analize(&Interaccion::checkInterruptPetting, &interaccion, &fallback);
      checkPetting();
      if (fallback) {
        stopRobot(&Interaccion::checkInterruptPetting, &interaccion);
      }
    }
  }
  int newAnglePath = cabeza.getBestAngle();
  if (newAnglePath <= 75 || newAnglePath >= 115) {
    // Girar y orientar hacia el mejor camino detectado sin obstaculos
    if (newAnglePath <= 75 || newAnglePath >= 115) {  // Puede ser que este if sea innecesario a causa del else de arriba.
      int direction;                                  // cambiar por un booleano goLeft, es mas entendible
      int steps;

      // Hacia donde debe girar y la cantidad de pasos para el giro. A medida que el angulo se acerca más al extremo, se incrementan la cantidad de pasos necesarios para la rotacion
      if (newAnglePath <= 75) {
        direction = 0;
        steps = map(newAnglePath, 75, 40, 1, 4);
      } else if (newAnglePath >= 115) {  // Este condicional puede ser redundante a causa del condicional en el que están contenidos
        direction = 1;
        steps = map(newAnglePath, 115, 140, 1, 5);
      }
      piernas.rotateCat(direction, steps, interaccion.getIsPetting());  // Esta acción no puede ser interrumpida por las caricias.
    }
  }

  /*
  // Si el nuevo camino no es seguir adelante || O si no se interrumpio el analisis por caricia
  if (newAnglePath <= 85 || newAnglePath >= 95) {
    // si es esta en modo gato
    if (interaccion.isInCatMode()) {
      int direction;
      int steps;
      // Hacia donde debe girar
      if (newAnglePath <= 85) {
        direction = 0;
        map(steps, 85, 40, 1, 4);
        // A medida que el angulo sea cada vez más cerca de 0, se incrementan la cantidad de pasos necesarios para la rotacion
      } else if (newAnglePath >= 95) {
        direction = 1;
        map(steps, 95, 140, 1, 5);
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
*/
  cabeza.centerNeck();
  checkPetting();

  // Posibles acciones randoms del gato
  int mustAct = random(0, 10);  // Si el valor random obtenido está entre 0 y 3, hace alguna de las acciones de abajo.
  if (((millis() - currentTime) > randomActionTimer) && mustAct <= 3) {
    int randomAction = random(0, 4);  // 4 acciones posibles
    // Si es gato
    bool catState = interaccion.isInCatMode();
    if (catState) {
      // Dar la pata
      if (randomAction == 0 || randomAction == 2)
        piernas.giveHand();
      // O sentarse
      else
        piernas.sitDown();
    }
    // Si no es gato
    else {
      piernas.weirdPose();
    }
    // En cualquier caso también puede acostarse (segun su estado lo hace de forma distinta)
    if (randomAction == 4)
      piernas.lieDown(catState);

    currentTime = millis();
    Serial.print("Se ejecutó la acción random: ");
    Serial.println(randomAction);
  }
  delay(1000);

  // Comportamiento acorde a la deteccion de petting
  checkPetting();

  // El robot puede llegar a parar de ejecutarse
  stopRobot(&Interaccion::checkInterruptPetting, &interaccion);
  checkPetting();
}

void Robot::pingIntCheck() {
  cabeza.pingInterruptionCheck();
}

void Robot::move() {
  /*
  if (interaccion.isInCatMode())
    piernas.catMovementWCb(&Interaccion::checkInterruptPetting, &interaccion);
  else
    piernas.botMovementWCb(&Interaccion::checkInterruptPetting, &interaccion);
    */

  //piernas.botMovementAlt(&Interaccion::checkInterruptPetting, &interaccion);
   piernas.catMovementAlt(&Interaccion::checkInterruptPetting, &interaccion);
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
    // En el caso de que alcance el modo gato, establece la velocidad de los movimientos a otra más lenta.
    piernas.setMoveTimer(25);
    cabeza.setMoveTimer(25);
    interaccion.petting();
  } else if (!interaccion.isInCatMode()) {
    // En el caso de que alcance el modo gato, establece la velocidad de los movimientos a otra más rápida y explosiva.
    piernas.setMoveTimer(4);
    cabeza.setMoveTimer(4);
  }
}

void Robot::stopRobot(bool (Interaccion::*callback)(), Interaccion* interaccionObj) {
  // Mientras antes se lo acaricie, menos "frecuente" se parará el robot.
  if ((millis() - lastStopedTime) > randomStopTimer) {
    bool isPetting = (interaccionObj->*callback)();
    while (!isPetting) {
      piernas.lieDown(interaccion.isInCatMode());
      Serial.println("Robot en estado crítico. Acariciar!");
      isPetting = (interaccionObj->*callback)();
      // checkPetting();
    }
    lastStopedTime = millis();
    delay(1000);
  }
}
