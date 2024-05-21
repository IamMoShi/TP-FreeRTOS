#include <stdio.h> 
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/uart.h"
#include "CO2.h"
#include "ProducteurConsommateur.h"



const uart_config_t CO2_uart_config = {
    .baud_rate = 9600,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .source_clk = UART_SCLK_APB,
};



void v_initCO2() {
    uart_driver_install(UART_NUM_1, CO2_RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &CO2_uart_config);
    uart_set_pin(UART_NUM_1, CO2_TXD_PIN, CO2_RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    xTaskCreatePinnedToCore(
        v_CO2Task, 
        "get_co2",
        configMINIMAL_STACK_SIZE * 3, 
        NULL,
        5, 
        NULL,
        1
    );

    printf("CO2 initialized \n");
}

void v_CO2Task() {
    int CO2_rxBytes, CO2_txBytes = 0; 

    const uint8_t CO2_data[CO2_UART_LEN] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
    uint8_t CO2_dataRX[CO2_UART_LEN];

    // ---------------------------------------//
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(CO2_PERIOD);
    xLastWakeTime = xTaskGetTickCount();
    // ---------------------------------------//

    while (true) {
        // ---------------------------------------//
        vTaskDelayUntil( &xLastWakeTime, xFrequency );
        // ---------------------------------------//

        CO2_txBytes = uart_write_bytes(UART_NUM_1, CO2_data, CO2_UART_LEN);
        CO2_rxBytes = uart_read_bytes(UART_NUM_1, CO2_dataRX, CO2_UART_LEN, 1000 / portTICK_PERIOD_MS);

        if (CO2_rxBytes > 0) {
            v_addToBufferPC(CO2_dataRX[2], CO2_dataRX[3]);

            printf("CO2 : %d\n", CO2_dataRX[2] * 255 + CO2_dataRX[3]);
        }
    }

}