#include <Servo.h>

#define EXE_INTERVAL 1000              //350
unsigned long lastExecutedMillis = 0;  // vairable to save the last executed time

const int piernaTrasIzqPin = 12;
const int piernaTrasDerPin = 13;
const int piernaDelIzqPin = 3;
const int piernaDelDerPin = 2;

Servo traseraIzq;
Servo traseraDer;
Servo delanteraIzq;
Servo delanteraDer;

int posTraseraIzquierda = 35;
int posDelanteraIzquierda = 65;
int posTraseraDerecha = 35;
int posDelanteraDerecha = 65;

void setup() {
  Serial.begin(9600);
  traseraIzq.attach(piernaTrasIzqPin);
  traseraDer.attach(piernaTrasDerPin);
  delanteraIzq.attach(piernaDelIzqPin);
  delanteraDer.attach(piernaDelDerPin);

  // traseraIzq.write(20);     // 150 - 0 // sentado = 130 // parado = 50 // adelantado = 20
  // traseraDer.write(125);    // 0 - 140 // sentado = 20 // parado = 90 // adelantado = 125
  // delanteraDer.write(110);  // 0 - 137 // sentado = 20 // parado = 50 // adelantado = 110
  // delanteraIzq.write(20);   // 145 - 0 // sentado = 120 // parado = 85 // adelantado = 20

  //pararse();
}
void loop() {

  moverse();
}


// Movimiento de a 2 piernas (Solo las de atras).
void movimiento_2PiernasTras(bool& accion) {
  if (accion) {
    traseraIzq.write(120);
    delanteraDer.write(20);
    traseraDer.write(30);
    delanteraIzq.write(120);
    accion = false;
  } else {
    traseraIzq.write(10);
    delanteraDer.write(20);  // 110
    traseraDer.write(135);
    delanteraIzq.write(120);  // 20
    accion = true;
  }
}

// Movimiento de a 2 piernas
void movimiento_2Piernas(bool& alternar) {
  if (alternar) {
    if (traseraIzq.read() < 120) {
      //Piernas traseras hacia atras
      traseraIzq.write(120);
      traseraDer.write(30);
    } else {
      //Piernas traseras hacia adelante
      traseraIzq.write(10);
      traseraDer.write(135);
      alternar = false;
    }

    //Piernas delanteras paradas
    delanteraDer.write(50);
    delanteraIzq.write(85);
  } else {
    if (delanteraIzq.read() <= 25) {
      //Piernas delanteras paradas
      delanteraDer.write(55);  //110 - 35
      delanteraIzq.write(80);  //20 - 105
    } else {
      //Piernas delanteras hacia adelante
      delanteraDer.write(110);  //110
      delanteraIzq.write(20);   //20
      alternar = true;
    }

    //Piernas traseras hacia adelante
    traseraIzq.write(10);
    traseraDer.write(135);
  }
}
// Mover trasera hacia adelante -- Mover delantera LENTAMENTE hacia atras
// Hacer impulso con trasera y seguir empujando delantera hacia atras LENTAMENTE
// Cuando la trasera avanza y se esta por reincoporar (apoyar), la delantera impulsa hacia adelante.
// Lo mismo pasan con las piernas del otro lado del cuerpo pero al reves, cuando de un lado van hacia adelante, del otro van hacia atras x ej.

// Movimiento más fluido (puede ser a 1 pierna o a 2 pero 1del y 1tras)
// IMPORTANTE al hacerlo con una sola mitad (pierna del y tras derechas), produce una rotacion para, en este caso, el lado izquierdo.
// ARREGLAR ==> Mover mas para atras las piernas de adelante para su movimiento?
void movimiento_Gato() {

  int iDer = 135;
  int ultimoValorIDer = iDer;
  int jDer = 95;  // Tiene que bajar a 50/55

  int iIzq = 120;
  int ultimoValorIIzq = iIzq;
  int jIzq = 85;  // Tiene que bajar a 20

  int velocidadPierna = 2;  // Mientras más chico el valor, más rapido se mueve la pierna secundaria

  // Pierna trasera derecha va desde adelante hacia atras -- Izquierda desde atras hacia adelante
  while (iDer > 30 || iIzq > 10) {

    if (iDer > 30) {
      traseraDer.write(iDer);
      iDer--;
      // Para incrementar la otra pierna más lenta que la que va a tener el impulso principal, chequeo un viejo valor de I almacenado sumado a un rango x
      if (iDer < ultimoValorIDer - velocidadPierna) {
        // Mueve la pierna delantera derecha hacia atras LENTAMENTE
        delanteraDer.write(jDer);
        jDer--;
        ultimoValorIDer = iDer;
      }
    }

    // Parte izquierda
    if (iIzq > 10) {
      traseraIzq.write(iIzq);
      iIzq--;

      if (iIzq < ultimoValorIIzq - 1) {
        // Mueve la pierna delantera derecha hacia atras LENTAMENTE
        delanteraIzq.write(jIzq);
        jIzq--;
        ultimoValorIIzq = iIzq;
      }
    }

    // Para cambiar el ritmo de actualizacion.
    if (iDer > 70)
      delay(10);
    else
      delay(30);
  }

  //Serial.println(ultimoValorIDer);
  //Serial.println(jDer);
  // i = 30
  // j = ? --> depende del rango. Con rango 2 ==>

  ultimoValorIDer = iDer;
  ultimoValorIIzq = iIzq;

  // Pierna trasera derecha va desde atras hacia adelante
  while (iDer < 135 || iIzq < 105) {
    if (iDer < 135) {
      traseraDer.write(iDer);
      iDer++;

      if (iDer > ultimoValorIDer + velocidadPierna) {
        // Mueve la pierna delantera derecha hacia atras LENTAMENTE
        delanteraDer.write(jDer);

        // Cuando la pierna trasera este a mitad de recorrido, la pierna delantera va a empezar a ir hacia adelante
        if (traseraDer.read() >= 135 / 2) {
          jDer++;
          //velocidadPierna = 2;
        } else
          jDer--;

        ultimoValorIDer = iDer;
      }
    }

    // Parte izquierda
    if (iIzq < 105) {
      traseraIzq.write(iIzq);
      iIzq++;
      if (iIzq > ultimoValorIIzq + 1) {
        delanteraIzq.write(jIzq);
        // Cuando la pierna trasera este a mitad de recorrido, la pierna delantera va a empezar a ir hacia adelante
        if (traseraIzq.read() >= 15) {  // 105 porque el max en este caso el max es 10 y el min 105. 105 - 10 = 95 == rango
          jIzq++;
          //velocidadPierna = 2;
        } else
          jIzq--;
        ultimoValorIIzq = iIzq;
      }
    }

    if (iDer < 70)
      delay(10);
    else
      delay(30);
  }
}

