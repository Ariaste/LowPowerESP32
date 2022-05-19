#include <Arduino.h>

const int led = 26;
const int t = 500;

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  digitalWrite(led, HIGH);
  delay(t);
  digitalWrite(led, LOW);
  delay(t);
}