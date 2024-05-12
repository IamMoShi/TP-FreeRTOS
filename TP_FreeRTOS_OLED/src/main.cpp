#include<Arduino.h>
#include<stdio.h>
#include"driver/uart.h"
#include<Ticker.h>
#include"DHTesp.h"

/* //////////////////////////////////////////////////////////////////// */
/* ----------------------- TEMPERATURE-HUMIDITY ----------------------- */
/* //////////////////////////////////////////////////////////////////// */

// ───────────────────────────────────────────────────────────────────  //
void v_HumidityTemperatureTask(void *pvParameters);
bool b_getTemperature();

DHTesp dht;

TaskHandle_t tempTaskHandle = NULL; // Task handle for the light value read task
int dhtPin                  = 15;   // Pin number for DHT11 data pin

int temperature; 
int humidity;    

SemaphoreHandle_t xTemperature;
SemaphoreHandle_t xHumidity;

// ───────────────────────────────────────────────────────────────────  //

void v_initTemp() {
  byte resultValue = 0;
	dht.setup(dhtPin, DHTesp::DHT22);  // Initialize temperature sensor
	Serial.println("DHT initiated");   // Show that the temperature task was ask

  // ---------------------------------------//
  xTemperature = xSemaphoreCreateBinary(); 
  xHumidity    = xSemaphoreCreateBinary();

  xSemaphoreGive(xTemperature);
  xSemaphoreGive(xHumidity);
  // ---------------------------------------//

  // Start task to get temperature
	xTaskCreatePinnedToCore(
			v_HumidityTemperatureTask,      /* Function to implement the task */
			"tempTask ",                    /* Name of the task */
			4000,                           /* Stack size in words */
			NULL,                           /* Task input parameter */
			5,                              /* Priority of the task */
			&tempTaskHandle,                /* Task handle. */
			1);                             /* Core where the task should run */
}

// ───────────────────────────────────────────────────────────────────  //

void v_HumidityTemperatureTask(void *pvParameters) {
  // ---------------------------------------//
    TickType_t xLastWakeTime;               // Seting up the periodicity of the task
    const TickType_t xFrequency = 2000;     // Period of the task
    xLastWakeTime = xTaskGetTickCount();    // The counter
  // ---------------------------------------//

  while (true) {
    // ---------------------------------------//
    xTaskDelayUntil( &xLastWakeTime, xFrequency );
    xSemaphoreTake(xTemperature, portMAX_DELAY);
    xSemaphoreTake(xHumidity, portMAX_DELAY);
    // ---------------------------------------//
    
    b_getTemperature();

    // ---------------------------------------//
    xSemaphoreGive(xTemperature);
    xSemaphoreGive(xHumidity);
    // ---------------------------------------//

  }
}


bool b_getTemperature() {
  /*
   * To use this function you have to take the mutex(s) of humidity and temperature variables
   */ 

  TempAndHumidity newValues = dht.getTempAndHumidity();

	if (dht.getStatus() != 0) {
		//Serial.println("DHT22 error status: " + String(dht.getStatusString()));
		return false;
	}

  temperature = newValues.temperature;
  humidity    = newValues.humidity;
  return true;
}

// ───────────────────────────────────────────────────────────────────  //

/* //////////////////////////////////////////////////////////////////////// */
/* ----------------------- END-TEMPERATURE-HUMIDITY ----------------------- */
/* //////////////////////////////////////////////////////////////////////// */

/* ////////////////////////////////////////////////////////////////////// */
/* ----------------------- UART-COMMUNICATION-ESP ----------------------- */
/* ////////////////////////////////////////////////////////////////////// */

// ───────────────────────────────────────────────────────────────────  //
void v_uartTask(void *pvParameters);
void v_initUart();

TaskHandle_t uartTaskHandle = NULL;

int RS232_RX       = 27;
int RS232_TX       = 26;
int RX_BUF_SIZE    = 1024;

uint32_t LEN_RS232 = 4;
int txBytes;
int rxBytes;

uint8_t DataRX_RS232[4];
uint8_t data_RS232[4] = {0, 0, 0, 0};

uint16_t detection; 
int co2; 

SemaphoreHandle_t xDetection; 
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

void v_initUart() {
  uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0); 
  uart_param_config(UART_NUM_1, &uart_config);
  uart_set_pin(UART_NUM_1, RS232_RX, RS232_TX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

  // ---------------------------------------//
  xDetection = xSemaphoreCreateBinary();
  xCo2       = xSemaphoreCreateBinary();

  xSemaphoreGive(xDetection);
  xSemaphoreGive(xCo2);
  // ---------------------------------------//

  xTaskCreatePinnedToCore(
			v_uartTask,                     /* Function to implement the task */
			"uartTask ",                    /* Name of the task */
			4000,                           /* Stack size in words */
			NULL,                           /* Task input parameter */
			2,                              /* Priority of the task */
			&uartTaskHandle,                /* Task handle. */
			0);                             /* Core where the task should run */


  Serial.println("UART initiated");
}

