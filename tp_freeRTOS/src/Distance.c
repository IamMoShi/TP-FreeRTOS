#include <stdio.h> 
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "ultrasonic.h"
#include "Distance.h"
#include "driver/gpio.h"
#include "esp_timer.h"

float distance;
ultrasonic_sensor_t sensor      = {
                                    .trigger_pin = TRIGGER_GPIO,
                                    .echo_pin = ECHO_GPIO
                                  };


void v_initDistance() {
    ultrasonic_init(&sensor);
    gpio_set_direction(DISTANCE_PIN_OUTPUT, GPIO_MODE_OUTPUT);
    gpio_set_level(DISTANCE_PIN_OUTPUT, 0);

    xTaskCreatePinnedToCore(
        v_distanceTask,
        "get_distance",
        configMINIMAL_STACK_SIZE * 3,
        NULL,
        5, 
        NULL,
        1
    );

    printf("Distance initialized \n");
}


void v_distanceTask() {
    uint64_t last_detection;
    bool distance_high;
    // ---------------------------------------//
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(DISTANCE_PERIOD);
    xLastWakeTime = xTaskGetTickCount();
    // ---------------------------------------//
    distance = 0;
    distance_high = false;
    last_detection = 0;
    while (true) {
        // ---------------------------------------//
        vTaskDelayUntil( &xLastWakeTime, xFrequency);
        // ---------------------------------------//

        ultrasonic_measure(&sensor, MAX_DISTANCE_CM, &distance);
        distance = distance * 100;  // Transforme la distance en CM
        
        if (distance < TRIGGER_DISTANCE) {
            last_detection = esp_timer_get_time();
            if (! distance_high) {
                distance_high = true;
                gpio_set_level(DISTANCE_PIN_OUTPUT, 1);
                printf("HIGH\n");
            }
        } else {
            if (last_detection != 0 && esp_timer_get_time() - last_detection > WAKEUP_DELAY_SENSIBILITY_MS * 1000) {
                last_detection = 0;
                gpio_set_level(DISTANCE_PIN_OUTPUT, 0);
                distance_high = false;
                printf("LOW\n");
            }
        }

    }
}