#include"ProducteurConsommateur.h"

// Déclaration variables globales
// ───────────────────────────────────────────────────────────────────  //

// --------------------------------------------------------------------------------------------//
SemaphoreHandle_t xBufferPC;      // Mutex
SemaphoreHandle_t xBufferPCPlein; // Nombre de place possible à lire
SemaphoreHandle_t xBufferPCVide;  // Nombre de place possible à écrire
// --------------------------------------------------------------------------------------------//

DataType BufferPC[bufferSize];    // Création du buffer contenant les productions
int positionP;                    // Position de la prochaine production à mettre dans le buffer
int positionC;                    // Position de la prochaine production à consommer


// ───────────────────────────────────────────────────────────────────  //

void v_initProducteurConsommateur() {

    positionP = 0;
    positionC = 0;

    // --------------------------------------------------------------------------------------------//
    xBufferPC = xSemaphoreCreateBinary();
    xSemaphoreGive(xBufferPC);
    xBufferPCPlein = xSemaphoreCreateCounting(bufferSize, 0);
    xBufferPCVide = xSemaphoreCreateCounting(bufferSize, bufferSize);
    // --------------------------------------------------------------------------------------------//

    printf("Producteur consommateur initialized\n");

}

// ───────────────────────────────────────────────────────────────────  //

void v_addToBuffer(u_int16_t value, DataTypeEnum type) {
    /* 
     * Cette fonction est appelée de manière asynchrone par les différents
     * producteurs. Elle permet uniquement d'éviter la duplucation de code dans les taks
     */


    DataType newData;

    newData.value = value;  // Complétion de la valeur de la production
    newData.type  = type;   // Type de production ajouter

    // --------------------------------------------------------------------------------------------//
    xSemaphoreTake(xBufferPCVide, portMAX_DELAY);  // Demande le droit d'écrire, il faut qu'il y ait une place libre dans le buffer
    xSemaphoreTake(xBufferPC, portMAX_DELAY);      // Demande le droit d'accès au buffer
    // --------------------------------------------------------------------------------------------//

    BufferPC[positionP] = newData;                 // Ajout de la production au buffer
    positionP = (positionP + 1) % bufferSize;

    // --------------------------------------------------------------------------------------------//
    xSemaphoreGive(xBufferPC);                     // Libération du droit d'accès au buffer
    xSemaphoreGive(xBufferPCPlein);                // Ajoute la possibilité de consommer une valeur de plus dans le buffer
    // --------------------------------------------------------------------------------------------//
}

// ───────────────────────────────────────────────────────────────────  //

DataType DT_removeFromBuffer() {
    /* 
     * Cette fonction est appelée de manière asynchrone par le(s) différents
     * consommateur. Elle permet uniquement d'éviter la duplucation de code dans les taks
     */
    
    DataType newData;
    // --------------------------------------------------------------------------------------------//
    if (xSemaphoreTake(xBufferPCPlein, pdMS_TO_TICKS(DELAY_REMOVE)) == pdTRUE) {
        // Demande le droit de consommer, il faut qu'il y ait une place libre dans le buffer
        if (xSemaphoreTake(xBufferPC, pdMS_TO_TICKS(DELAY_REMOVE)) == pdTRUE) {
            // Demande le droit d'accès au buffer
            newData = BufferPC[positionC];
            positionC = (positionC + 1) % bufferSize;

            // --------------------------------------------------------------------------------------------//
            xSemaphoreGive(xBufferPC);                     // Libération du droit d'accès au buffer
            xSemaphoreGive(xBufferPCVide);                // Ajoute la possibilité de consommer une valeur de plus dans le buffer
            // --------------------------------------------------------------------------------------------//
            return newData;
        } else {
            xSemaphoreGive(xBufferPCPlein);                 // We did not use to place
            newData.value = 0;
            newData.type = ERROR;
            return newData;
        }
    } else {
        newData.value = 0;
        newData.type = ERROR;
        return newData;
    }
          
}