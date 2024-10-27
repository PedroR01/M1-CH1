#include <Servo.h>
#include <NewPing.h>
#include <CapacitiveSensor.h>

const int filtro = 400;
const int motorPin = 8;

const int cuelloPin = A7;
const int triggerPin = 5;
const int echoPin = 3;

CapacitiveSensor capSensor = CapacitiveSensor(10, 7);
NewPing sonar(triggerPin, echoPin, 100);

Servo cuello;

const int piernaTrasIzqPin = A1;
const int piernaTrasDerPin = A5;
const int piernaDelDerPin = A3;
const int piernaDelIzqPin = A8;

Servo piernaTI;
Servo piernaTD;
Servo piernaDI;
Servo piernaDD;

void setup() {
  Serial.begin(9600);

  pinMode(motorPin, OUTPUT);

  inicializarServos();
}

void loop() {
  Serial.println(testUS());
  //testMovimiento();
 // testCabeza();
}

void inicializarServos() {
  cuello.attach(cuelloPin);
  cuello.write(90);

  piernaTI.attach(piernaTrasIzqPin);
  piernaTD.attach(piernaTrasDerPin);
  piernaDI.attach(piernaDelIzqPin);
  piernaDD.attach(piernaDelDerPin);

  piernaTI.write(80);   // 80 derecho - 150 atras - 35 adelante
  piernaTD.write(130);  // 130 derecho - 45 atras - 170 adelante
  piernaDI.write(80);   // 80 derecho - 150 atras - 20 adelante
  piernaDD.write(100);  // 100 derecho - 40 atras - 170 adelante
}

int testUS() {
  int distancia = sonar.ping_cm();
  return distancia;
}

int testCapacitive() {
  int tacto = capSensor.capacitiveSensor(30);
  return tacto;
}

void testMotor(bool power) {
  if (power)
    digitalWrite(motorPin, HIGH);
  else
    digitalWrite(motorPin, LOW);
}

void testCabeza() {
  if (sonar.ping_cm() < 10) {
    int i = 90;
    while (i < 140) {
      i++;
      cuello.write(i);
      testMimos();
    }
    delay(500);
    while (i > 40) {
      i--;
      cuello.write(i);
      testMimos();
    }
    delay(500);
    while (i < 90) {
      i++;
      cuello.write(i);
      testMimos();
    }
    delay(500);
  }
}

void testMimos() {
  if (testCapacitive() > filtro)
    testMotor(true);
  else
    testMotor(false);
}

void testMovimiento() {
  int posTD = 130;  // Pierna recta
  int ultimoValorPosTD = posTD;
  int posDD = 130;  // Pierna recta

  int posTI = 150;  // Pierna hacia atras
  int ultimoValorPosTI = posTI;
  int posDI = 100;  // Pierna ligeramente hacia atras

  int offsetDelantera = 2;  // Diferencia de tiempo de actualizacion respecto a la pierna trasera.

  // Primera parte del movimiento
  while (posTD > 45 || posTI > 35) {  // Mientras la trasera D no esté atras O la trasera I no esté adelante
    if (posTD > 45) {
      piernaTD.write(posTD);
      posTD--;
      if (posTD < ultimoValorPosTD - offsetDelantera) {
        piernaDD.write(posDD);
        posDD--;
        ultimoValorPosTD = posTD;
      }
    }
    if (posTI > 35) {
      piernaTI.write(posTI);
      if (posTD <= 45)
        posTI -= 2;
      else
        posTI--;
      if (posTI < ultimoValorPosTI - 1) {
        piernaDI.write(posDI);
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
  while (posTD < 170 || posTI < 160) {  // Mientras la trasera D no esté adelante O la trasera I no esté atras
    if (posTD < 170) {
      piernaTD.write(posTD);
      posTD++;
      if (posTD > ultimoValorPosTD + offsetDelantera) {
        piernaDD.write(posDD);
        if (piernaTD.read() >= 110) {  // 135/2
          posDD += offsetDelantera + 1;
        } else
          posDD--;
        ultimoValorPosTD = posTD;
      }
    }
    if (posTI < 160) {
      piernaTI.write(posTI);
      posTI++;
      if (posTI > ultimoValorPosTI + 1) {
        piernaDI.write(posDI);
        if (piernaTI.read() >= 15) {
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
