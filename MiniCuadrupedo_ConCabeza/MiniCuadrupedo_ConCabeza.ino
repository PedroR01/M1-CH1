// Instalar librerias: Capacitive Sensor (tacto humano) - Servo (servomotores) - New Ping (ultrasonico)
#define LIMITE_SUP 110
#define LIMITE_INF 70
#define DISTANCIA_OBSTACULO 30
#define ANCHO_CAMINO 25             // Un poco más del ancho del robot para que avance por lugares por los que pueda pasar
#define TIEMPO_MAX_MONITOREO 13000  // 13 seg
#define TIEMPO_MOVIMIENTO 850
#define AFECTO_MINIMO 1  // Valor minimo de afecto para que actue más como un gato (alcanzado por mimos())

// Tiempos aprox en los que tarda en rotar dependiendo de los grados a los que deba hacerlo
#define TMAX_ROTACION 4
#define TMIN_ROTACION 2

#include <Servo.h>
#include <NewPing.h>
#include <CapacitiveSensor.h>

// Filtro o rango a partir del que se desea efectuar algun evento
int filtro = 300;

/* --- CABEZA --- */
const int cuelloPin = A1;
const int triggerPin = 8;
const int echoPin = A0;

const int limiteSup = 100;
const int limiteInf = 80;
const int valorInicialCuello = 90;

unsigned long ultimoTiempoEjecucion = 0;

// pin 6 envia energia - pin 5 capta cambios de energia
CapacitiveSensor capSensor = CapacitiveSensor(6, 5);

NewPing sonar(triggerPin, echoPin, 100);
Servo cuello;

/* --- PIERNAS --- */

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

// Si esta como global ¿hace falta que lo pase como parametro de los metodos?
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
  mimos(modoGato);
  observar(modoGato);
  moverse(modoGato);
  if (modoGato >= 0.3)
    modoGato -= 0.3;
  Serial.print("Valor final: ");
  Serial.println(modoGato);
}

/* !! ---- MOVIMIENTOS ---- !! */
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
void movimiento_2Piernas(bool& alternar, float& modoGato) {
  // HOTFIX
  if (capSensor.capacitiveSensor(30) > filtro) {
    mimos(modoGato);
  }
  if (sonar.ping_cm() <= DISTANCIA_OBSTACULO) {
    observar(modoGato);
  }
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
    //HOTFIX
    if (capSensor.capacitiveSensor(30) > filtro) {
      mimos(modoGato);
    }
    if (sonar.ping_cm() <= DISTANCIA_OBSTACULO) {
      observar(modoGato);
    }
  }
}
// Mover trasera hacia adelante -- Mover delantera LENTAMENTE hacia atras
// Hacer impulso con trasera y seguir empujando delantera hacia atras LENTAMENTE
// Cuando la trasera avanza y se esta por reincoporar (apoyar), la delantera impulsa hacia adelante.
// Lo mismo pasan con las piernas del otro lado del cuerpo pero al reves, cuando de un lado van hacia adelante, del otro van hacia atras x ej.

// Movimiento más fluido (puede ser a 1 pierna o a 2 pero 1del y 1tras)
// IMPORTANTE al hacerlo con una sola mitad (pierna del y tras derechas), produce una rotacion para, en este caso, el lado izquierdo.
// ARREGLAR ==> Mover mas para atras las piernas de adelante para su movimiento?
// HOTFIX: resto de aca el modoGato para decrementar su valor más rápido, ya que sino tarda mucho en decrementar debido a su largo/complejo movimiento
void movimiento_Gato(float& modoGato) {

  int iDer = 135;
  int ultimoValorIDer = iDer;
  int jDer = 95;  // Tiene que bajar a 50/55

  int iIzq = 120;
  int ultimoValorIIzq = iIzq;
  int jIzq = 85;  // Tiene que bajar a 20

  int velocidadPierna = 2;  // Mientras más chico el valor, más rapido se mueve la pierna secundaria
  if (modoGato >= 1)
    modoGato -= 0.3;
  // HOTFIX
  if (capSensor.capacitiveSensor(30) > filtro) {
    mimos(modoGato);
  }
  if (sonar.ping_cm() <= DISTANCIA_OBSTACULO) {
    observar(modoGato);
  }
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

  if (modoGato >= 1)
    modoGato -= 0.3;

  // HOTFIX
  if (capSensor.capacitiveSensor(30) > filtro) {
    mimos(modoGato);
  }
  if (sonar.ping_cm() <= DISTANCIA_OBSTACULO) {
    observar(modoGato);
  }
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


void rotarIzquierda(int orientacionCabeza, float& modoGato) {
  // Mientras que no detecte ningun obstaculo va a seguir girando
  // RESOLVER: ¿ A que velocidad rota para un costado ? (Testear con el robot)
  int tiempoRotacion = map(orientacionCabeza, 90, 180, TMIN_ROTACION, TMAX_ROTACION);  // Lo multiplico por 60 porque se ejecuta 60 veces por segundo?

  while (tiempoRotacion > 0) {
    // HOTFIX
    if (modoGato >= 1)
      modoGato -= 0.2;
    Serial.println(tiempoRotacion);
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
    tiempoRotacion--;
  }
  cuello.write(90);  // Volver a centrar la orientacion de la cabeza
}

void rotarDerecha(int orientacionCabeza) {
  Serial.println("Rotacion a la derecha");
}

// ARREGLAR => No esta el parametro por referencia, por eso no lo modifica desde movimiento_Gato();
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

  delanteraIzq.write(140);  // --- 20 --- 140
  delanteraDer.write(0);    // --- 105 --- 0
}

