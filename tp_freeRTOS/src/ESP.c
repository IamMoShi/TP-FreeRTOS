#include <stdio.h> 
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/uart.h"
#include "ESP.h"
#include "ProducteurConsommateur.h"



const uart_config_t ESP_uart_config = {
    .baud_rate = 9600,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .source_clk = UART_SCLK_APB,
};


void v_initESP() {
    uart_driver_install(UART_NUM_2, ESP_RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_2, &ESP_uart_config);
    uart_set_pin(UART_NUM_2, ESP_TXD_PIN, ESP_RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);


    xTaskCreatePinnedToCore(
        v_ESPTask, 
        "get_co2",
        configMINIMAL_STACK_SIZE * 3, 
        NULL,
        5, 
        NULL,
        1
    ); 

    printf("ESP initialized\n");
}


void v_ESPTask() {
    PC_Type data;
    int ESP_rxBytes, ESP_txBytes = 0;
    uint8_t ESP_data[ESP_UART_LEN];

    // ---------------------------------------//
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(ESP_PERIOD);
    xLastWakeTime = xTaskGetTickCount();
    // ---------------------------------------//
    
    while (true) {
        // ---------------------------------------//
        vTaskDelayUntil( &xLastWakeTime, xFrequency );
        // ---------------------------------------//

        data = PC_removeFromBufferPC();
        ESP_data[0] = data.value_1;
        ESP_data[1] = data.value_2;
        ESP_txBytes = uart_write_bytes(UART_NUM_2, ESP_data, ESP_UART_LEN);
    }
    
}
