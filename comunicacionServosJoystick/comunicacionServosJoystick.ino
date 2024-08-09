#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

void setup() {
  Serial.begin(9600);
  servo1.attach(13);
  servo2.attach(12);
  servo3.attach(3);
  servo4.attach(2);
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    int index = data.indexOf(',');
    int servo1Pos = data.substring(0, index).toInt();
    int servo2Pos = data.substring(index + 1, data.indexOf(',', index + 1)).toInt();
    index = data.indexOf(',', index + 1);
    int servo3Pos = data.substring(index + 1, data.indexOf(',', index + 1)).toInt();
    int servo4Pos = data.substring(data.indexOf(',', index + 1) + 1).toInt();

    // Limitar el rango de los servomotores entre 5 y 70
    servo1Pos = constrain(servo1Pos, 5, 70);
    servo2Pos = constrain(servo2Pos, 5, 70);
    servo3Pos = constrain(servo3Pos, 5, 70);
    servo4Pos = constrain(servo4Pos, 5, 70);

    servo1.write(servo1Pos);
    servo2.write(servo2Pos);
    servo3.write(servo3Pos);
    servo4.write(servo4Pos);
  }
}
