#define LIMITE_SUP 110
#define LIMITE_INF 70
#define DISTANCIA_OBSTACULO 20
#define ANCHO_CAMINO 25
#define TIEMPO_MAX_MONITOREO 13000
#define TIEMPO_MOVIMIENTO 850
#define AFECTO_MINIMO 1

#include <Servo.h>
#include <NewPing.h>
#include <CapacitiveSensor.h>

int filtro = 300;

const int cuelloPin = A1;
const int triggerPin = 8;
const int echoPin = A0;

const int limiteSup = 100;
const int limiteInf = 80;
const int valorInicialCuello = 90;

unsigned long ultimoTiempoEjecucion = 0;
unsigned long ultimoTiempoMimos = 0;
unsigned long ultimoTiempoObservacion = 0;

CapacitiveSensor capSensor = CapacitiveSensor(6, 5);
NewPing sonar(triggerPin, echoPin, 100);
Servo cuello;

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

float modoGato = 0;

void setup() {
  Serial.begin(9600);

  cuello.attach(cuelloPin);
  cuello.write(90);

  traseraIzq.attach(piernaTrasIzqPin);
  traseraDer.attach(piernaTrasDerPin);
  delanteraIzq.attach(piernaDelIzqPin);
  delanteraDer.attach(piernaDelDerPin);
}

void loop() {
  inicializar();
  //observar(modoGato);
  unsigned long tiempoActual = millis();
  bool alternar = true;
  if (modoGato >= AFECTO_MINIMO) {
    movimiento_Gato(modoGato);
  } else {
    movimiento_Gato(modoGato);
    
    if (tiempoActual - ultimoTiempoEjecucion >= TIEMPO_MOVIMIENTO) {
      movimiento_2Piernas(alternar, modoGato);
      ultimoTiempoEjecucion = tiempoActual;
    }
    
  }
  /*if (tiempoActual - ultimoTiempoMimos >= 100) {
    mimos(modoGato);
    ultimoTiempoMimos = tiempoActual;
  }

  if (tiempoActual - ultimoTiempoObservacion >= 6000) {
    observar(modoGato);
    ultimoTiempoObservacion = tiempoActual;
  }

  moverse(modoGato);

  // Decrementar modoGato si es mayor o igual a 0.3
  if (modoGato >= 0.3) {
    modoGato -= 0.3;
  } else if (modoGato < 0.3 && modoGato > 0) {
    modoGato = 0;  // Para evitar valores negativos
  }

  Serial.print("Valor final: ");
  Serial.println(modoGato);*/
}

void inicializar() {
  traseraIzq.write(80);     // 80 derecho - 160 atras - 35 adelante
  traseraDer.write(130);    // 130 derecho - 45 atras - 170 adelante
  delanteraIzq.write(80);   // 80 derecho - 150 atras - 20 adelante
  delanteraDer.write(100);  // 100 derecho - 40 atras - 170 adelante
}

void movimiento_2PiernasTras(bool& accion) {
  if (accion) {
    traseraIzq.write(80);
    delanteraDer.write(130);
    traseraDer.write(130);
    delanteraIzq.write(150);
    accion = false;
  } else {
    traseraIzq.write(160);
    delanteraDer.write(130);
    traseraDer.write(45);
    delanteraIzq.write(150);
    accion = true;
  }
}

void movimiento_2Piernas(bool& alternar, float& modoGato) {
  if (capSensor.capacitiveSensor(30) > filtro) {
    mimos(modoGato);
  }
  if (sonar.ping_cm() <= DISTANCIA_OBSTACULO && sonar.ping_cm() != 0) {
    observar(modoGato);
  }
  if (alternar) {
    if (traseraIzq.read() < 80) {
      traseraIzq.write(80);
      traseraDer.write(130);
    } else {
      traseraIzq.write(160);
      traseraDer.write(45);
      alternar = false;
    }
    delanteraDer.write(50);
    delanteraIzq.write(85);
  } else {
    if (delanteraIzq.read() <= 25) {
      delanteraDer.write(100);
      delanteraIzq.write(80);
    } else {
      delanteraDer.write(135);
      delanteraIzq.write(45);
      alternar = true;
    }
    traseraIzq.write(160);
    traseraDer.write(45);
    if (capSensor.capacitiveSensor(30) > filtro) {
      mimos(modoGato);
    }
    if (sonar.ping_cm() <= DISTANCIA_OBSTACULO && sonar.ping_cm() != 0) {
      observar(modoGato);
    }
  }
}

