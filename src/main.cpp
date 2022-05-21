#include <Arduino.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("Maunzikatze"); //Name des ESP32
  Serial.println("Der ESP32 ist bereit. Verbinde dich nun über Bluetooth.");
  BTScanResults* devices = SerialBT.discover();

}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  delay(25);
}