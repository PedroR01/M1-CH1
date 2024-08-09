#include <Servo.h>

#define EXE_INTERVAL 350
unsigned long lastExecutedMillis = 0;  // vairable to save the last executed time

const int piernaTrasIzqPin = 13;
const int piernaTrasDerPin = 12;
const int piernaDelIzqPin = 2;
const int piernaDelDerPin = 3;

Servo traseraIzq;
Servo traseraDer;
Servo delanteraIzq;
Servo delanteraDer;

bool estado = true;

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

  traseraIzq.write(135); // 150 - 0 // sentado = 135 // parado = 
  traseraDer.write(20); // 0 - 140 // sentado = 20 // parado = 
  delanteraDer.write(20); // 0 - 137 // sentado = 17 // parado = 
  delanteraIzq.write(110); // 145 - 0 // sentado = 130 // parado = 

  //pararse();
}
void loop() {

  //moverse();
  //caminar();
  //delay(5000);
  /*
  if (estado) {
    demostracion();
    estado = false;
  }*/
}

void movimiento_a(bool& accion) {
  if (accion) {
    traseraIzq.write(10);
    delanteraDer.write(30);
    traseraDer.write(78);
    delanteraIzq.write(70);
    accion = false;
  } else {
    traseraIzq.write(60);
    delanteraDer.write(15);
    traseraDer.write(15);
    delanteraIzq.write(80);
    accion = true;
  }
}

void movimiento_b(bool& accion) {
  if (accion) {
    traseraIzq.write(10);
    delanteraDer.write(60);
    traseraDer.write(78);
    delanteraIzq.write(25);
    accion = false;
  } else {
    traseraIzq.write(60);
    delanteraDer.write(15);
    traseraDer.write(15);
    delanteraIzq.write(80);
    accion = true;
  }
}

void movimiento_c(bool& accion) {
  if (accion) {
    traseraIzq.write(10);
    delanteraDer.write(60);
    traseraDer.write(25);
    delanteraIzq.write(65);
    accion = false;
  } else {
    traseraIzq.write(55);
    delanteraDer.write(20);
    traseraDer.write(70);
    delanteraIzq.write(25);
    accion = true;
  }
}

void movimiento_d(bool& pierna1, bool& pierna2, bool& pierna3, bool& pierna4) {

  if (pierna4) {
    for (int i = 25; i < 80; i++) {
      traseraDer.write(i);
      if (i < 50)
        delay(40);
      else
        delay(10);
    }
    /*for(float i = 25; i < 70; i+=0.2){
      int j = (int)(i);
      traseraDer.write(j);
    }*/
    //pierna4 = false;
    //pierna3 = true;
  } else if (pierna3) {
    traseraIzq.write(60);
    delanteraDer.write(15);
    traseraDer.write(15);
    delanteraIzq.write(80);
    pierna3 = false;
    pierna2 = true;
  } else if (pierna2) {
    traseraIzq.write(60);
    delanteraDer.write(15);
    traseraDer.write(15);
    delanteraIzq.write(80);
    pierna2 = false;
    pierna1 = true;
  } else {
    traseraIzq.write(60);
    delanteraDer.write(15);
    traseraDer.write(15);
    delanteraIzq.write(80);
    pierna1 = false;
    pierna4 = true;
  }
}

void moverse() {
  unsigned long currentMillis = millis();
  bool accion = true;
  bool pierna1, pierna2, pierna3 = false;
  bool pierna4 = true;
  while (1) {
    currentMillis = millis();
    if (currentMillis - lastExecutedMillis >= EXE_INTERVAL) {
      //movimiento_a(accion);
      //movimiento_d(pierna1, pierna2, pierna3, pierna4);
      //13
      traseraIzq.write(100); // 20 min 130 max?
      //12
      
      traseraDer.write(30); // 20max 130 min
      lastExecutedMillis = currentMillis;
    }
  }
}

void demostracion() {
  pararse();
  unsigned long currentMillis = millis();
  int ciclo = 0;
  bool accion = true;

  while (ciclo < 4) {
    currentMillis = millis();

    if (currentMillis - lastExecutedMillis >= EXE_INTERVAL) {
      if (accion) {
        acostarse();
        accion = false;
      } else {
        pararse();
        accion = true;
      }
      ciclo++;
      lastExecutedMillis = currentMillis;
    }
  }
}

void caminar() {
  Serial.println("caminando");

  // Arreglo de los servos y sus posiciones
  Servo legs[4] = { traseraIzq, delanteraIzq, traseraDer, delanteraDer };
  int positions[4] = { posTraseraIzquierda, posDelanteraIzquierda, posTraseraDerecha, posDelanteraDerecha };

  // Secuencia de movimiento: orden de las piernas
  int sequence[4] = { 0, 1, 2, 3 };  // Índices correspondientes a las piernas en 'legs'

  // Ciclo de movimiento
  for (int cycle = 0; cycle < 10; cycle++) {  // X cantidad de veces; ajusta el valor de X según sea necesario
    for (int i = 0; i < 4; i++) {
      int currentLeg = sequence[i];
      int nextLeg = sequence[(i + 1) % 4];

      // Mover la pierna actual
      /*Para aquellas piernas que tienen los valores inversos*/
      if (currentLeg == 0 || currentLeg == 1) {
        for (int pos = positions[currentLeg]; pos > 5; pos -= 1) {
          legs[currentLeg].write(pos);
          delay(15);
        }
      } else {
        for (int pos = positions[currentLeg]; pos <= 70; pos += 1) {
          legs[currentLeg].write(pos);
          delay(15);
        }
      }

      // Mover la pierna siguiente mientras la pierna actual vuelve a su posición inicial
      if (currentLeg == 0 || currentLeg == 1) {
        for (int pos = 5; pos >= positions[currentLeg]; pos += 1) {
          legs[currentLeg].write(pos);
          legs[nextLeg].write(positions[nextLeg] - (70 - positions[nextLeg]) / 2);  // Movimiento intermedio de la pierna siguiente
          delay(15);
        }
      } else {
        for (int pos = 70; pos >= positions[currentLeg]; pos -= 1) {
          legs[currentLeg].write(pos);
          // Esto nose si esta bien. REVISAR
          if (currentLeg == 3)
            legs[nextLeg].write(positions[nextLeg] - (70 - positions[nextLeg]) / 2);
          else
            legs[nextLeg].write(positions[nextLeg] + (70 - positions[nextLeg]) / 2);  // Movimiento intermedio de la pierna siguiente

          delay(15);
        }
      }

      // Devolver la pierna siguiente a su posición estable
      legs[nextLeg].write(positions[nextLeg]);
    }
  }
}

void pararse() {  //acostado
  int izquierda = 50;
  int derecha = 50;
  traseraIzq.write(35);    // invertido 20 max 70 min -- 90?
  delanteraIzq.write(85);  // invertido 5 max 70-80 min

  delanteraDer.write(50);  // incrementa para avanzar -- 55/60 para avanzar Max parase 30 min 5 // apoyar adelante:22
  traseraDer.write(65);    // incrementa para avanzar //Pierna tipo2: Max 90 min 5 -- 90 para mover // estirada caminata:
}

void acostarse() {
  traseraIzq.write(80);
  delanteraDer.write(5);
  traseraDer.write(5);
  delanteraIzq.write(80);
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
