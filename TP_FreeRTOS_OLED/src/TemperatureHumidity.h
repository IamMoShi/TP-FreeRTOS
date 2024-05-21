#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include<Arduino.h>
#include"DHTesp.h"


// Définitions
// ───────────────────────────────────────────────────────────────────  //
const int DHT_PIN                     = 15;
const int TEMPERATURE_HUMIDITY_PERIOD = 5000;

// Déclarations fonctions
// ───────────────────────────────────────────────────────────────────  //

void v_initTemp();
void v_HumidityTemperatureTask(void *pvParameters); // Initialisation de la tâche

// Déclarations variables globales
// ───────────────────────────────────────────────────────────────────  //



#endif // TEMPERATURE_H