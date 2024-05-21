#include <stdio.h> 
#include <freertos/FreeRTOS.h>
#include "ultrasonic.h"

#define MAX_DISTANCE_CM             500  // 500 cm max
#define WAKEUP_DELAY_SENSIBILITY_MS 5000 // Temps entre la fin de la détection de présence et le signal d'extinction de la présence
#define TRIGGER_GPIO                33   
#define ECHO_GPIO                   32
#define DISTANCE_PERIOD             10   // in ms, very fast to catch fast a person passing in front of the sensor
#define TRIGGER_DISTANCE            50
#define DISTANCE_PIN_OUTPUT         25   // Pin that send the data to the other ESP

void v_initDistance();
void v_distanceTask();