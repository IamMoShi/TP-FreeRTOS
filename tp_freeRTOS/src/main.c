#include <stdio.h> 
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/semphr.h"
#include <stdbool.h>
#include <stdlib.h>

#include "ultrasonic.h"


/*---------------------------------------DISTANCE_SENSOR------------------------------------------*/
/*
#define MAX_DISTANCE_CM 500 // 500 cm max
#define WAKEUP_DISTANCE_CM 10 // Distance de détection de présence
#define WAKEUP_DELAY_SENSIBILITY_MS 5000 // Temps entre la fin de la détection de présence et le signal d'extinction de la présence
#define TRIGGER_GPIO 33
#define ECHO_GPIO 32


void vget_distance() {
    ultrasonic_sensor_t sensor = {
        .trigger_pin = TRIGGER_GPIO,
        .echo_pin = ECHO_GPIO
    };

    ultrasonic_init(&sensor);

    // ---------------------------------------//
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 25;
    xLastWakeTime = xTaskGetTickCount();
    // ---------------------------------------//

    while (true) {
        // ---------------------------------------//
        vTaskDelayUntil( &xLastWakeTime, xFrequency );
        // ---------------------------------------//

        float distance;
        esp_err_t res = ultrasonic_measure(&sensor, MAX_DISTANCE_CM, &distance);


        if (res != ESP_OK) {
            printf("Error %d: ", res);
            switch (res)
            {
                case ESP_ERR_ULTRASONIC_PING:
                    printf("Cannot ping (device is in invalid state)\n");
                    break;
                case ESP_ERR_ULTRASONIC_PING_TIMEOUT:
                    printf("Ping timeout (no device found)\n");
                    break;
                case ESP_ERR_ULTRASONIC_ECHO_TIMEOUT:
                    printf("Echo timeout (i.e. distance too big)\n");
                    break;
                default:
                    printf("%s\n", esp_err_to_name(res));
            }
        }
        else {
            if (distance * 100 < WAKEUP_DISTANCE_CM) {
                printf("Distance: %0.04f cm\n", distance*100);
            }       
        }
    }
}
*/
/*---------------------------------------DISTANCE_SENSOR------------------------------------------*/


void app_main() {
    printf("Hello world \n");
    //xTaskCreate(vget_distance, "get_distance", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
}

