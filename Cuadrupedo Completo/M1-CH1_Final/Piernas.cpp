#include "Piernas.h"

Piernas::Piernas() {
  // La inicializacion se hará en begin()
}

void Piernas::begin() {
  backL.attach(backLPin);
  backR.attach(backRPin);
  frontL.attach(frontLPin);
  frontR.attach(frontRPin);

  standUp();

  lastMoveTime = millis();
}

void Piernas::standUp() {
  backL.write(100);   // 80 derecho - 160 atras - 35 adelante
  backR.write(90);    // 100 derecho - 15 atras - 170 adelante
  frontL.write(70);   // 70 derecho - 150 atras - 20 adelante
  frontR.write(100);  // 100 derecho - 40 atras - 170 adelante
}

void Piernas::lieDown() {

  const int adelanteIzq = 0;
  const int adelanteDer = 180;
  const int atrasIzq = 170;
  const int atrasDer = 20;

  // modo robot
  backL.write(15);
  backR.write(170);
  //frontL.write(atrasIzq);
  //frontR.write(atrasDer);

  //modo gato + ANIMACION SUAVE
  frontL.write(adelanteIzq);
  frontR.write(adelanteDer);
}

// Cuando se acuesta y detecta algo delante que se aproxima, hace este movimiento como para que le hagan mimos?
void Piernas::sitDown() {
  while (backL.read() != 15 && backR.read() != 170) {
    if (timeLapse()) {
      if (backL.read() != 15) {
        int i = backL.read();
        i--;
        backL.write(i);
      }
      if (backR.read() != 170) {
        int i = backR.read();
        i++;
        backR.write(i);
      }
      lastMoveTime = millis();
    }
  }
  delay(100);
  frontL.write(70);
  frontR.write(100);
}

void Piernas::standInTwoLegs() {
  lieDown();  // Deja backL en 15 y backR en 170
  // Habría que hacerlo con una animacion más sutil para que el impulso del movimiento repentino no lo desestabilice
  delay(200);
  while (backL.read() != 100 && backR.read() != 90) {
    if (timeLapse()) {
      if (backL.read() != 100) {
        int i = backL.read();
        i++;
        backL.write(i);
      }
      if (backR.read() != 90) {
        int i = backR.read();
        i--;
        backR.write(i);
      }
      lastMoveTime = millis();
    }
  }
}

void Piernas::giveHand() {
  while (backL.read() != 70 && backR.read() != 110) {
    if (timeLapse()) {

      // Dependiendo de la pos de la trasera izquierda es hacia donde tiene que moverse.
      if (backL.read() > 70) {
        int i = backL.read();
        i--;
        backL.write(i);
      } else if (backL.read() < 70) {
        int i = backL.read();
        i++;
        backL.write(i);
      }

      // Dependiendo de la pos de la trasera derecha es hacia donde tiene que moverse.
      if (backR.read() < 110) {
        int i = backR.read();
        i++;
        backR.write(i);
      } else if (backR.read() > 110) {
        int i = backR.read();
        i--;
        backR.write(i);
      }
      lastMoveTime = millis();
    }
  }

  delay(400);

  while (frontL.read() != 0 && frontR.read() != 100) {
    if (timeLapse()) {

      if (frontL.read() != 0) {
        int i = frontL.read();
        i--;
        frontL.write(i);
      }

      // Dependiendo de la pos de la delanter derecha es hacia donde tiene que moverse.
      if (frontR.read() < 100) {
        int i = frontR.read();
        i++;
        frontR.write(i);
      } else if (frontR.read() > 100) {
        int i = frontR.read();
        i--;
        frontR.write(i);
      }
      lastMoveTime = millis();
    }
  }
}

void Piernas::rotateCat(int direccion, int cant, bool* isPetting) {
  int girando = cant;

  //1
  standUp();
  //2
  if (!*isPetting) {
    if (direccion == 1) {
      frontL.write(50);
      delay(1000);
      // Gira hacia la izquierda
      while (girando && !*isPetting) {
        backR.write(130);
        frontR.write(130);
        delay(1000);
        backR.write(60);
        frontR.write(60);
        delay(1000);
        girando--;
        Serial.println("Girando a la izquierda");
      }
    } else {
      frontR.write(110);
      backR.write(105);
      delay(1000);
      // Gira hacia la derecha
      while (girando && !*isPetting) {
        backL.write(65);
        frontL.write(55);
        delay(1000);
        backL.write(110);
        frontL.write(100);
        delay(1000);
        girando--;
        Serial.println("Girando a la derecha");
      }
    }
  }

  //3
  standUp();
  delay(3000);
}

