#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

const int ledPin = 2;     // LED pin
const int touchPin = 14;  // Touch pin (T4)
int touchThreshold = 30;  // Adjust after testing
bool ledState = false;    // To track last LED state

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  SerialBT.begin("ESP_MASTER", true);  // true = master mode
  Serial.println("ESP32 ready for touch");
}

void loop() {
  int touchValue = touchRead(touchPin);
  Serial.println(touchValue);

  bool isTouched = (touchValue <= touchThreshold);

  if (isTouched && !ledState) {
    ledState = true;
    digitalWrite(ledPin, HIGH);
    SerialBT.println("HIGH");
    Serial.println("Touch detected → LED ON, Sent HIGH");
  } 
  else if (!isTouched && ledState) {
    ledState = false;
    digitalWrite(ledPin, LOW);
    SerialBT.println("LOW");
    Serial.println("Touch released → LED OFF, Sent LOW");
  }

  delay(100);
}
