#include "Piernas.h"

Piernas::Piernas() {
  alternar = true;

  piernaTI.attach(pinTraseraI);
  piernaTD.attach(pinTraseraD);
  piernaDI.attach(pinDelanteraI);
  piernaDD.attach(pinDelanteraD);
}

// Motricidad dividida en 2 partes --- ARREGLAR: Cambiar delay por millis(); Probar este codigo y luego el de Chat GPT
void Piernas::movimientoGatuno() {
  int posTD = 135;
  int ultimoValorPosTD = posTD;
  int posDD = 95;

  int posTI = 120;
  int ultimoValorPosTI = posTI;
  int posDI = 85;

  int velocidadPierna = 2;

  // Primera parte del movimiento
  while (posTD > 30 || posTI > 10) {
    if (posTD > 30) {
      piernaTD.write(posTD);
      posTD--;
      if (posTD < ultimoValorPosTD - velocidadPierna) {
        piernaDD.write(posDD);
        posDD--;
        ultimoValorPosTD = posTD;
      }
    }
    if (posTI > 10) {
      piernaTI.write(posTI);
      posTI--;
      if (posTI < ultimoValorPosTI - 1) {
        piernaDI.write(posDI);
        posDI--;
        ultimoValorPosTI = posTI;
      }
    }
    if (posTD > 70)
      delay(10);
    else
      delay(30);
  }

  ultimoValorPosTD = posTD;
  ultimoValorPosTI = posTI;

  // Segunda parte del movimiento
  while (posTD < 135 || posTI < 105) {
    if (posTD < 135) {
      piernaTD.write(posTD);
      posTD++;
      if (posTD > ultimoValorPosTD + velocidadPierna) {
        piernaDD.write(posDD);
        if (piernaTD.read() >= 135 / 2) {
          posDD++;
        } else
          posDD--;
        ultimoValorPosTD = posTD;
      }
    }
    if (posTI < 105) {
      piernaTI.write(posTI);
      posTI++;
      if (posTI > ultimoValorPosTI + 1) {
        piernaDI.write(posDI);
        if (piernaTI.read() >= 15) {
          posDI++;
        } else
          posDI--;
        ultimoValorPosTI = posTI;
      }
    }
    if (posTD < 70)
      delay(10);
    else
      delay(30);
  }
}

void Piernas::movimientoRobotico() {
  if (alternar) {
    if (piernaTI.read() < 120) {
      piernaTI.write(120);
      piernaTD.write(30);
    } else {
      piernaTI.write(10);
      piernaTD.write(135);
      this->alternar = false;
    }
    piernaDD.write(50);
    piernaDI.write(85);
  } else {
    if (piernaDI.read() <= 25) {
      piernaDD.write(55);
      piernaDI.write(80);
    } else {
      piernaDD.write(110);
      piernaDI.write(20);
      this->alternar = true;
    }
    piernaTI.write(10);
    piernaTD.write(135);
  }
}

void Piernas::retroceder() {
  int retrocederDistancia = 20;
  piernaTI.write(piernaTI.read() - retrocederDistancia);
  piernaTD.write(piernaTD.read() - retrocederDistancia);
  piernaDI.write(piernaDI.read() - retrocederDistancia);
  piernaDD.write(piernaDD.read() - retrocederDistancia);
  delay(1000);  // Espera para completar la maniobra de retroceso
}