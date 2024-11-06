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

void Piernas::lieDown(bool catMode) {
  // FORMAS DE ACOSTARSE DIFERENTE ENTRE GATO Y ROBOT!!!
  if (catMode) {
    while (backL.read() != 15 || backR.read() != 170) {
      if (timeLapse()) {
        // Dependiendo de la pos de la trasera izquierda es hacia donde tiene que moverse.
        if (backL.read() > 15) {
          int i = backL.read();
          i--;
          backL.write(i);
        } else if (backL.read() < 15) {
          int i = backL.read();
          i++;
          backL.write(i);
        }

        // Dependiendo de la pos de la trasera derecha es hacia donde tiene que moverse.
        if (backR.read() < 170) {
          int i = backR.read();
          i++;
          backR.write(i);
        } else if (backR.read() > 170) {
          int i = backR.read();
          i--;
          backR.write(i);
        }

        if (frontL.read() > 0) {
          int i = frontL.read();
          i--;
          frontL.write(i);
        }

        if (frontR.read() < 180) {
          int i = frontR.read();
          i++;
          frontR.write(i);
        }

        lastMoveTime = millis();
      }
    }
  } else {
    while (backL.read() != 15 || backR.read() != 170) {
      if (timeLapse()) {
        // Dependiendo de la pos de la trasera izquierda es hacia donde tiene que moverse.
        if (backL.read() > 15) {
          int i = backL.read();
          i--;
          backL.write(i);
        } else if (backL.read() < 15) {
          int i = backL.read();
          i++;
          backL.write(i);
        }

        // Dependiendo de la pos de la trasera derecha es hacia donde tiene que moverse.
        if (backR.read() < 170) {
          int i = backR.read();
          i++;
          backR.write(i);
        } else if (backR.read() > 170) {
          int i = backR.read();
          i--;
          backR.write(i);
        }


        if (frontL.read() < 160) {
          int i = frontL.read();
          i++;
          frontL.write(i);
        }

        if (frontR.read() > 20) {
          int i = frontR.read();
          i--;
          frontR.write(i);
        }

        lastMoveTime = millis();
      }
    }
  }
}

// Por implementar: Cuando se acuesta y detecta algo delante que se aproxima, hace este movimiento como para que le hagan mimos?
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

// Con el cable y un poco de ayuda se llega a parar en 2 piernas, pero igualmente creo que no la voy a usar porque es mucho peso sobre los servos traseros,
// hacen un ruido como crujido cada vez que se para en 2 piernas.
void Piernas::standInTwoLegs() {
  while (backL.read() > 15 || backR.read() < 170) {
    if (timeLapse()) {
      if (backL.read() > 15) {
        int i = backL.read();
        i--;
        backL.write(i);
      }
      if (backR.read() < 170) {
        int i = backR.read();
        i++;
        backR.write(i);
      }
      lastMoveTime = millis();
    }
  }

  // Habría que hacerlo con una animacion más sutil para que el impulso del movimiento repentino no lo desestabilice
  delay(700);
  while (frontL.read() > 10 || frontR.read() < 170) {
    if (timeLapse()) {
      if (frontL.read() > 10) {
        int i = frontL.read();
        i--;
        frontL.write(i);
      }
      if (frontR.read() < 170) {
        int i = frontR.read();
        i++;
        frontR.write(i);
      }
      lastMoveTime = millis();
    }
  }

  while (backL.read() < 100 || backR.read() > 90) {
    if (timeLapse()) {
      if (backL.read() < 100) {
        int i = backL.read();
        i++;
        backL.write(i);
      }
      if (backR.read() > 90) {
        int i = backR.read();
        i--;
        backR.write(i);
      }
      lastMoveTime = millis();
    }
  }
}

