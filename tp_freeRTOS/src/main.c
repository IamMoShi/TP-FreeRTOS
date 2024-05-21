#include <stdio.h> 
#include <freertos/FreeRTOS.h>
#include "ProducteurConsommateur.h"
#include "CO2.h"
#include "Distance.h"
#include "ESP.h"

void app_main() {
    v_initProducteurConsommateur();
    v_initDistance();
    v_initCO2();
    v_initESP();
}