void Piernas::backwards(bool (Interaccion::*callback)(), Interaccion* interaccionObj) {
  int retrocederDistancia = 40;
  standUp();
  bool isPetting = (interaccionObj->*callback)();
  delay(1000);
  if (!isPetting) {
    frontL.write(frontL.read() + retrocederDistancia);
    frontR.write(frontR.read() - retrocederDistancia);
    delay(1000);  // Espera para completar la maniobra de retroceso
    isPetting = (interaccionObj->*callback)();
    if (!isPetting) {
      frontL.write(frontL.read() - (retrocederDistancia * 1.5));
      frontR.write(frontR.read() + (retrocederDistancia * 1.5));
      delay(1000);
    }
  }
}

void Piernas::botMovementWCb(bool (Interaccion::*callback)(), Interaccion* interaccionObj) {
  standUp();
  delay(500);
  bool isPetting = (interaccionObj->*callback)();
  if (!isPetting) {
    backL.write(100);
    backR.write(90);
    frontL.write(40);
    frontR.write(130);
    delay(500);
    backL.write(140);
    backR.write(50);
    frontL.write(70);
    frontR.write(100);
    delay(500);
  }
}

void Piernas::catMovementWCb(bool (Interaccion::*callback)(), Interaccion* interaccionObj) {
  int posTD = 130;  // Pierna recta
  int ultimoValorPosTD = posTD;
  int posDD = 130;  // Pierna recta

  int posTI = 150;  // Pierna hacia atras
  int ultimoValorPosTI = posTI;
  int posDI = 100;  // Pierna ligeramente hacia atras

  int offsetDelantera = 2;  // Diferencia de tiempo de actualizacion respecto a la pierna trasera.

  bool isPetting = (interaccionObj->*callback)();
  // Primera parte del movimiento
  while ((posTD > 45 || posTI > 65) && !isPetting) {  // Mientras la trasera D no esté atras O la trasera I no esté adelante
    /*if (timeLapse()) {

      lastMoveTime = millis();
    }*/
    if (posTD > 45) {
      backR.write(posTD);
      posTD--;
      if (posTD < ultimoValorPosTD - offsetDelantera) {
        frontR.write(posDD);
        posDD--;
        ultimoValorPosTD = posTD;
      }
    }
    if (posTI > 65) {
      backL.write(posTI);
      if (posTD <= 45)
        posTI -= 2;
      else
        posTI--;
      if (posTI < ultimoValorPosTI - 1) {
        frontL.write(posDI);
        posDI--;
        ultimoValorPosTI = posTI;
      }
    }
    if (posTD > 70)
      delay(10);
    else
      delay(30);

    isPetting = (interaccionObj->*callback)();
  }

  ultimoValorPosTD = posTD;
  ultimoValorPosTI = posTI;

  // Segunda parte del movimiento
  while ((posTD < 125 || posTI < 130) && !isPetting) {  // Mientras la trasera D no esté adelante O la trasera I no esté atras
    if (posTD < 125) {
      backR.write(posTD);
      posTD++;
      if (posTD > ultimoValorPosTD + offsetDelantera) {
        frontR.write(posDD);
        if (backR.read() >= 110) {  // 135/2
          posDD += offsetDelantera + 1;
        } else
          posDD--;
        ultimoValorPosTD = posTD;
      }
    }
    if (posTI < 130) {
      backL.write(posTI);
      posTI++;
      if (posTI > ultimoValorPosTI + 1) {
        frontL.write(posDI);
        if (backL.read() >= 15) {
          posDI++;
        } else
          posDI--;
        ultimoValorPosTI = posTI;
      }
    }
    if (posTD < 70)
      delay(10);
    else
      delay(30);
    isPetting = (interaccionObj->*callback)();
  }
}

bool Piernas::timeLapse() {
  return ((millis() - lastMoveTime) > moveTimer);
}

void Piernas::setMoveTimer(unsigned long value) {
  this->moveTimer = value;
}