void Piernas::giveHand() {

  while (backL.read() != 60 || backR.read() != 125) {
    if (timeLapse()) {
      // Dependiendo de la pos de la trasera izquierda es hacia donde tiene que moverse.
      if (backL.read() > 60) {
        int i = backL.read();
        i--;
        backL.write(i);
      } else if (backL.read() < 60) {
        int i = backL.read();
        i++;
        backL.write(i);
      }

      // Dependiendo de la pos de la trasera derecha es hacia donde tiene que moverse.
      if (backR.read() < 125) {
        int i = backR.read();
        i++;
        backR.write(i);
      } else if (backR.read() > 125) {
        int i = backR.read();
        i--;
        backR.write(i);
      }
      lastMoveTime = millis();
    }
  }

  delay(1000);

  while (frontL.read() != 0 || frontR.read() != 100) {
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

void Piernas::weirdPose() {
  frontL.write(70);   // 70 derecho - 150 atras - 20 adelante
  frontR.write(100);  // 100 derecho - 40 atras - 170 adelante
  delay(600);
  backL.write(150);  // 80 derecho - 160 atras - 35 adelante
  delay(300);
  backR.write(30);  // 100 derecho - 15 atras - 170 adelante
  delay(600);
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
      while (girando > 0 && !*isPetting) {
        backR.write(130);
        frontR.write(130);
        delay(1000);
        backR.write(60);
        frontR.write(60);
        delay(1000);
        girando--;
      }
    } else {
      frontR.write(110);
      backR.write(105);
      delay(1000);
      // Gira hacia la derecha
      while (girando > 0 && !*isPetting) {
        backL.write(65);
        frontL.write(55);
        delay(1000);
        backL.write(110);
        frontL.write(100);
        delay(1000);
        girando--;
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

void Piernas::botMovementAlt(bool (Interaccion::*callback)(), Interaccion* interaccionObj) {
  standUp();
  // El problema es que si las de adelante van muy hacia atras, el gato no puede efecutar bien su movimiento.
  if (1) {
    /*movimiento tipo chimpance */

    // Piernas delanteras hacia delante
    frontR.write(160);
    frontL.write(30);
    delay(600);
    // Piernas delanteras rectas y piernas traseras hacia delante
    frontR.write(100);
    frontL.write(70);
    backR.write(160);
    backL.write(45);
    delay(600);
    // Piernas traseras hacia atras y piernas delanteras hacia delante.
    backR.write(25);
    backL.write(150);
    frontR.write(160);
    frontL.write(30);
    delay(600);
  }
}

//CAMINÓ MEDIO TORCIDO A VELOCIDAD 4/5 DE 150 A 100 Y DE 25 A 70

void Piernas::catMovementWCb(bool (Interaccion::*callback)(), Interaccion* interaccionObj) {
  int posTD = 130;  // Pierna recta
  int ultimoValorPosTD = posTD;

  int posTI = 150;  // Pierna hacia atras
  int ultimoValorPosTI = posTI;

  bool isPetting = (interaccionObj->*callback)();
  // Primera parte del movimiento
  while ((posTD > 45 || posTI > 65) && !isPetting) {  // Mientras la trasera D no esté atras O la trasera I no esté adelante
    if (timeLapse()) {
      if (posTD > 45) {
        backR.write(posTD);
        posTD--;
        int stepsR = map(posTD, 45, 125, 120, 60);
        frontR.write(stepsR);
      }
      if (posTI > 65) {
        backL.write(posTI);
        if (posTD <= 45)
          posTI -= 2;
        else
          posTI--;
        int stepsL = map(posTI, 130, 65, 50, 110);
        frontL.write(stepsL);
      }
      lastMoveTime = millis();
    }
    isPetting = (interaccionObj->*callback)();
  }

  ultimoValorPosTD = posTD;
  ultimoValorPosTI = posTI;

  // Segunda parte del movimiento
  while ((posTD < 125 || posTI < 130) && !isPetting) {  // Mientras la trasera D no esté adelante O la trasera I no esté atras
    if (timeLapse()) {
      if (posTD < 125) {
        backR.write(posTD);
        posTD++;
        int stepsR = map(posTD, 125, 45, 60, 120);
        frontR.write(stepsR);
        if (posTI < 130) {
          backL.write(posTI);
          posTI++;
          int stepsL = map(posTI, 65, 130, 110, 50);
          frontL.write(stepsL);
        }

        lastMoveTime = millis();
      }
    }
    isPetting = (interaccionObj->*callback)();
  }
}

// Notese que las operaciones entre los diferentes lados son opuestas. Las derechas ++ y las izquierdas -- para adelantar por ejemplo.
// PROBAR MOVER IZQUIERDA CON IZQUIERDA Y DERECHA CON DERECHA, DESPÚES DE AHI SE PUEDEN IR MEZCLANDO UN POCO TAL VEZ. NINGUNA DE LAS PIERNAS DELANTERAS DEBE QUEDAR ATRAS,
//  Y EN EL CASO DE QUE UNA PIERNA TRASERA ESTÉ EN EL AIRE, SOLO DEBE MOVERSE PARA ATRAS CUANDO SEA APOYADA
void Piernas::catMovementAlt(bool (Interaccion::*callback)(), Interaccion* interaccionObj) {


  int posFR = 150;  // adelante
  int posFL = 50;   // un poco menos adelante

  int posBL = 40;   // Pierna adelante (con el talon apoyado)
  int posBR = 100;  // Pierna recta

  int lastPosFR = posFR;
  int lastPosFL = posFL;

  bool isPetting = (interaccionObj->*callback)();
  // Cuando la delantera izquierda este adelante

  // Mientras la Delantera Derecha no se encuentre recta y no se lo este acariciando
  while (posFR > 100 && !isPetting) {
    if (timeLapse()) {
      // Ambas piernas van de adelante hacia atras
      posFR--;
      frontR.write(posFR);
      int stepsBL = map(posFR, 150, 100, posBL, 140);  // O 160 para max TI
      backL.write(stepsBL);
      // posTI++;

      // Cuando las 2 anteriores esten llegando al impulso/rectitud, quiero que la trasera derecha (que va a estar en el aire), se mueva hacia adelante, y apenas la delantera izquierda
      if (posFR < 120) {
        int stepsBR = map(posFR, 120, 100, posBR, 150);
        backR.write(stepsBR);
        int stepsFL = map(posFR, 120, 100, posFL, 30);  // Capaz conviene un poco menos adelante la delantera izquierda.
        frontL.write(stepsFL);
      }
      //posTD++;
      //posDI--;
      lastMoveTime = millis();
    }
    // Chequea la caricias devuelta llamando a la funcion pasada por parametro
    isPetting = (interaccionObj->*callback)();
  }

  lastPosFR = posFR;

  // Mientras la Delantera Izquierda no se encuentre recta y no se lo este acariciando
  while (posFL < 70 && !isPetting) {
    if (timeLapse()) {
      // Una vez que este adelantada, la delantera derecha se va a volver a mover apenas para adelante y la delantera izquierda comienza a ir para atras
      posFL++;
      frontL.write(posFL);
      if (posFL < 40) {
        int stepsFR = map(posFL, 20, 40, lastPosFR, 130);
        frontR.write(stepsFR);
      }
      // posDD++;
      // posDI++;

      // Cuando la DI alcanza cierta rotacion, la pierna trasera derecha se apoya sobre el suelo y comienza a ir hacia atras.
      if (posFL >= 50) {
        int stepsBR = map(posFL, lastPosFL, 70, 150, 50);
        backR.write(stepsBR);
        //posTD--;
        //posDI++;
      }
      lastMoveTime = millis();
    }
    // Chequea la caricias devuelta llamando a la funcion pasada por parametro
    isPetting = (interaccionObj->*callback)();
  }

  // Cuando las otras 2 ya estan atras, la TI comienza a ir hacía adelante porque se encuentra en el aire.
  //posTI--;
  // Luego se repite el ciclo desde el principio.
}

bool Piernas::timeLapse() {
  return ((millis() - lastMoveTime) > moveTimer);
}

void Piernas::setMoveTimer(unsigned long value) {
  this->moveTimer = value;
}