void agazaparse() {
  traseraIzq.write(0);
  traseraDer.write(145);

  delanteraIzq.write(0);
  delanteraDer.write(140);
}

void comprobarExtremidades() {
  unsigned long tiempoActual = millis();

  if (tiempoActual - ultimoTiempoEjecucion >= TIEMPO_MOVIMIENTO) {
    ultimoTiempoEjecucion = tiempoActual;  // save the last executed time
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

void tateQuieto(int distancia) {
  while (distancia >= sonar.ping_cm())
    Serial.println("Se queda quieto");
}

/* !! ---- ESCANEO ENTORNO ---- !! */
void observar(float& modoGato) {
  //Serial.print("modoGato: ");
  //Serial.print(modoGato);
  // Cada una determinada cantidad de tiempo mover la cabeza en el rango del ancho del robot para saber si hay algun obstaculo que pueda interferir a lo ancho del robot
  unsigned long tiempoActual = millis();
  // RESOLVER: Hacer que el tiempo random minimo varie en 500 ms con respecto al ultimo tiempo.
  int temporizadorMonitoreo = random(8000, TIEMPO_MAX_MONITOREO);
  Serial.print("temporizador: ");
  Serial.print(tiempoActual - ultimoTiempoEjecucion);
  cuello.write(90);  // HOTFIX para algunas veces que se queda mirando a un costado.
  if ((tiempoActual - ultimoTiempoEjecucion) > temporizadorMonitoreo) {
    analizarEntorno(modoGato);
    tiempoActual = millis();
    ultimoTiempoEjecucion = tiempoActual;
  }

  const int temporizadorPersona = 1500;
  // La cabeza permanece quieta hasta que se cruza con un objeto a una determinada distancia
  int distancia = sonar.ping_cm();
  bool obstaculo = (distancia <= DISTANCIA_OBSTACULO) && (distancia != 0);
  if (obstaculo) {
    Serial.print("Distancia: ");
    Serial.println(distancia);

    tiempoActual = millis();
    int temporizador = tiempoActual;
    // 2. Comprobar si esta recibiendo mimos.
    while ((tiempoActual - temporizador) < temporizadorPersona) {
      if (capSensor.capacitiveSensor(30) > filtro) {
        mimos(modoGato);
      }
      tiempoActual = millis();
    }
    // TESTEO PARA DIFERENCIAR ENTRE UN OBSTACULO Y UNA PERSONA.
    int posicionAnterior = sonar.ping_cm();
    delay(temporizadorPersona);
    int posicionActual = sonar.ping_cm();
    // (Solo si esta en modo gato ¿o robot?) 3. Si detecto algo y luego desaparecio, puede mirar a los costados y hacer la comprobación (con el temporizador de movimiento) para ver si la persona detectada se movio hacia otro lado.
    if (modoGato > AFECTO_MINIMO) {
      posicionAnterior = sonar.ping_cm();
      delay(temporizadorPersona);
      posicionActual = sonar.ping_cm();
      if (((posicionAnterior != posicionActual) && posicionActual > DISTANCIA_OBSTACULO)) {
        Serial.println("Buscando persona");
        analizarEntorno(modoGato);
      }
    }
    // 1. Si lo detectado se sigue acercando (en base a un temporizador) aunque el gato este quieto, eso quiere decir que es un humano (sin tener en cuenta el caso en el que sea algun objeto no estatico).
    else {
      if (abs(posicionAnterior - posicionActual) >= 2) {
        //tateQuieto(posicionActual);
      }
    }
  }
}

// Controla las caricias de la persona hacia el gato y su reacción
void mimos(float& modoGato) {
  // Guarda el valor captado por el sensor
  long valorTacto = capSensor.capacitiveSensor(30);

  // AGREGAR! En los for al ir llegando al techo del valor del indice se puede ir incrementando el delay para producir un "suavizado" o en su defecto ir incrementando mas lento el indice.
  // Cuando se acaricie al gato, la cabeza se mueve muy poco a los costados
  while (valorTacto > filtro) {
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
    valorTacto = capSensor.capacitiveSensor(30);
    if (valorTacto > filtro) {
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
    delay(random(40, 100));

    // Revisa si se sigue acariciando o no para no quedar en un while infinito.
    valorTacto = capSensor.capacitiveSensor(30);
    // Vuelve la cabeza a su posicion inicial de forma suave.
    if (valorTacto < filtro) {
      if (i < valorInicialCuello) {
        for (i; i != valorInicialCuello; i++) {
          cuello.write(i);
          delay(random(35, 75));
        }
      } else if (i > valorInicialCuello) {
        for (i; i != valorInicialCuello; i--) {
          cuello.write(i);
          delay(random(35, 75));
        }
      }
    }
  }
  Serial.print("modoGato: ");
  Serial.print(modoGato);
}

void mirarIzquierda(bool& caminoLibre) {
  bool puedeCaminar = false;
  int rangoCamino = ANCHO_CAMINO;

  while (!caminoLibre && cuello.read() < 180) {  // el limite seria 180 - el valor del ancho del robot (rango en el que no deben haber obstaculos para que pase el robot)
    cuello.write(cuello.read() + 1);
    if (sonar.ping_cm() > DISTANCIA_OBSTACULO) {
      puedeCaminar = true;
      rangoCamino--;
      if (rangoCamino == 0) {                              // Si por un rango en el que pueda pasar el robot no se encuentran obstaculos, está habilitado para direccionarse hacia allí y avanzar.
        cuello.write(cuello.read() - (ANCHO_CAMINO / 2));  // Apunta la cabeza hacia el centro del camino disponible.
        caminoLibre = true;
        // Girar todo su cuerpo hasta que queden en orientación derecha al igual que la cabeza
        rotarIzquierda(cuello.read(), modoGato);
      }
    } else {
      puedeCaminar = false;
      rangoCamino = ANCHO_CAMINO;
    }
    delay(20);
  }
}

void mirarDerecha(bool& caminoLibre) {
  Serial.println("Mirando a la derecha");
  bool puedeCaminar = false;
  int rangoCamino = ANCHO_CAMINO;

  while (!caminoLibre && cuello.read() > 0) {  // el limite seria 0 + el valor del ancho del robot (rango en el que no deben haber obstaculos para que pase el robot)
    cuello.write(cuello.read() - 1);
    if (sonar.ping_cm() > DISTANCIA_OBSTACULO) {
      puedeCaminar = true;
      rangoCamino--;
      if (rangoCamino == 0) {                              // Si por un rango en el que pueda pasar el robot no se encuentran obstaculos, está habilitado para direccionarse hacia allí y avanzar.
        cuello.write(cuello.read() + (ANCHO_CAMINO / 2));  // Apunta la cabeza hacia el centro del camino disponible.
        caminoLibre = true;
        // Girar todo su cuerpo hasta que queden en orientación derecha al igual que la cabeza
        rotarDerecha(cuello.read());
      }
    } else {
      puedeCaminar = false;
      rangoCamino = ANCHO_CAMINO;
    }
    delay(20);
  }
}

void marchaAtras() {
  Serial.println("Dando marcha atras");
  cuello.write(valorInicialCuello);
}

void girarCabeza(int cabeza) {
  for (int i = cabeza; i < cabeza + 20; i++) {
    cuello.write(i);
    delay(30);
  }
  for (int i = cabeza; i < cabeza - 20; i--) {
    cuello.write(i);
    delay(30);
  }
}

void analizarEntorno(float& modoGato) {
  bool hayCamino = false;
  /* 
  Gira para un lado la cabeza (hasta un limite de rotación o hasta que no detecta nada enfrente).
  Si esta todo bloqueado gira la cabeza hacia el otro lado de donde empezo a girarla.
  */
  mirarIzquierda(hayCamino);
  if (!hayCamino) {
    mirarDerecha(hayCamino);

    // Si no hay ningun camino posible vuelve hacia atras (girandose o volviendose para atras una determinada cantidad de pasos y analizando devuelta el entorno)
    if (!hayCamino)
      if (modoGato >= AFECTO_MINIMO) {
        acostarse();
        while (modoGato >= AFECTO_MINIMO) {
          girarCabeza(cuello.read());
          modoGato -= 0.1;
          if (capSensor.capacitiveSensor(30) > filtro)
            modoGato += 0.1;
        }
      } else {
        marchaAtras();
      }
  }
}