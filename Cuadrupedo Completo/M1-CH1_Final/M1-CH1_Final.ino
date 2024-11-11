/*
  CUADRUPED ROBOT "M1CH1" FINAL VERSION FOR ARTIMAÑAS 2024 EXPO
*/

#include "Robot.h"

Robot michi;

volatile bool checkState = false;
const uint16_t limit = 9375;  // Timer5 interruption frecuency / velocity - - 9375 = int cada 0.6seg

void setup() {
  Serial.begin(9600);

  //initTimer5();
  michi.begin();
}

void loop() {
  michi.pingIntCheck();
  michi.path();
  //michi.move();
}

/*
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

// Rutina de interrupción para el Timer5
ISR(TIMER5_COMPA_vect) {
  checkState = !checkState;
  michi.setCheckState(checkState);
}
*/