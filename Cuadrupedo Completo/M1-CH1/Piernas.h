#pragma once
#ifndef PIERNAS_H
#define PIERNAS_H

#include <Arduino.h>
#include <Servo.h>

class Piernas {
private:
  const int pinTraseraI = 12;
  const int pinTraseraD = 13;
  const int pinDelanteraI = 3;
  const int pinDelanteraD = 2;

  Servo piernaTI; // Trasera Izquierda
  Servo piernaTD; // Trasera Derecha
  Servo piernaDI; // Delantera Izquierda
  Servo piernaDD; // Delantera Derecha

  bool alternar;

public:
  Piernas();
  void movimientoGatuno();
  void movimientoRobotico();
  void retroceder();
};

#endif  // !PIERNAS_H