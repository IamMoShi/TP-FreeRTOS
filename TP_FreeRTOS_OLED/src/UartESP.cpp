#include<Arduino.h>
#include"driver/uart.h"
#include"UartESP.h"
#include"ProducteurConsommateur.h"

uint8_t DataRX_RS232[LEN_RS232];
int rxBytes;

const uart_config_t uart_config = { 
  .baud_rate = 9600, 
  .data_bits = UART_DATA_8_BITS,
  .parity = UART_PARITY_DISABLE,
  .stop_bits = UART_STOP_BITS_1,
  .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
  .source_clk = UART_SCLK_APB, 
  };

// ───────────────────────────────────────────────────────────────────  //

void v_initUart() {
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0); 
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, RS232_RX, RS232_TX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    rxBytes = 0;
    xTaskCreatePinnedToCore(
        v_uartTask,                     /* Function to implement the task */
        "uartTask ",                    /* Name of the task */
        4000,                           /* Stack size in words */
        NULL,                           /* Task input parameter */
        2,                              /* Priority of the task */
        NULL,                           /* Task handle. */
        0);                             /* Core where the task should run */

    Serial.println("UART initiated");
}

// ───────────────────────────────────────────────────────────────────  //

void v_uartTask(void *pvParameters) {
    uint16_t co2 = 0;
    // --------------------------------------------------------------------------------------------//
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(UART_ESP_PERIOD);
    xLastWakeTime = xTaskGetTickCount();
    // --------------------------------------------------------------------------------------------//
    
    while (true) {
    // --------------------------------------------------------------------------------------------//
    xTaskDelayUntil( &xLastWakeTime, xFrequency);
    // --------------------------------------------------------------------------------------------//
    
    rxBytes = uart_read_bytes(UART_NUM_1, DataRX_RS232, LEN_RS232, 1000 / portTICK_PERIOD_MS);
    printf("Reading %d bytes\n", rxBytes);
    if (rxBytes > 0) {
        co2 = DataRX_RS232[0]*255 + DataRX_RS232[1];
        printf("---> CO2 %d \n", co2);
        v_addToBuffer(co2, CO2);
    }
  }
}