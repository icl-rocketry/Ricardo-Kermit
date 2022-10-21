#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

#define ARDUINO_LOOP_STACK_SIZE 8192

#include <Arduino.h>
#include <exception>
#include "stateMachine.h"
#include "States/idle.h"
#include "ADS1219/ADS1219.h"
#include <global_config.h>
#include "NTCThermistor/NTCthermistor.h"
#include "LoadCell/LoadCell.h"

stateMachine statemachine;

void setup_task()
{
    statemachine.initialise(new Idle(&statemachine));
}

void inner_loop_task()
{
    statemachine.update();
}

void loopTask(void *pvParameters)
{
    // esp_log_level_set("*", ESP_LOG_INFO);
    setup_task();
    for (;;)
    {
         inner_loop_task();
         vTaskDelay(1);
        // Serial.println((statemachine.ADS0.readAdjusted(10))-4282700);
        // delay(200);
        // Serial.println("T0");
        // Serial.println(statemachine.ntctemp0.getTempLinear());
        // delay(100);
        // Serial.println("T1");
        // Serial.println(statemachine.ntctemp1.getTempLinear());
        // delay(100);
        //Serial.println("mass");


        // Serial.println(statemachine.loadcell0.getMass());
        // delay(500);
        // delay(500);
        // Serial.println("ntc0");
        // Serial.println(statemachine.ntctemp0.getTempLinear());
        // delay(500);
        // Serial.println("ntc1");
        // Serial.println(statemachine.ntctemp1.getTempLinear());
        // delay(500);
        //Serial.println("ADCR");
        // Serial.println(statemachine.ADS0.readAdjusted(10));
        // delay(250);
        //Serial.println(statemachine.ntctemp0.getTempLinear());
        // delay(500);
        //Serial.println(statemachine.ptap0.getPressure());
        //Serial.println(statemachine.ADS2.readAdjusted(1));
        // Serial.println(statemachine.ADS0.readAdjusted(10));
        // delay(100);
        // Serial.println(statemachine.ADS2.readAdjusted(0));
        // delay(100);
        // Serial.println("Tap0");
        // Serial.println(statemachine.ptap0.getPressure());
        // Serial.println(statemachine.ntctemp1.getTempLinear());
        // delay(100);
        // Serial.println(statemachine.ntctemp0.getTempLinear());
        // delay(100);
        //Serial.println(statemachine.ntctemp1.getTempLinear());
        //delay(100);
        // // Serial.println(statemachine.loadcell0.getWeight());
        // // delay(100);
        // // delay(100);
        // Serial.println(statemachine.ptap1.getPressure());
        // Serial.println(statemachine.loadcell0.getWeight());
        // delay(100);
        
        //Serial.println(statemachine.ADS0.readAdjusted(10));
        //delay(100);

        // delay(100);


     }
}

TaskHandle_t loopTaskHandle = NULL;

extern "C" void app_main()
{
    initArduino(); // probably dont even need this
    xTaskCreateUniversal(loopTask, "loopTask", ARDUINO_LOOP_STACK_SIZE, NULL, 1, &loopTaskHandle, 1);
}
