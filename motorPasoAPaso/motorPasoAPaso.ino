#include <Stepper.h>

const int pasosPorVuelta = 612; 
Stepper motor(pasosPorVuelta, 24, 23, 25, 22);
void setup() {
  motor.setSpeed(15);
}

void loop() {
  motor.step(pasosPorVuelta);
  delay(500);
  motor.step(-pasosPorVuelta);
  delay(500);
}
