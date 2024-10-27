float distancia;     //Variable altura
long tiempo;          //Variable para el tiempo
bool initHC = true;  //Variable para acceder a la funcion HCSR04

float lect[10];     //Vector donde guardaremos 10 lecturas del sensor
int cont = 0;       //contador de lecturas del sensor 
int contActivo = 0;          //contador de lecturas que indican que el sensor esta activado
int buzzerFlag = 0;          //Bandera para acceder a la funcion buzzer
int umbral = 5;     //Variable que indica cuantas lecturas seguidas de deteccion sean necesarias para activar la alarma
float distanciaProm = 0.00;   //Distancia promedio

// Interrupcion hardware para medir la duración del pulso Echo 

const int echoPin = 3;

volatile float PulsoAlt;   //Contador de tiempo en alto del pulso eco
volatile long contAltInit;  //Punto de inicio del conteo de la señal eco

void INTalt() {
    if (digitalRead(echoPin) == HIGH) contAltInit = micros();  //Pulso eco en alto
    if (digitalRead(echoPin) == LOW) {                         //Pulso eco en bajo - ACA PODRIA IR SIMPLEMENTE UN ELSE
        PulsoAlt = micros() - contAltInit;               //Fin del conteo
        initHC = true;                                   //Bandera para ingresar a la funcion HCSR04
    }
}

void setup() {
    pinMode(echoPin, INPUT_PULLUP); // Pin Echo 
    pinMode(10, OUTPUT);      // pin Trigger 
    pinMode(12, OUTPUT);      //pin Buzzer
    pinMode(21, OUTPUT);      //pin Led
    attachInterrupt(digitalPinToInterrupt(3), INTalt, CHANGE);  //Interrupcion en el pin echoPin activada por CHANGE ejecuta funcion INTalt
    Serial.begin(115200);     //Inicializar comunicación serial
}

void loop() {

    while (micros() - tiempo < 6000);     //se mantiene el programa en este punt por 60ms
    tiempo = micros();                    //Se actualiza la variable tiempo para el nuevo conteo de 60 ms
    HCSR04();                             //Función de generación del pulso trigger del sensor y lectura de la distancia

    
    if (cont < 10) {                     //Se guarda en un vector de 10 posiciones, 10 lecturas consecutivas del sensor
        lect[cont] = distancia;
        cont = cont + 1;                //la variable cont nos indica la posición de la lectura en el vector
    }

    if (cont == 9) {                             //Una vez el vector esta lleno
        for (int i = 0; i < 10; i++) {
             distanciaProm = distanciaProm + lect[i];   //Sumamos el contenido del vector y guardamos el resultado en distanciaProm
        }
        distanciaProm = distanciaProm / 10;           //Se divide el total de la suma en la cantidad de datos
        Serial.print("Distancia prom ");              //Imprimimos en el monitor serie el valor de la distancia promedio
        Serial.println(distanciaProm);


        if (distanciaProm < 100) {                 //Si la distancia promedio es menor al valor del umbral
            contActivo = contActivo + 1;                             //Incrementamos la variable de conteo contActivo
            Serial.println(contActivo);                     //Imprimimos en el monitor serial el valor de la variable de conteo 
        }
        else {
            digitalWrite(21, 0);                   //Si la distancia promedio es mayor al valor del umbral, apagamos el led
            contActivo = 0;                                 //Reiniciamos la variable de conteo contActivo
            buzzerFlag = 1;                                 //Asignamos el valor de 1 a la bandera de la funcion buzzer
            Serial.println(contActivo);                     //Imprimimos en el monitor serial el valor de la variable de conteo   
        }

        if (contActivo > umbral) { 
            digitalWrite(21, 1);                   //Si la variable de conteo contActivo adquiere el valor de umbral, encendemos el led
            Serial.println(buzzerFlag);                     //Imprimimos en el monitor serial el valor de la bandera de la funcion buzzer                   

            if (buzzerFlag == 1) {
                buzzer();                         //Ejecutamos la funcion buzzer
            }
        }
        cont = 0;                                 //Reiniciamos el contador de numero de datos del vector
        distanciaProm = 0;                        //Reiniciamos el valor de la distancia promedio
    }


}

void HCSR04() {
    if (initHC == true) {
        distancia = PulsoAlt * 0.01715; // Calcular distancia en función del ancho de pulso 
        digitalWrite(10, 1);  //  Salida D10 en alto (pulso trigger)
        delayMicroseconds(10); // Esperamos 10us 
        digitalWrite(10, 0);    // Salida D10 en bajo 
        initHC = false;         //Desactivamos la bandera de ingreso a la funcion  
    }
}

void buzzer() {
    digitalWrite(12, 1);           //activamos el buzzer por 300ms
    Serial.println("buzzer on");   //imprimimos en el monitor serie que el buzzer esta activado
    delay(300);
    digitalWrite(12, 0);           //desactivamos el buzzer por 300ms
    Serial.println("buzzer off");  //imprimimos en el monitor serie que el buzzer esta desactivado
    delay(300);
    digitalWrite(12, 1);
    Serial.println("buzzer on");
    delay(300);
    digitalWrite(12, 0);
    Serial.println("buzzer off");
    delay(300);
    digitalWrite(12, 1);
    Serial.println("buzzer on");
    delay(300);
    digitalWrite(12, 0);
    Serial.println("buzzer off");
    delay(300);
    buzzerFlag = 0;                         //desactivamos la bandera de ingreso de la funcion buzzer
}
