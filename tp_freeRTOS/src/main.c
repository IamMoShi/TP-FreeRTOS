#include <stdio.h> 
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdbool.h>
#include <stdlib.h>
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/semphr.h"
#include "ultrasonic.h"
#include "driver/uart.h"
#include <string.h>
#include <math.h>

void affchage_binaire(unsigned int n) {
    int i;
    printf("%d en binaire : ", n);
    for (i = 31; i >= 0; i--) {
        printf("%d", (n >> i ) & 1);
    }
    printf("\n");
} 

/* ////////////////////////////////////////////////////////////////////////////////////////////// */
/*---------------------------------------DISTANCE_SENSOR------------------------------------------*/
/* ////////////////////////////////////////////////////////////////////////////////////////////// */

void v_initDistance();
void v_getDistance();

// ───────────────────────────────────────────────────────────────────  //

#define MAX_DISTANCE_CM 500              // 500 cm max
#define WAKEUP_DELAY_SENSIBILITY_MS 5000 // Temps entre la fin de la détection de présence et le signal d'extinction de la présence
#define TRIGGER_GPIO 33
#define ECHO_GPIO 32

TaskHandle_t distanceTaskHandle = NULL;
ultrasonic_sensor_t sensor      = {
                                    .trigger_pin = TRIGGER_GPIO,
                                    .echo_pin = ECHO_GPIO
                                  };
float distance;

SemaphoreHandle_t xDistance;

// ───────────────────────────────────────────────────────────────────  //

void v_initDistance() {
    ultrasonic_init(&sensor);
    
    // ---------------------------------------//
    xDistance = xSemaphoreCreateBinary();
    xSemaphoreGive(xDistance);
    // ---------------------------------------//

    xTaskCreatePinnedToCore(
        v_getDistance,
        "get_distance",
        configMINIMAL_STACK_SIZE * 3,
        NULL,
        5, 
        &distanceTaskHandle,
        1
    );
    
    printf("v_initDistance Done\n");
}

// ───────────────────────────────────────────────────────────────────  //

void v_getDistance() {
    // ---------------------------------------//
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 100;
    xLastWakeTime = xTaskGetTickCount();
    // ---------------------------------------//

    while (true) {
        // ---------------------------------------//
        vTaskDelayUntil( &xLastWakeTime, xFrequency);
        xSemaphoreTake(xDistance, portMAX_DELAY);
        // ---------------------------------------//

        ultrasonic_measure(&sensor, MAX_DISTANCE_CM, &distance);
        distance = distance * 100;
        // printf("Distance : %0.04f cm\n", distance);

        // ---------------------------------------//
        xSemaphoreGive(xDistance);
        // ---------------------------------------//
    }
}

// ───────────────────────────────────────────────────────────────────  //

/* ////////////////////////////////////////////////////////////////////////////////////////////// */
/*-----------------------------------END_DISTANCE_SENSOR------------------------------------------*/
/* ////////////////////////////////////////////////////////////////////////////////////////////// */

/* ////////////////////////////////////////////////////////////////////////////////////////////// */
/*-------------------------------------------CO_SENSOR--------------------------------------------*/
/* ////////////////////////////////////////////////////////////////////////////////////////////// */

// ───────────────────────────────────────────────────────────────────  //

void v_initCo2();
void v_get_co2(); 

// ───────────────────────────────────────────────────────────────────  //

int RX_BUF_SIZE = 1024;
int TXD_PIN     = 17;
int RXD_PIN     = 16;

int rxBytes; 
int txBytes;

