#ifndef DISPLAYDATA_h
#define DISPLAYDATA_h

#include<Arduino.h>
#include<stdio.h>
#include <Wire.h>
#include <SSD1306Wire.h>

// Définitions
// ───────────────────────────────────────────────────────────────────  //

const u_int8_t DISTANCE_PIN_INPUT = 25;
const uint16_t DISTANCE_PERIOD    = 500;

// Déclarations fonctions
// ───────────────────────────────────────────────────────────────────  //

void v_initDisplay();
void v_displayTask(void *pvParameters);

// Déclarations variables globales
// ───────────────────────────────────────────────────────────────────  //



#endif // DISPLAYDATA_H