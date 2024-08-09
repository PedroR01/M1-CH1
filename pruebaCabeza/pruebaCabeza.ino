#include <Servo.h>
#include <NewPing.h>

const int ServoPin = 3;
const int ServoPinDer = 2;
const int TriggerPin = 8;
const int EchoPin = 12;
//const int LED = 13;

NewPing sonar (TriggerPin, EchoPin, 100);
Servo servo;
Servo servoDer;

void setup() {
  Serial.begin(9600);
 // pinMode(LED, OUTPUT);
  servo.attach(ServoPin);
  servoDer.attach(ServoPinDer);
}
int i = 0;
void loop() {
  //int valorAnalogico = analogRead(A0);
  int distancia = sonar.ping_cm();
  int angulo = map(distancia, 2, 50, 0, 360);
  i+=1;
  if (i > 180) i = 0;
  Serial.println(distancia);
  servo.write(angulo);
  servoDer.write(i);
}
