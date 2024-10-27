#include "Interaccion.h"

// CUANDO SE LO ACARICIA, SE INTERRUMPE MOVIENDO LA CABEZA Y VIBRANDO EL MOTOR.
void Interaccion::acariciar(float modoGato) {
/*  while (tacto.capacitiveSensor(30) >= filtro) {
    modoGato++;
    int i;
    int suavizado = random(6, 13);
    digitalWrite(motorPin, HIGH);

    for (i = cuello.read(); i < LIMITE_SUP; i++) {
      cuello.write(i);
      if (i >= LIMITE_SUP - suavizado)
        delay(random(60, 100));
      else
        delay(random(35, 75));
    }
    if (modoGato < 2.5)
      modoGato += 0.5;

    // Si se sigue acariciando, gira la cabeza hacia el otro lado
    if (tacto.capacitiveSensor(30) >= filtro) {
      for (i = cuello.read(); i > LIMITE_INF; i--) {
        cuello.write(i);
        if (i <= LIMITE_INF + suavizado)
          delay(random(60, 100));
        else
          delay(random(35, 75));
      }
      if (modoGato < 2.5)
        modoGato += 0.5;
    }
  }
  digitalWrite(motorPin, LOW);
  Serial.print("Valor modoGato en mimos: ");
  Serial.println(modoGato);*/
}