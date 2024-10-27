#define TTP223 24

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TTP223, INPUT);
}

void loop() {
  if (digitalRead(TTP223) == HIGH) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Encendido");
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Encendido");
  }
}