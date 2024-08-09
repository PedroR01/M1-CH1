char dato;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  if (Serial.available() > 0) {
    dato = Serial.read();

    if (dato == '1')
      digitalWrite(LED_BUILTIN, HIGH);

    if (dato == 'a')
      digitalWrite(LED_BUILTIN, LOW);

    delay(2000);
  }
}
