/*
  Para hacerlo parecer más orgánico hay que pensar los cambios de valores con algo más aleatorio, no tan estricto
  , y además incluir algo para "suavizar" los movimientos o rotaciones de los motores.
*/

#define LIMITE_SUP 110
#define LIMITE_INF 70
#define DISTANCIA_OBSTACULO 35
#define ANCHO_CAMINO 25  // Un poco más del ancho del robot para que avance por lugares por los que pueda pasar
#define TIEMPO_MAX 5000

#include <Servo.h>
#include <NewPing.h>
#include <CapacitiveSensor.h>

// Filtro o rango a partir del que se desea efectuar algun evento
int filtro = 500;

// TENER EN CUENTA CAMBIOS DE PINES PWM A ANALOGICOS.
const int ServoPin = A1;
const int TriggerPin = 8;
const int EchoPin = A0;

const int limiteSup = 100;
const int limiteInf = 80;
const int valorInicialCuello = 90;

unsigned long ultimoTiempoEjecucion = 0;

// pin 6 envia energia - pin 5 capta cambios de energia
CapacitiveSensor capSensor = CapacitiveSensor(6, 5);

NewPing sonar(TriggerPin, EchoPin, 100);
Servo servo;

void setup() {
  Serial.begin(9600);

  servo.attach(ServoPin);
  servo.write(90);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  mimos();
  observar();
}

void observar() {

  // Cada una determinada cantidad de tiempo mover la cabeza en el rango del ancho del robot para saber si hay algun obstaculo que pueda interferir a lo ancho del robot
  unsigned long tiempoActual = millis();
  const int temporizadorCheckeo = random(3000, TIEMPO_MAX);
  if ((tiempoActual - ultimoTiempoEjecucion) > temporizadorCheckeo) {
    analizarEntorno();
    tiempoActual = millis();
    ultimoTiempoEjecucion = tiempoActual;
  }

  // La cabeza permanece quieta hasta que se cruza con un objeto a una determinada distancia
  int distancia = sonar.ping_cm();
  /* // SOLUCIONAR: Algo para filtrar señales inesperadas y espontaneas?
  int temporizadorFiltrado = 90;
  while(temporizadorFiltrado > 0)
    temporizadorFiltrado--;
    */
  bool obstaculo = (distancia <= DISTANCIA_OBSTACULO) && (distancia != 0);
  if (obstaculo) {
    Serial.println(distancia);
    //analizarEntorno();
  }
}

// Controla las caricias de la persona hacia el gato y su reacción
void mimos() {
  // Guarda el valor captado por el sensor
  long valorTacto = capSensor.capacitiveSensor(30);

  // AGREGAR! En los for al ir llegando al techo del valor del indice se puede ir incrementando el delay para producir un "suavizado" o en su defecto ir incrementando mas lento el indice.
  // Cuando se acaricie al gato, la cabeza se mueve muy poco a los costados
  while (valorTacto > filtro) {
    int i;
    int suavizado = random(6, 13);
    for (i = servo.read(); i < LIMITE_SUP; i++) {
      servo.write(i);
      if (i >= LIMITE_SUP - suavizado)
        delay(random(60, 100));
      else
        delay(random(35, 75));
    }

    // Si se sigue acariciando, gira la cabeza hacia el otro lado
    valorTacto = capSensor.capacitiveSensor(30);
    if (valorTacto > filtro) {
      for (i = servo.read(); i > LIMITE_INF; i--) {
        servo.write(i);
        if (i <= LIMITE_INF + suavizado)
          delay(random(60, 100));
        else
          delay(random(35, 75));
      }
    }
    delay(random(40, 100));

    // Revisa si se sigue acariciando o no para no quedar en un while infinito.
    valorTacto = capSensor.capacitiveSensor(30);
    // Vuelve la cabeza a su posicion inicial de forma suave.
    if (valorTacto < filtro) {
      if (i < valorInicialCuello) {
        for (i; i != valorInicialCuello; i++) {
          servo.write(i);
          delay(random(35, 75));
        }
      } else if (i > valorInicialCuello) {
        for (i; i != valorInicialCuello; i--) {
          servo.write(i);
          delay(random(35, 75));
        }
      }
    }
  }
}

void mirarIzquierda(bool& caminoLibre) {
  bool puedeCaminar = false;
  int rangoCamino = ANCHO_CAMINO;

  while (!caminoLibre && servo.read() < 180) {  // el limite seria 180 - el valor del ancho del robot (rango en el que no deben haber obstaculos para que pase el robot)
    servo.write(servo.read() + 1);
    if (sonar.ping_cm() < DISTANCIA_OBSTACULO) {
      puedeCaminar = true;
      rangoCamino--;
      if (rangoCamino == 0) {                            // Si por un rango en el que pueda pasar el robot no se encuentran obstaculos, está habilitado para direccionarse hacia allí y avanzar.
        servo.write(servo.read() - (ANCHO_CAMINO / 2));  // Apunta la cabeza hacia el centro del camino disponible.
        caminoLibre = true;
        // Girar todo su cuerpo hasta que queden en orientación derecha al igual que la cabeza
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
  /*while(!obstaculos && servo.read() > 0){ // el limite seria 0 + el valor del ancho del robot (rango en el que no deben haber obstaculos para que pase el robot)
      servo.write(servo.read()-1);
      // Tomar el valor inicial del servomotor en el que dejo de encontrar obstaculos
        // Si no encuentra obstaculos hasta un rango limite, luego se posiciona mirando al medio entre el valor max y el min del rango
        // Gira todo su cuerpo hasta que queden en orientación derecha al igual que la cabeza
      // Si no deja de encontrar obstaculos simplemente se va del while
      delay(20);
    }*/
}
void marchaAtras() {
  Serial.println("Dando marcha atras");
  servo.write(valorInicialCuello);
}
void analizarEntorno() {
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
      marchaAtras();
  }
}