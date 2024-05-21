#include <stdio.h> 
#include <freertos/FreeRTOS.h>
#include "driver/uart.h"

#define CO2_RX_BUF_SIZE  1024
#define CO2_TXD_PIN      17
#define CO2_RXD_PIN      16
#define CO2_UART_LEN     9
#define CO2_PERIOD       10000


void v_initCO2();
void v_CO2Task();