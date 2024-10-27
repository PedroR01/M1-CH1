#include <CapacitiveSensor.h>

#define THRESHOLD 700
#define CAP_OUTPUT_PIN 10
#define CAP_INPUT_PIN 7
#define MOTOR_PIN 8

const uint16_t ini = 0;       // El nombre init no está disponible. Error
const uint16_t limit = 9375;  // Timer interruption frecuency / velocity
uint16_t check = 0;
bool status = LOW;

int modoGato = 0;
volatile bool checkPetting = false;
CapacitiveSensor capSensor = CapacitiveSensor(CAP_OUTPUT_PIN, CAP_INPUT_PIN);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  Serial.begin(9600);
  initTimer5();
}

void loop() {
  check = TCNT5;

  if (checkPetting)
    petting();

  if (digitalRead(MOTOR_PIN) == HIGH)
    modoGato++;

  Serial.println(modoGato);
}

void initTimer5() {
  TCNT5 = 0;  // Inicializar el contador en 0

  TCCR5B = 0;  // INICIO PRESCALAR EN 0
  // LUEGO CONFIGURO LOS BITS NECESARIOS PARA EL VALOR DE PRESCALAR QUE DESEO (1024 en este caso)
  TCCR5B |= (1 << CS50);
  //TCCR5B |= (1 << CS51);
  TCCR5B |= (1 << CS52);
  TCCR5B |= (1 << WGM52);  //CTC => Clear Timer on Compare

  TCCR5A = 0;
  TIMSK5 = 0;
  TIMSK5 |= (1 << OCIE5A);  // Habilita la interrupción de comparación del Timer5

  OCR5A = limit;  // Establecer el valor de comparación para el Timer5
}

void petting() {
  if (capSensor.capacitiveSensor(30) > THRESHOLD)
    digitalWrite(MOTOR_PIN, HIGH);
  else
    digitalWrite(MOTOR_PIN, LOW);
}

// Rutina de interrupción para el Timer5
ISR(TIMER5_COMPA_vect) {
  status = !status;
  checkPetting = !checkPetting;
  digitalWrite(LED_BUILTIN, status);
  check = OCR5A;
}
