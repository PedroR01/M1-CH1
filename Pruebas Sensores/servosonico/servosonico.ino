#include <Servo.h>
//#include <NewPing.h>

const int ServoPin = 9;
//const int TriggerPin = 12;
//const int EchoPin = 11;
//const int LED = 13;

//NewPing sonar (TriggerPin, EchoPin, 100);
Servo servo;

void setup() {
  Serial.begin(9600);
 // pinMode(LED, OUTPUT);
  servo.attach(ServoPin);
}
int i = 0;
void loop() {
  int valorAnalogico = analogRead(A0);
  //int distancia = sonar.ping_cm();
  int angulo = map(valorAnalogico, 0, 1023, 0, 360);
  i+=1;
  if (i > 90) i = 0;
  Serial.println(angulo);
  servo.write(angulo);

}
