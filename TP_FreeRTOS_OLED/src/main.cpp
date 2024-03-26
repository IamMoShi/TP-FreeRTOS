#include <Arduino.h>
#include "ultrasonic.h"

#define MAX_DISTANCE_CM 30 // 500 cm max

#define TRIGGER_GPIO 33
#define ECHO_GPIO 32

void setup() {
  Serial.begin(115200);
  printf("hello world !");
}

void loop() {
  Serial.println("loop");
  delay(500);
}

