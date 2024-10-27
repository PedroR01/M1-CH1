#pragma once
#ifndef CABEZA_H
#define CABEZA_H

#include <Arduino.h>
#include <Servo.h>
#include <NewPing.h>
#include <CapacitiveSensor.h>

class Cabeza {
private:

  // Pin conectado a la base del transistor para activar el motor de vibración 3v
  const int motorPin = 9;

  // El pin 5 es el que capta el cambio de corriente producido por el tacto humano.
  CapacitiveSensor tacto = CapacitiveSensor(6, 5);
  const int filtro = 300;

  // Ultrasonico HC-SR04
  const int triggerPin = 8;
  const int echoPin = A0;
  NewPing ojos = NewPing(triggerPin, echoPin, 100);


  // Servomotor SG-90
  Servo cuello;
  const int cuelloPin = A1;
  const int limiteSup = 100;
  const int limiteInf = 80;
  const int valorInicialCuello = 90;

  // Temporizadores
  int tiempoActual;
  unsigned long tCaricias;
  unsigned long tObservacion;
  int temporizador;

public:
  Cabeza();  // Constructor
  void observar();
  void acariciar(float modoGato);
};

#endif  // !CABEZA_H