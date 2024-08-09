#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

void setup() {
  Serial.begin(9600);
  servo1.attach(3);
  servo2.attach(2);
  servo3.attach(13);
  servo4.attach(12);
  
  servo1.write(50);
  servo2.write(50);
  servo3.write(50);
  servo4.write(50);

}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    int commaIndex1 = data.indexOf(',');
    int commaIndex2 = data.indexOf(',', commaIndex1 + 1);
    int commaIndex3 = data.indexOf(',', commaIndex2 + 1);

    if (commaIndex1 > 0) {
      String value1 = data.substring(0, commaIndex1);
      String value2 = data.substring(commaIndex1 + 1, commaIndex2);
      String value3 = data.substring(commaIndex2 + 1, commaIndex3);
      String value4 = data.substring(commaIndex3 + 1);

      int servo1Value = value1.toInt();
      int servo2Value = value2.toInt();
      int servo3Value = value3.toInt();
      int servo4Value = value4.toInt();

      // Asegurar que los valores estén dentro del rango permitido
      servo1Value = constrain(servo1Value, 0, 90);
      servo2Value = constrain(servo2Value, 0, 90);
      servo3Value = constrain(servo3Value, 5, 90);
      servo4Value = constrain(servo4Value, 5, 90);

      // Ajustar los servomotores
      servo1.write(servo1Value);
      servo2.write(servo2Value);
      servo3.write(servo3Value);
      servo4.write(servo4Value);

      Serial.print("Servo1: "); // 48 para adelantarse
      Serial.print(servo1Value);
      Serial.print(" Servo2: "); // 47 para adelantarse
      Serial.println(servo2Value);
      Serial.print(" Servo3: ");
      Serial.println(servo3Value); // en 10 se para
      Serial.print(" Servo4: ");
      Serial.println(servo4Value); // en 85 se para
    }
  }
}