void marchaAtras(){

}

void rotarIzquierda() {
  int i = 135;
  int ultimoValorI = i;
  int j = 110;
  int velocidadPierna = 2;  // Mientras más chico el valor, más rapido se mueve la pierna secundaria

  // Pierna trasera derecha va desde adelante hacia atras
  while (i > 30) {
    traseraDer.write(i);
    i--;
    // Para incrementar la otra pierna más lenta que la que va a tener el impulso principal, chequeo un viejo valor de I almacenado sumado a un rango x
    if (i < ultimoValorI - velocidadPierna) {
      // Mueve la pierna delantera derecha hacia atras LENTAMENTE
      delanteraDer.write(j);
      j--;
      ultimoValorI = i;
    }

    // Para cambiar el ritmo de actualizacion.
    if (i > 70)
      delay(10);
    else
      delay(30);
  }

  // Pierna trasera derecha va desde atras hacia adelante
  while (i < 135) {
    traseraDer.write(i);
    i++;

    if (i > ultimoValorI + velocidadPierna) {
      // Mueve la pierna delantera derecha hacia atras LENTAMENTE
      delanteraDer.write(j);

      // Cuando la pierna trasera este a mitad de recorrido, la pierna delantera va a empezar a ir hacia adelante
      if (traseraDer.read() >= 135 / 2) {
        j++;
        //velocidadPierna = 2;
      } else
        j--;

      ultimoValorI = i;
    }

    if (i < 70)
      delay(10);
    else
      delay(30);
  }
}

void moverse() {
  unsigned long currentMillis = millis();
  bool accion = true;
  //acostarse();
  movimiento_Gato();


  /*while (1) {
    currentMillis = millis();
    if (currentMillis - lastExecutedMillis >= EXE_INTERVAL) {
      movimiento_2Piernas(accion);

      lastExecutedMillis = currentMillis;
    }
  }*/
}

void pararse() {  //acostado
  int izquierda = 50;
  int derecha = 50;
  traseraIzq.write(35);    // invertido 20 max 70 min -- 90?
  delanteraIzq.write(85);  // invertido 5 max 70-80 min

  delanteraDer.write(50);  // incrementa para avanzar -- 55/60 para avanzar Max parase 30 min 5 // apoyar adelante:22
  traseraDer.write(65);    // incrementa para avanzar //Pierna tipo2: Max 90 min 5 -- 90 para mover // estirada caminata:
}

// PARAR EN 2 PATAS
// CON VALORES: TraserIzq = 10 | traserDer = 135 | delanteraIzq = 20 | delanteraDer = 105 ==> lo puedo llegar a parar en 2 piernas (o primero adelantando mucho las piernas de adelante, luego adelantando las de atras y luego poniendolas un poco más atras.)

// Acostado raro --> tipo robot?
void acostarse() {
  traseraIzq.write(120);
  traseraDer.write(10);

  delanteraIzq.write(140);    // --- 20 --- 140
  delanteraDer.write(0);  // --- 105 --- 0
}

void agazaparse() {
  traseraIzq.write(0);
  traseraDer.write(145);

  delanteraIzq.write(0);
  delanteraDer.write(140);
}

void comprobarExtremidades() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastExecutedMillis >= EXE_INTERVAL) {
    lastExecutedMillis = currentMillis;  // save the last executed time
    if (traseraIzq.read() == 90) {
      traseraIzq.write(0);
      delanteraDer.write(90);
      traseraDer.write(0);
      delanteraIzq.write(90);
    } else {
      traseraIzq.write(90);
      delanteraDer.write(0);
      traseraDer.write(90);
      delanteraIzq.write(0);
    }
  }
}