uint8_t data[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
int len = 9;

uint8_t DataRX[9];
u_int8_t co2_1 = 0;
u_int8_t co2_2 = 0;

SemaphoreHandle_t xCo2;

const uart_config_t uart_config = {
    .baud_rate = 9600,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .source_clk = UART_SCLK_APB,
};

// ───────────────────────────────────────────────────────────────────  //

void v_initCo2() {
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // ---------------------------------------//
    xCo2 = xSemaphoreCreateBinary();
    xSemaphoreGive(xCo2);
    // ---------------------------------------// 
    
    xTaskCreatePinnedToCore(
        v_get_co2, 
        "get_co2",
        configMINIMAL_STACK_SIZE * 3, 
        NULL,
        5, 
        NULL,
        1
    );

    printf("v_initCo2 Done \n");
}

// ───────────────────────────────────────────────────────────────────  //

void v_get_co2() {
    // ---------------------------------------//
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 100;
    xLastWakeTime = xTaskGetTickCount();
    // ---------------------------------------//

    while (true) {

        // ---------------------------------------//
        vTaskDelayUntil( &xLastWakeTime, xFrequency );
        xSemaphoreTake(xCo2, portMAX_DELAY);
        // ---------------------------------------//

        txBytes = uart_write_bytes(UART_NUM_1, data, len);
        rxBytes = uart_read_bytes(UART_NUM_1, DataRX, len, 1000 / portTICK_PERIOD_MS);
        
        if (rxBytes > 0) {
            co2_1 = DataRX[2];
            co2_2 = DataRX[3];
            // printf("CO2 : %d\n", co2_1 * 255 + co2_2);
        }

        // ---------------------------------------//
        xSemaphoreGive(xCo2);
        // ---------------------------------------//
    }
}

// ───────────────────────────────────────────────────────────────────  //

/* ////////////////////////////////////////////////////////////////////////////////////////////// */
/*-------------------------------------------END_CO_SENSOR----------------------------------------*/
/* ////////////////////////////////////////////////////////////////////////////////////////////// */

/* ////////////////////////////////////////////////////////////////////////////////////////////// */
/*-----------------------------------------UART_CONNECTION_ESP------------------------------------*/
/* ////////////////////////////////////////////////////////////////////////////////////////////// */

// ───────────────────────────────────────────────────────────────────  //

void v_initUartEsp();
void v_uartEspTask();

// ───────────────────────────────────────────────────────────────────  //

int RX_BUF_SIZE_2 = 1024;
int TXD_PIN_2     = 26;
int RXD_PIN_2     = 27;

int rxBytes_2; 
int txBytes_2;

uint8_t data_2[4];
int len_2 = 4;

uint8_t DataRX_2[4];

const uart_config_t uart_config_2 = {
    .baud_rate = 9600,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .source_clk = UART_SCLK_APB,
};

// ───────────────────────────────────────────────────────────────────  //

void v_initUartEsp() {
    uart_driver_install(UART_NUM_2, RX_BUF_SIZE_2 * 2, 0, 0, NULL, 0); 
    uart_param_config(UART_NUM_2, &uart_config_2);
    uart_set_pin(UART_NUM_2, RXD_PIN_2, TXD_PIN_2, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    xTaskCreatePinnedToCore(
        v_uartEspTask,
        "uartEspTask",
        4000,
        NULL,
        3,
        NULL,
        0
    );

    printf("v_initUartEsp Done \n");
}

// ───────────────────────────────────────────────────────────────────  //

void v_uartEspTask() {
    // ---------------------------------------//
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 100;
    xLastWakeTime = xTaskGetTickCount();
    // ---------------------------------------//
    while (true) {
        // ---------------------------------------//
        xTaskDelayUntil( &xLastWakeTime, xFrequency);
        xSemaphoreTake(xDistance, portMAX_DELAY);
        xSemaphoreTake(xCo2, portMAX_DELAY);
        // ---------------------------------------//
        data_2[0] = (uint8_t)((int) round(distance) & 0xFF); // octet de poids faible
        data_2[1] = (uint8_t)(((int) round(distance) >> 8) & 0xFF); // octet de poids fort

        printf("Data 0\n");
        affchage_binaire(data_2[0]);
        printf("Data 1\n");
        affchage_binaire(data_2[1]);
        printf("distance\n");
        affchage_binaire((int) distance);

        data_2[2] = co2_1;
        data_2[3] = co2_2;

        txBytes = uart_write_bytes(UART_NUM_2, data_2, len_2);
        
        // ---------------------------------------//
        xSemaphoreGive(xCo2);
        xSemaphoreGive(xDistance);
        // ---------------------------------------//
    }
}

// ───────────────────────────────────────────────────────────────────  //

/* ////////////////////////////////////////////////////////////////////////////////////////////// */
/*---------------------------------------END_UART_CONNECTION_ESP----------------------------------*/
/* ////////////////////////////////////////////////////////////////////////////////////////////// */

void app_main() {
    v_initDistance();
    v_initCo2();
    v_initUartEsp();
}

