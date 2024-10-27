const uint16_t ini = 0; // El nombre init no está disponible. Error
const uint16_t limit = 9375;
uint16_t check = 0;
bool status = LOW;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  TCNT1 = 0;

  TCCR1B = 0;  // INICIO PRESCALAR EN 0
  // LUEGO CONFIGURO LOS BITS NECESARIOS PARA EL VALOR DE PRESCALAR QUE DESEO (1024 en este caso)
  TCCR1B |= (1 << CS10);
  //TCCR1B |= (1 << CS11);
  TCCR1B |= (1 << CS12);
  TCCR1B |= (1 << WGM12);  //CTC => Clear Timer on Compare

  TCCR1A = 0;
  TIMSK1 = 0;
  TIMSK1 |= (1 << OCIE1A);  // Poniendo este bit en 1 le dice a arduino que cuando el valor del reloj alcance el valor de OCR1A
  // se ejecute la interrupcion

  OCR1A = limit;
}

void loop() {
  Serial.println(status);
  check = TCNT1;
}

// Rutina de interrupción
ISR(TIMER1_COMPA_vect){
status = !status;
digitalWrite(LED_BUILTIN, status);
check = OCR1A;
}
