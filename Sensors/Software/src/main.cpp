#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

#define ARDUINO_LOOP_STACK_SIZE 8192

#include <Arduino.h>

#include "stateMachine.h"
#include "States/setup.h"

#include <exception>

// #define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
// #include "esp_log.h"

// #include <i2cpyro.h"

stateMachine statemachine;

TaskHandle_t loopTaskHandle = NULL;

void setup_task()
{
        statemachine.initialise(new Setup(&statemachine));
}

void inner_loop_task()
{
    statemachine.update();
}

void loopTask(void *pvParameters)
{
    setup_task();
    for(;;) {
        inner_loop_task();
        vTaskDelay(1);
 
    }
}

extern "C" void app_main()
{
    initArduino(); //probably dont even need this
    xTaskCreateUniversal(loopTask, "loopTask", ARDUINO_LOOP_STACK_SIZE, NULL, 1, &loopTaskHandle, 1);
}
