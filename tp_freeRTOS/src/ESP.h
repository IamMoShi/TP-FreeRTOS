#include <stdio.h> 
#include <freertos/FreeRTOS.h>
#include "driver/uart.h"

#define ESP_RX_BUF_SIZE  1024
#define ESP_TXD_PIN      26
#define ESP_RXD_PIN      27
#define ESP_UART_LEN     2
#define ESP_PERIOD       10000

void v_initESP();
void v_ESPTask();