#include <stdio.h> 
#include <freertos/FreeRTOS.h>
#include "freertos/semphr.h"
#include <freertos/task.h>
#include "ProducteurConsommateur.h"


QueueHandle_t xQueue;

void v_initProducteurConsommateur() {
    xQueue = xQueueCreate(BUFFER_SIZE, sizeof(PC_Type));
    printf("Producteur Consommateur Initialized\n");
}


void v_addToBufferPC(uint8_t value_1, uint8_t value_2) {
    PC_Type data;
    data.value_1 = value_1;
    data.value_2 = value_2;
    if (xQueueSend(xQueue, &data, portMAX_DELAY) != pdPASS) {
        printf("Error ! impossible to add data to queue !\n");
    }
}

PC_Type PC_removeFromBufferPC() {
    PC_Type data;
    if (xQueueReceive(xQueue, &data, portMAX_DELAY) == pdPASS) {
        return data;
    } else {
        data.value_1 = 0;
        data.value_2 = 0;
        printf("Error ! Impossible to remive data from queue\n");
        return data;
    }
}