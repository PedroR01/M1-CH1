#include <Servo.h>
#define UMBRAL_ANGULOS 20

Servo servo;

const int ServoPin = 9;
int valorAnterior = 0;
unsigned long milisPrevios = 0;
const long intervalo = 50;

int diferencia(int num1, int num2) {
  int resultado = num1 - num2;
  if (resultado < 0)
    resultado = resultado * (-1);
  return resultado;
}

void setup() {
  Serial.begin(9600);
  servo.attach(ServoPin);
}

void loop() {
  int valorAnalogico = analogRead(A0);
  int angulo = map(valorAnalogico, 0, 1023, 0, 85);
  //unsigned long milisActuales = millis();

  /* if((milisActuales - milisPrevios) >= intervalo){
    milisPrevios = milisActuales;
    angulo = map(valorAnalogico, 0, 1023, 0, 85);

    Serial.println (diferencia(angulo, valorAnterior));
  } else
    valorAnterior = angulo;*/

  //if(diferencia(angulo, valorAnterior) > UMBRAL_ANGULOS)
  int rotacion = 0;
  bool ladoA = true;
  bool ladoB = false;

  while (ladoA) {
    unsigned long milisActuales = millis();

    if (rotacion < 89 && ((milisActuales - milisPrevios) >= intervalo)) {
      rotacion++;
      servo.write(rotacion);
      milisPrevios = milisActuales;
    } else if (rotacion >= 89) {
      ladoB = true;
      ladoA = false;
    }
  }
  while (ladoB) {
    unsigned long milisActuales = millis();

    if (rotacion > 0 && ((milisActuales - milisPrevios) >= intervalo)) {
      rotacion--;
      servo.write(rotacion);
      milisPrevios = milisActuales;
    } else if (rotacion <= 0) {
      ladoA = true;
      ladoB = false;
    }
  }
  //servo.write(angulo);
}
