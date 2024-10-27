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

  actualTime = 0;
  lastExecutedMov = 0;
  side = 0;
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
  lieDown();
  delay(1000);

  frontL.write(70);
  frontR.write(100);
}

void Piernas::standInTwoLegs() {
  lieDown();
  // Habría que hacerlo con una animacion más sutil para que el impulso del movimiento repentino no lo desestabilice
  backL.write(100);
  backR.write(90);
}

void Piernas::giveHand() {
  backL.write(70);
  backR.write(110);
  delay(400);
  frontL.write(0);
  frontR.write(100);
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

void Piernas::backwards(bool* isPetting) {
  int retrocederDistancia = 40;
  standUp();
  delay(1000);
  if (!isPetting) {
    //backL.write(backL.read() + retrocederDistancia);
    //backR.write(backR.read() - retrocederDistancia);
    frontL.write(frontL.read() + retrocederDistancia);
    frontR.write(frontR.read() - retrocederDistancia);
    delay(2000);  // Espera para completar la maniobra de retroceso
    frontL.write(frontL.read() - (retrocederDistancia * 1.5));
    frontR.write(frontR.read() + (retrocederDistancia * 1.5));
    delay(1000);
  }
}


void Piernas::botMovement(bool* isPetting) {
  standUp();
  delay(500);
  if (!*isPetting) {
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

// tras Izquierda sube muucho y baja un poco de mas
// tras Derecha sube mucho
void Piernas::catMovement(bool* isPetting) {

  int posTD = 130;  // Pierna recta
  int ultimoValorPosTD = posTD;
  int posDD = 130;  // Pierna recta

  int posTI = 150;  // Pierna hacia atras
  int ultimoValorPosTI = posTI;
  int posDI = 100;  // Pierna ligeramente hacia atras

  int offsetDelantera = 2;  // Diferencia de tiempo de actualizacion respecto a la pierna trasera.

  // Primera parte del movimiento
  while (posTD > 45 || posTI > 65) {  // Mientras la trasera D no esté atras O la trasera I no esté adelante
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
  }

  ultimoValorPosTD = posTD;
  ultimoValorPosTI = posTI;

  // Segunda parte del movimiento
  while (posTD < 125 || posTI < 130) {  // Mientras la trasera D no esté adelante O la trasera I no esté atras
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
  }
}

void Piernas::botMovementWCb(bool(Interaccion::*callback)(), Interaccion* interaccionObj){
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

void Piernas::catMovementWCb(bool(Interaccion::*callback)(), Interaccion* interaccionObj){
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