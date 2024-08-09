//#include <Stepper.h>
#include <Servo.h>

// Configuración del motor paso a paso
//const int stepsPerRevolution = 2048;  // Número de pasos por revolución para 28BYJ-48
//Stepper stepper(stepsPerRevolution, 24, 23, 25, 22);  // Pines conectados al ULN2003

// Configuración del servomotor
Servo servo;
const int servoPin = 3;  // Pin conectado al SG90

void setup() {
  // Inicializar motor paso a paso a una velocidad de 10 RPM
  //stepper.setSpeed(10);

  // Inicializar el servomotor
  servo.attach(servoPin);

  Serial.begin(9600);
  servo.write(60);

}

void loop() {

  //stepper.step(1024);
  //servo.write(180);
  //delay(1000);  // Esperar 1 segundo


  //stepper.step(1024);

  //servo.write(0);
  //delay(1000); 
}
