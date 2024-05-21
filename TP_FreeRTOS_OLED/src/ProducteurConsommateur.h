#ifndef PRODUCTEUR_CONSOMMATEUR_H
#define PRODUCTEUR_CONSOMMATEUR_H

#include<Arduino.h>
#include<stdio.h>

/* 
 * Fichier contenant les ressources nécessaire à la mise en place du modèle producteur
 * consommateur. Une fonction d'initialisation est présente dans le fichier .cpp correspondant.
 */

// Définitions
// ───────────────────────────────────────────────────────────────────  //

const int bufferSize   = 50;
const int DELAY_REMOVE = 100; // After 250ms waiting a semaphore, the program abort and send error data.  

// Déclarations types
// ───────────────────────────────────────────────────────────────────  //

enum DataTypeEnum {
  ERROR       = 0,
  Humidity    = 1,
  CO2         = 2,
  Temperature = 3
};

typedef struct {
  enum DataTypeEnum type; // Le type de production
  uint16_t value;         // Valeur de la production
} DataType;

// Déclarations fonctions
// ───────────────────────────────────────────────────────────────────  //

void v_initProducteurConsommateur();
void v_addToBuffer(u_int16_t value, DataTypeEnum type);
DataType DT_removeFromBuffer();

#endif // PRODUCTEUR_CONSOMMATEUR_H