void movimiento_Gato(float& modoGato) {
  int iDer = 130;
  int ultimoValorIDer = iDer;
  int jDer = 100;

  int iIzq = 150;
  int ultimoValorIIzq = iIzq;
  int jIzq = 100;

  int velocidadPierna = 2;

  if (capSensor.capacitiveSensor(30) > filtro) {
    mimos(modoGato);
  }
  if (sonar.ping_cm() <= DISTANCIA_OBSTACULO && sonar.ping_cm() != 0) {
    observar(modoGato);
  }

  while (iDer > 45 || iIzq > 35) {
    if (iDer > 45) {
      traseraDer.write(iDer);
      iDer--;
      if (iDer < ultimoValorIDer - velocidadPierna) {
        delanteraDer.write(jDer);
        jDer--;
        ultimoValorIDer = iDer;
      }
    }
    if (iIzq > 35) {
      traseraIzq.write(iIzq);
      iIzq--;
      if (iIzq < ultimoValorIIzq - 1) {
        delanteraIzq.write(jIzq);
        jIzq--;
        ultimoValorIIzq = iIzq;
      }
    }
    if (iDer > 70)
      delay(10);
    else
      delay(30);
  }

  ultimoValorIDer = iDer;
  ultimoValorIIzq = iIzq;

  if (capSensor.capacitiveSensor(30) > filtro) {
    mimos(modoGato);
  }
  if (sonar.ping_cm() <= DISTANCIA_OBSTACULO && sonar.ping_cm() != 0) {
    observar(modoGato);
  }

  while (iDer < 170 || iIzq < 160) {
    if (iDer < 170) {
      traseraDer.write(iDer);
      iDer++;
      if (iDer > ultimoValorIDer + velocidadPierna) {
        delanteraDer.write(jDer);
        if (traseraDer.read() >= 130) {
          jDer++;
        } else
          jDer--;
        ultimoValorIDer = iDer;
      }
    }
    if (iIzq < 160) {
      traseraIzq.write(iIzq);
      iIzq++;
      if (iIzq > ultimoValorIIzq + 1) {
        delanteraIzq.write(jIzq);
        if (traseraIzq.read() >= 35) {
          jIzq++;
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

void moverse(float modoGato) {
  unsigned long tiempoActual = millis();
  bool alternar = true;
  if (modoGato >= AFECTO_MINIMO) {
    movimiento_Gato(modoGato);
  } else {
    //movimiento_Gato(modoGato);
    
    if (tiempoActual - ultimoTiempoEjecucion >= TIEMPO_MOVIMIENTO) {
      movimiento_2Piernas(alternar, modoGato);
      ultimoTiempoEjecucion = tiempoActual;
    }
    
  }
}

void observar(float& modoGato) {
  analizarEntorno();
}

void analizarEntorno() {
  int anguloInicial = 45;
  int anguloFinal = 135;
  int paso = 5;
  int mejorAngulo = 90;
  int mejorDistancia = 0;

  for (int angulo = anguloInicial; angulo <= anguloFinal; angulo += paso) {
    cuello.write(angulo);
    delay(200);  // Espera para que el servo termine de moverse
    int distancia = sonar.ping_cm();
    if (distancia > mejorDistancia) {
      mejorDistancia = distancia;
      mejorAngulo = angulo;
    }
  }

  cuello.write(mejorAngulo);
  delay(500);  // Espera para que el servo termine de moverse

  if (mejorDistancia <= DISTANCIA_OBSTACULO) {
    retroceder();
    delay(1000);  // Espera para retroceder
  }

  cuello.write(valorInicialCuello);  // Vuelve a la posición inicial
}

void retroceder() {
  int retrocederDistancia = 20;
  traseraIzq.write(traseraIzq.read() - retrocederDistancia);
  traseraDer.write(traseraDer.read() - retrocederDistancia);
  delanteraIzq.write(delanteraIzq.read() - retrocederDistancia);
  delanteraDer.write(delanteraDer.read() - retrocederDistancia);
  delay(1000);  // Espera para completar la maniobra de retroceso
}

void mimos(float& modoGato) {
  int tiempoActual = millis();
  while (capSensor.capacitiveSensor(30) > filtro) {
    modoGato++;
    int i;
    int suavizado = random(6, 13);
    for (i = cuello.read(); i < LIMITE_SUP; i++) {
      cuello.write(i);
      if (i >= LIMITE_SUP - suavizado)
        delay(random(60, 100));
      else
        delay(random(35, 75));
    }
    if (modoGato < 2.5)
      modoGato += 0.5;

    // Si se sigue acariciando, gira la cabeza hacia el otro lado
    if (capSensor.capacitiveSensor(30) > filtro) {
      for (i = cuello.read(); i > LIMITE_INF; i--) {
        cuello.write(i);
        if (i <= LIMITE_INF + suavizado)
          delay(random(60, 100));
        else
          delay(random(35, 75));
      }
      if (modoGato < 2.5)
        modoGato += 0.5;
    }
  }
  Serial.print("Valor modoGato en mimos: ");
  Serial.println(modoGato);
}
