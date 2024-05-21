#include<Arduino.h>
#include<stdio.h>
#include<Wire.h>
#include<SSD1306Wire.h>
#include"DisplayData.h"
#include"ProducteurConsommateur.h"


SSD1306Wire display(0x3c, SDA, SCL);
u_int16_t displayCO2, displayTemp, displayHum;
volatile bool displayEnabled = false;
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;



// ───────────────────────────────────────────────────────────────────  //

void IRAM_ATTR handleInterrupt() {
  // Lire l'état actuel de la broche
  int state = digitalRead(DISTANCE_PIN_INPUT);

  portENTER_CRITICAL_ISR(&mux);
  displayEnabled = (state == HIGH);
  portEXIT_CRITICAL_ISR(&mux);

}

void v_initDistanceInterrupt() {
  pinMode(DISTANCE_PIN_INPUT, INPUT);
  attachInterrupt(digitalPinToInterrupt(DISTANCE_PIN_INPUT), handleInterrupt, CHANGE);
}

// ───────────────────────────────────────────────────────────────────  //

void v_initDisplay() {
  delay(1000);                        // Pour être sur que les autres tâches sont bien lancées 
  Serial.begin(115200);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  displayTemp = 0;
  displayHum  = 0;
  displayCO2  = 0;

  v_initDistanceInterrupt();

  xTaskCreatePinnedToCore(
			v_displayTask,                  /* Function to implement the task */
			"printingTask",                 /* Name of the task */
			4000,                           /* Stack size in words */
			NULL,                           /* Task input parameter */
			2,                              /* Priority of the task */
			NULL,                           /* Task handle. */
			0);                             /* Core where the task should run */


  Serial.println("Display initiated");
}

// ───────────────────────────────────────────────────────────────────  //

void v_displayTask(void *pvParameters) {
    DataType newProduct;
    volatile bool currentDisplayEnabled;
  // ---------------------------------------//
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(DISTANCE_PERIOD);
    xLastWakeTime = xTaskGetTickCount();
  // ---------------------------------------//

  while (true) {
    // ---------------------------------------//
    xTaskDelayUntil( &xLastWakeTime, xFrequency);
    // ---------------------------------------//
    
    newProduct = DT_removeFromBuffer();

    switch (newProduct.type) {
    case Temperature:
        printf("Modification de la Temperature : %d\n", newProduct.value);
        displayTemp = newProduct.value;
        break;
    case Humidity:
        printf("Modification de l'humidité : %d\n", newProduct.value);
        displayHum = newProduct.value;
        break;
    case CO2:
        printf("Modification du CO2 : %d\n", newProduct.value);
        displayCO2 = newProduct.value;
        break;
    case ERROR:
      break;
    default:
        break;
    }

    portENTER_CRITICAL(&mux);
    currentDisplayEnabled = displayEnabled;
    portEXIT_CRITICAL(&mux);

    if (currentDisplayEnabled) {
      display.clear();  
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      display.drawString(0, 0, "CO2: " + String(displayCO2));
      display.drawString(0, 12, "Temperature: " + String(displayTemp) + "°C");
      display.drawString(0, 24, "Humidity: " + String(displayHum) + "%");
      display.display();  
    } else {
      display.clear();
      display.display();
    }

  }
}