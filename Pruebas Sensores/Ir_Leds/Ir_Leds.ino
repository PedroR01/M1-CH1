#include <IRremote.hpp>

const int irPin = 2;
const int ledPin = A5;
bool isOn = false;

const int sendPin = 5;

uint16_t sAddress = 0x0;
uint8_t sCommand = 0x5C;  //0xC boton apagado -- 0x5C boton Ok.
uint8_t sRepeats = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(sendPin, OUTPUT);
  Serial.begin(9600);
  IrReceiver.begin(irPin);
  IrSender.begin(sendPin);
}

void loop() {
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.decodedRawData == 92) {  // 0x5C = 92
      Serial.println("Led Infrarrojo recibido");
    }
    if (IrReceiver.decodedIRData.decodedRawData == 65548 || IrReceiver.decodedIRData.decodedRawData == 12)  // 1000C (mantener apretado) o C (apretar) en hexa
    {
      delay(100);
      IrReceiver.resume();  // Early enable receiving of the next IR frame

      if (!isOn) {
        //digitalWrite(sendPin, LOW);
        analogWrite(ledPin, 255);

      } else {
        //digitalWrite(sendPin, HIGH);
        // Intentando apagarlo con LED infrarrojo == NO FUNCIONA. Parece que no recibe directamente ninguna señal del LED Infrarrojo - Probar digitalWrite(high) y que reciba muchos 0 de valores.
       /* while (IrReceiver.decodedIRData.decodedRawData != 92) {
          if (IrReceiver.decode()) {
            Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
            IrSender.sendRC6(sAddress, sCommand, sRepeats);
            IrReceiver.resume();  // Early enable receiving of the next IR frame
          }
          delay(500);
        }*/
        analogWrite(ledPin, 0);
      }
      isOn = !isOn;
      if (IrReceiver.decodedIRData.decodedRawData == 92) {  // 0x5C = 92
        Serial.println("Led Infrarrojo recibido");
      }
    }


    // Debugg para decodificar la señal de un control y saber como enviarla por un LED infrarrojo
    /*
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
    IrReceiver.printIRResultShort(&Serial);
    IrReceiver.printIRSendUsage(&Serial);
    Serial.println();
    */

    IrReceiver.resume();  // Early enable receiving of the next IR frame
  }
}