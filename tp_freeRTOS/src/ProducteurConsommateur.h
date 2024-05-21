#include <stdio.h> 
#include <freertos/FreeRTOS.h>


#define BUFFER_SIZE 10

typedef struct {
    uint8_t value_1;
    uint8_t value_2;
} PC_Type;


void v_initProducteurConsommateur();
void v_addToBufferPC(uint8_t value_1, uint8_t value_2);
PC_Type PC_removeFromBufferPC();