// ───────────────────────────────────────────────────────────────────  //

void v_uartTask(void *pvParameters) {
  // ---------------------------------------//
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 20;
    xLastWakeTime = xTaskGetTickCount();
  // ---------------------------------------//

  while (true) {
    // ---------------------------------------//
    xTaskDelayUntil( &xLastWakeTime, xFrequency);
    xSemaphoreTake(xDetection, portMAX_DELAY);
    xSemaphoreTake(xCo2, portMAX_DELAY);
    // ---------------------------------------//
    rxBytes = uart_read_bytes(UART_NUM_1, DataRX_RS232, LEN_RS232, 1000 / portTICK_PERIOD_MS);
    if (rxBytes > 0) {
      co2 = DataRX_RS232[2]*256 + DataRX_RS232[3];
      detection = ((int)data_RS232[1] << 8) | data_RS232[0];
    }

    // ---------------------------------------//
    xSemaphoreGive(xDetection);
    xSemaphoreGive(xCo2);
    // ---------------------------------------//
  }
}

// ───────────────────────────────────────────────────────────────────  //

/* ////////////////////////////////////////////////////////////////////////// */
/* ----------------------- END-UART-COMMUNICATION-ESP ----------------------- */
/* ////////////////////////////////////////////////////////////////////////// */

/* ///////////////////////////////////////////////////////////// */
/* ----------------------- PRINTING-DATA ----------------------- */
/* ///////////////////////////////////////////////////////////// */

// ───────────────────────────────────────────────────────────────────  //

void v_initPrinting();
void v_printingTask(void *pvParameters);

// ───────────────────────────────────────────────────────────────────  //

void v_initPrinting() {
  // ---------------------------------------//
  xTemperature = xSemaphoreCreateBinary(); 
  xHumidity    = xSemaphoreCreateBinary();
  xDetection   = xSemaphoreCreateBinary();
  xCo2         = xSemaphoreCreateBinary();

  xSemaphoreGive(xTemperature);
  xSemaphoreGive(xHumidity);
  xSemaphoreGive(xDetection);
  xSemaphoreGive(xCo2);
  // ---------------------------------------//

  xTaskCreatePinnedToCore(
			v_printingTask,                 /* Function to implement the task */
			"printingTask",                 /* Name of the task */
			4000,                           /* Stack size in words */
			NULL,                           /* Task input parameter */
			2,                              /* Priority of the task */
			&uartTaskHandle,                /* Task handle. */
			0);                             /* Core where the task should run */


  Serial.println("Printing initiated");
}

// ───────────────────────────────────────────────────────────────────  //

void v_printingTask(void *pvParameters) {
  // ---------------------------------------//
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 5000;
    xLastWakeTime = xTaskGetTickCount();
  // ---------------------------------------//

  while (true) {
    // ---------------------------------------//
    xTaskDelayUntil( &xLastWakeTime, xFrequency );

    xSemaphoreTake(xTemperature, portMAX_DELAY);
    xSemaphoreTake(xHumidity, portMAX_DELAY);
    xSemaphoreTake(xDetection, portMAX_DELAY);
    xSemaphoreTake(xCo2, portMAX_DELAY);
    // ---------------------------------------//

    printf("CO2 : %d; detection : %d; temp : %d; hum : %d\n", co2, detection, temperature, humidity);
    
    // ---------------------------------------//
    xSemaphoreGive(xTemperature);
    xSemaphoreGive(xHumidity);
    xSemaphoreGive(xDetection);
    xSemaphoreGive(xCo2);
    // ---------------------------------------//
  }
}

// ───────────────────────────────────────────────────────────────────  //

/* ///////////////////////////////////////////////////////////////// */
/* ----------------------- END-PRINTING-DATA ----------------------- */
/* ///////////////////////////////////////////////////////////////// */


// ───────────────────────────────────────────────────────────────────  //
// ────────────────────────────SETUP──────────────────────────────────  //
// ───────────────────────────────────────────────────────────────────  //
void setup() {
  // --------------------------------------------
  Serial.begin(115200);
  Serial.println();
  // --------------------------------------------
  v_initUart();
  // --------------------------------------------
  v_initTemp();
  // --------------------------------------------
  v_initPrinting();
}
// ───────────────────────────────────────────────────────────────────  //
// ───────────────────────────────────────────────────────────────────  //
// ───────────────────────────────────────────────────────────────────  //

void loop() {

}