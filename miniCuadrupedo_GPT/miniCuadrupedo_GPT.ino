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
  unsigned long tiempoActual = millis();

  if (tiempoActual - ultimoTiempoMimos >= 100) {
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
  Serial.println(modoGato);
}

void movimiento_2PiernasTras(bool& accion) {
  if (accion) {
    traseraIzq.write(120);
    delanteraDer.write(20);
    traseraDer.write(30);
    delanteraIzq.write(120);
    accion = false;
  } else {
    traseraIzq.write(10);
    delanteraDer.write(20);
    traseraDer.write(135);
    delanteraIzq.write(120);
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
    if (traseraIzq.read() < 120) {
      traseraIzq.write(120);
      traseraDer.write(30);
    } else {
      traseraIzq.write(10);
      traseraDer.write(135);
      alternar = false;
    }
    delanteraDer.write(50);
    delanteraIzq.write(85);
  } else {
    if (delanteraIzq.read() <= 25) {
      delanteraDer.write(55);
      delanteraIzq.write(80);
    } else {
      delanteraDer.write(110);
      delanteraIzq.write(20);
      alternar = true;
    }
    traseraIzq.write(10);
    traseraDer.write(135);
    if (capSensor.capacitiveSensor(30) > filtro) {
      mimos(modoGato);
    }
    if (sonar.ping_cm() <= DISTANCIA_OBSTACULO && sonar.ping_cm() != 0) {
      observar(modoGato);
    }
  }
}

void movimiento_Gato(float& modoGato) {
  int iDer = 135;
  int ultimoValorIDer = iDer;
  int jDer = 95;

  int iIzq = 120;
  int ultimoValorIIzq = iIzq;
  int jIzq = 85;

  int velocidadPierna = 2;

  if (capSensor.capacitiveSensor(30) > filtro) {
    mimos(modoGato);
  }
  if (sonar.ping_cm() <= DISTANCIA_OBSTACULO && sonar.ping_cm() != 0) {
    observar(modoGato);
  }

  while (iDer > 30 || iIzq > 10) {
    if (iDer > 30) {
      traseraDer.write(iDer);
      iDer--;
      if (iDer < ultimoValorIDer - velocidadPierna) {
        delanteraDer.write(jDer);
        jDer--;
        ultimoValorIDer = iDer;
      }
    }
    if (iIzq > 10) {
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

  while (iDer < 135 || iIzq < 105) {
    if (iDer < 135) {
      traseraDer.write(iDer);
      iDer++;
      if (iDer > ultimoValorIDer + velocidadPierna) {
        delanteraDer.write(jDer);
        if (traseraDer.read() >= 135 / 2) {
          jDer++;
        } else
          jDer--;
        ultimoValorIDer = iDer;
      }
    }
    if (iIzq < 105) {
      traseraIzq.write(iIzq);
      iIzq++;
      if (iIzq > ultimoValorIIzq + 1) {
        delanteraIzq.write(jIzq);
        if (traseraIzq.read() >= 15) {
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
