#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("KienOS X3 booted (528x792, ESP32-C3)");
}

void loop() {
  delay(1000);
}
