#include <IRremote.hpp>
const int recv_pin = 51;
decode_results results;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(recv_pin);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (IrReceiver.decode()) {
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
    //IrReceiver.printIRResultShort(&Serial);
    //IrReceiver.printIRSendUsage(&Serial);
    IrReceiver.resume();  // Early enable receiving of the next IR frame
    Serial.println();

    /*
    if(IrReceiver.decodedIRData.decodedRawData == x){
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000):
      digitalWrite(LED_BUILTIN, LOW);
    }
    */
  }
}