#include <Servo.h>
const int servoPin = A5;
Servo motor;
void setup() {
  motor.attach(servoPin);
}

void loop() {
  motor.write(60);
  delay(1000);
  motor.write(140);
  delay(1000);
}
