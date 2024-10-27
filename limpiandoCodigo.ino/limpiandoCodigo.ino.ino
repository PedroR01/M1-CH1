#include <Servo.h>
#include <NewPing.h>
#include <CapacitiveSensor.h>

#define THRESHOLD 700
#define CAP_OUTPUT_PIN 10
#define CAP_INPUT_PIN 7
#define MOTOR_PIN 8
#define NECK_PIN A7
#define CENTERED_POS 90
#define TRIGGER_PIN 5    // Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PIN 3       // Arduino pin tied to echo pin on ping sensor.
#define MAX_DISTANCE 90  // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define OBSTACLE_DISTANCE 15

CapacitiveSensor capSensor = CapacitiveSensor(CAP_OUTPUT_PIN, CAP_INPUT_PIN);
Servo neck;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);  // NewPing setup of pins and maximum distance.

unsigned int pingSpeed = 50;  //120  // How frequently are we going to send out a ping (in milliseconds). 50ms would be 20 times a second.
unsigned long pingTimer;      // Holds the next ping time.

unsigned long simulatedMillis = 0;
unsigned long actual_time;
unsigned long last_observed_time = 0;
const unsigned long observe_compare = 600;  //400 //200

volatile bool petting = false;
volatile bool debeAnalizar = false;

void setup() {
  Serial.begin(9600);  // Open serial monitor at 115200 baud to see ping results.
  //pinMode(MOTOR_PIN, OUTPUT);
  neck.attach(NECK_PIN);
  neck.write(CENTERED_POS);
  pingTimer = millis();  // Start now.
}

void loop() {

  pingInterruptionCheck();
  // Do other stuff here.
  /* if (capSensor.capacitiveSensor(30) > THRESHOLD) {
    if (MOTOR_PIN == HIGH)
      digitalWrite(MOTOR_PIN, LOW);
    else
      digitalWrite(MOTOR_PIN, HIGH);
  }*/
  if (debeAnalizar) {
    actual_time = millis();
    last_observed_time = actual_time;
    seeArround();
  }
}

void pingInterruptionCheck() {
  if (millis() >= pingTimer) {    // pingSpeed milliseconds since last ping, do another ping.
    pingTimer += pingSpeed;       // Set the next ping time.
    sonar.ping_timer(echoCheck);  // Send out the ping, calls "echoCheck" function every 24uS where you can check the ping status.
  }
}

void echoCheck() {  // Timer2 interrupt calls this function every 24uS where you can check the ping status.
  // Don't do anything here!
  if (sonar.check_timer()) {  // This is how you check to see if the ping was received.
    // Here's where you can add code.
    Serial.print("Ping: ");
    Serial.print(sonarToCm());  // Ping returned, uS result in ping_result, convert to cm with US_ROUNDTRIP_CM.
    Serial.println("cm");

    // Analizar si hay algun obstaculo enfrente.
    if (sonarToCm() < OBSTACLE_DISTANCE && sonarToCm() != 0){
      debeAnalizar = true;
      simulatedMillis += 24 / 1000;
    }
  }
  // Don't do anything here!
}

// Cuando analiza el entorno no se va a mover, por lo que lo unico que lo puede
// interrumpir es acariciarlo
// FALTA TENER EN CUENTA SI EL ANCHO DEL GATO PERMITE QUE PASE POR EL MEJOR CAMINO DETECTADO
// Este metodo si se ejecuta desde la interrupcion del NewPing (Timer2) no le funcionan los metodos de Timer0 dentro (delay y millis).
void seeArround() {
  int anguloInicial = 45;
  int anguloFinal = 135;
  int paso = 5;
  int mejorAngulo = 90;
  int mejorDistancia = 0;


  // Mueve la cabeza para detectar el entorno
  // version con while - SOLO MIRA LADO , FALTA OTRO WHILE PARA MIRAR AL OTRO LADO
  int angulo = anguloInicial;
  bool noObstacle = false;
  //neck.write(30);
  for (int angulo = anguloInicial; angulo <= anguloFinal; angulo ++) {
    actual_time = millis();
    Serial.println(actual_time);

    if ((simulatedMillis - last_observed_time) >= observe_compare) {
      neck.write(angulo);

      int distancia = sonarToCm();
      if (distancia > mejorDistancia) {
        mejorDistancia = distancia;
        mejorAngulo = angulo;
      }
      last_observed_time = actual_time;
    }
  }

  for (int angulo = anguloFinal; angulo >= anguloInicial; angulo --) {
    actual_time = millis();
    Serial.println(actual_time);

    if ((simulatedMillis - last_observed_time) >= observe_compare) {
      neck.write(angulo);

      int distancia = sonarToCm();
      if (distancia > mejorDistancia) {
        mejorDistancia = distancia;
        mejorAngulo = angulo;
      }
      last_observed_time = actual_time;
    }
  }
 
//  Serial.println("Salio del while");

  // Usa booleano petting para chequear si se ejecutó la interrupción de la caricia. Si no se interrumpio el analisis, la cabeza gira a la mejor pos analizada
  if (!petting) {
    //neck.write(mejorAngulo);
    //Serial.println("Cabeza girada.");
    petting = true;
  } else if (petting) {
    petting = false;
  }

  // Si se encuentra encerrado debe retroceder y pegar la vuelta o analizar desde otra posicion
  if (mejorDistancia <= OBSTACLE_DISTANCE) {
    //Serial.println("Debe retroceder.");
    //retroceder();  -->  //cuello.write(valorInicialCuello);  // Vuelve a la posicion inicial del cuello
  }
  debeAnalizar = false;
}

int sonarToCm() {
  return (sonar.ping_result / US_ROUNDTRIP_CM);
}

bool inTime(int& observed_time) {
  if (millis() - observed_time >= observe_compare) {
    observed_time = millis();
    return true;
  }
  return false;
}