#include <Arduino.h>


void setup() {
  Serial.begin(115200);
  printf("hello world !");
}

void loop() {
  Serial.println("Yes man !");
}

