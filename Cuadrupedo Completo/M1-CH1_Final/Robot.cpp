#include "Robot.h"

#define MIN_RANDOM 20000  // 20 segundos
#define MAX_RANDOM 30000  // 30 segundos
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

  cabeza.analize(&Interaccion::checkInterruptPetting, &interaccion, &fallback);
  checkPetting();
  takeAnotherPath();
  checkPetting();

  cabeza.centerNeck();
  // Movimiento
  move();
  checkPetting();


  // Caso de que se interrumpa por un obstaculo -- Analisis entorno
  checkPetting();
  cabeza.analize(&Interaccion::checkInterruptPetting, &interaccion, &fallback);
  checkPetting();
  takeAnotherPath();
  checkPetting();


  cabeza.centerNeck();
  checkPetting();
  cabeza.analize(&Interaccion::checkInterruptPetting, &interaccion, &fallback);
  checkPetting();
  takeAnotherPath();
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
    // En cualquier caso también puede acostarse (según su estado lo hace de forma distinta)
    if (randomAction == 4)
      piernas.lieDown(catState);

    // Establece un nuevo tiempo y límite aleatorio
    currentTime = millis();
    randomActionTimer = random(MIN_RANDOM, MAX_RANDOM);  // Nuevo límite entre acciones
    Serial.print("Se ejecutó la acción random: ");
    Serial.println(randomAction);
    delay(2000);
  }

  // Comportamiento acorde a la deteccion de petting
  checkPetting();

  // El robot puede llegar a parar de ejecutarse
  stopRobot();
  checkPetting();
  cabeza.analize(&Interaccion::checkInterruptPetting, &interaccion, &fallback);
  checkPetting();
  takeAnotherPath();

}

void Robot::pingIntCheck() {
  cabeza.pingInterruptionCheck();
}

void Robot::move() {
  if (interaccion.isInCatMode())
    piernas.catMovementAlt(&Interaccion::checkInterruptPetting, &interaccion);
  else
    piernas.botMovementAlt(&Interaccion::checkInterruptPetting, &interaccion);
}

void Robot::setCheckState(bool value) {
  //interaccion.setCheckPetting(value);
  //interaccion.interruptSetPetting();
}

void Robot::checkPetting() {
  // Verificar si se está acariciando.
  interaccion.petting();
  while (*interaccion.getIsPetting()) {
    if (interaccion.isInCatMode()) {
      piernas.sitDown();
      cabeza.headPettingMovement(&Interaccion::checkInterruptPetting, &interaccion);
      // En el caso de que alcance el modo gato, establece la velocidad de los movimientos a otra más lenta.
      piernas.setMoveTimer(25);
      cabeza.setMoveTimer(25);
    } else {
      // En el caso de que alcance el modo gato, establece la velocidad de los movimientos a otra más rápida y explosiva.
      piernas.setMoveTimer(4);
      cabeza.setMoveTimer(4);
    }
    interaccion.petting();
  }

  delay(200);
  piernas.standUp();
}

void Robot::stopRobot() {
  // Mientras antes se lo acaricie, menos "frecuente" se parará el robot.
  if ((millis() - lastStopedTime) > randomStopTimer) {
    bool isPetting = interaccion.checkInterruptPetting();
    while (!isPetting) {
      piernas.lieDown(interaccion.isInCatMode());
      isPetting = interaccion.checkInterruptPetting();
    }
    lastStopedTime = millis();
    delay(1000);
  }
}

void Robot::takeAnotherPath() {
  // Mantener el codigo de la rotacion. Quitar que solo rote en modo gato.
  // En el caso de tener que retroceder, volver a ejecutar el analisis una vez que haya retrocedido
  // Si debe volver a retroceder, entonces "apagar" gato hasta que deje de detectar un obstaculo (por ende alguien lo tiene que mover) + añadir un temporizador o la espera de una caricia para reanudar el recorrido.

  // Medio raro como es este bloque de codigo para limitar la cant de veces que retrocede y su "desconexión" en caso extremo. Lo podría resolver con un while tranquilamente.
  if (fallback) {
    Serial.println("marcha atras");
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
        stopRobot();
      }
    }
  }
  int newAnglePath = cabeza.getBestAngle();
  if (newAnglePath != -1) {
    // Girar y orientar hacia el mejor camino detectado sin obstaculos
    if (newAnglePath != -1) {  // Puede ser que este if sea innecesario a causa del else de arriba.
      int direction;           // cambiar por un booleano goLeft, es mas entendible
      int steps;

      // Hacia donde debe girar y la cantidad de pasos para el giro. A medida que el angulo se acerca más al extremo, se incrementan la cantidad de pasos necesarios para la rotacion
      if (newAnglePath <= 75) {
        direction = 0;
        steps = map(newAnglePath, 75, 40, 3, 6);
      } else if (newAnglePath >= 115) {  // Este condicional puede ser redundante a causa del condicional en el que están contenidos
        direction = 1;
        steps = map(newAnglePath, 115, 140, 3, 6);
      }
      piernas.rotateCat(direction, steps, interaccion.getIsPetting());  // Esta acción no puede ser interrumpida por las caricias.
    }
    cabeza.resetBestAngle();
  }

  // QUE PASA SI ANALIZA Y ES -1 TODAVIA? NO HIZO EL FALLBACK? SE ATASCÓ?
}
