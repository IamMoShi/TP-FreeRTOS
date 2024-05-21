#include<Arduino.h>
#include<stdio.h>
#include"driver/uart.h"

// Définitions
// ───────────────────────────────────────────────────────────────────  //
const int UART_ESP_PERIOD = 10000;
const int RS232_RX        = 27;
const int RS232_TX        = 26;
const int RX_BUF_SIZE     = 1024;
const int LEN_RS232       = 2;

// Déclarations fonctions
// ───────────────────────────────────────────────────────────────────  //

void v_initUart();
void v_uartTask(void *pvParameters); // Initialisation de la tâche

// Déclarations variables globales
// ───────────────────────────────────────────────────────────────────  //

