// #include <stdio.h> 
// #include <freertos/FreeRTOS.h>
// #include <freertos/task.h>
// #include "sdkconfig.h"
// #include "driver/gpio.h"
// #include "sdkconfig.h"
// #include "freertos/semphr.h"
// #include <stdbool.h>
// #include <stdlib.h>

// #include "ultrasonic.h"
// #include "driver/uart.h"

// /*---------------------------------------DISTANCE_SENSOR------------------------------------------*/
// /*
// #define MAX_DISTANCE_CM 500 // 500 cm max
// #define WAKEUP_DISTANCE_CM 10 // Distance de détection de présence
// #define WAKEUP_DELAY_SENSIBILITY_MS 5000 // Temps entre la fin de la détection de présence et le signal d'extinction de la présence
// #define TRIGGER_GPIO 33
// #define ECHO_GPIO 32


// void vget_distance() {
//     ultrasonic_sensor_t sensor = {
//         .trigger_pin = TRIGGER_GPIO,
//         .echo_pin = ECHO_GPIO
//     };

//     ultrasonic_init(&sensor);

//     // ---------------------------------------//
//     TickType_t xLastWakeTime;
//     const TickType_t xFrequency = 25;
//     xLastWakeTime = xTaskGetTickCount();
//     // ---------------------------------------//

//     while (true) {
//         // ---------------------------------------//
//         vTaskDelayUntil( &xLastWakeTime, xFrequency );
//         // ---------------------------------------//

//         float distance;
//         esp_err_t res = ultrasonic_measure(&sensor, MAX_DISTANCE_CM, &distance);


//         if (res != ESP_OK) {
//             printf("Error %d: ", res);
//             switch (res)
//             {
//                 case ESP_ERR_ULTRASONIC_PING:
//                     printf("Cannot ping (device is in invalid state)\n");
//                     break;
//                 case ESP_ERR_ULTRASONIC_PING_TIMEOUT:
//                     printf("Ping timeout (no device found)\n");
//                     break;
//                 case ESP_ERR_ULTRASONIC_ECHO_TIMEOUT:
//                     printf("Echo timeout (i.e. distance too big)\n");
//                     break;
//                 default:
//                     printf("%s\n", esp_err_to_name(res));
//             }
//         }
//         else {
//             if (distance * 100 < WAKEUP_DISTANCE_CM) {
//                 printf("Distance: %0.04f cm\n", distance*100);
//             }       
//         }
//     }
// }
// */
// /*---------------------------------------DISTANCE_SENSOR------------------------------------------*/

// /*-------------------------------------------CO_SENSOR--------------------------------------------*/



// int RX_BUF_SIZE = 1024;
// int TXD_PIN = 17;
// int RXD_PIN = 16;
// int rxBytes; int txBytes;
// int data[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
// int len = 9;
// uint8_t DataRX[9];

// /*-------------------------------------------CO_SENSOR--------------------------------------------*/


// void app_main() {
//     //xTaskCreate(vget_distance, "get_distance", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);


//     const uart_config_t uart_config = {
//         .baud_rate = 9600,
//         .data_bits = UART_DATA_8_BITS,
//         .parity = UART_PARITY_DISABLE,
//         .stop_bits = UART_STOP_BITS_1,
//         .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
//         .source_clk = UART_SCLK_APB,
//     };
    
//     uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
//     uart_param_config(UART_NUM_1, &uart_config);
//     uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

//     printf("Byte0 :%x, Byte1: %x, Byte2: %x, Byte3 : %x, Byte4 : %x, Byte5 : %x, Byte6 : %x, Byte7 : %x, Byte8 : %x\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8]);

//     while (1) {
//         txBytes = uart_write_bytes(UART_NUM_1, data, len);
//         printf("txBytes = %d\n", txBytes);
//         //uint8_t* datar = (uint8_t*) malloc(9); 
//         rxBytes = uart_read_bytes(UART_NUM_1, DataRX, len, 1000 / portTICK_PERIOD_MS);
//         printf("Nombre de Bytes lus : %d\n", rxBytes);
//     }

// }

