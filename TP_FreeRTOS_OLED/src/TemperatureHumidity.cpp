#include<Arduino.h>
#include"DHTesp.h"
#include"TemperatureHumidity.h"
#include"ProducteurConsommateur.h"


DHTesp dht;
// ───────────────────────────────────────────────────────────────────  //

void v_initTemp() {
    dht.setup(DHT_PIN, DHTesp::DHT22);  // Initialize temperature sensor
    
    // Start task to get temperature
	xTaskCreatePinnedToCore(
        v_HumidityTemperatureTask,      /* Function to implement the task */
        "tempTask ",                    /* Name of the task */
        4000,                           /* Stack size in words */
        NULL,                           /* Task input parameter */
        5,                              /* Priority of the task */
        NULL,                           /* Task handle. */
        1);                             /* Core where the task should run */

    Serial.println("Temperature and humidity initialized");    
}

// ───────────────────────────────────────────────────────────────────  //

void v_HumidityTemperatureTask(void *pvParameters) {
  // --------------------------------------------------------------------------------------------//
    TickType_t xLastWakeTime;                                   // Seting up the periodicity of the task
    const TickType_t xFrequency = pdMS_TO_TICKS(TEMPERATURE_HUMIDITY_PERIOD);    // Period of the task
    xLastWakeTime = xTaskGetTickCount();                        // The counter
  // --------------------------------------------------------------------------------------------//

  while (true) {
    // --------------------------------------------------------------------------------------------//
    xTaskDelayUntil( &xLastWakeTime, xFrequency);
    // --------------------------------------------------------------------------------------------//
    
    TempAndHumidity newValues = dht.getTempAndHumidity();
    if (dht.getStatus() == 0) {
        v_addToBuffer((uint16_t) newValues.temperature, Temperature);
        v_addToBuffer((uint16_t) newValues.humidity, Humidity);
    }

  }
}