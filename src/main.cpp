/***
 * @brief main file for the SmartIce machine
 * @details This machine sets up and runs the all
 * of the functions and controllers for the machine.
 * The machine is designed for athletes to recover
 * after workouts. The machine can increase pressure,
 * and alert the athlete when it is time to add more
 * ice. \n \n
 * This is a proof of concept, and can be upgraded to
 * use a thermoelectric cooler, proportional valve, and
 * more for a premium and profitable machine.
***/

#include <Arduino.h>           // Our coding Bible
#include <motor_class.h>       // Our pump driver
#include <Adafruit_LPS35HW.h>  // For the I2C Temperature and pressure probe
#include <DallasTemperature.h> // For the ADC Temperature Probe
#include <OneWire.h>           // For ADC sensors
#include <STM32FreeRTOS.h>     // Header for RTOS system
#include <task_list.h>         // Works with RTOS system
#include "taskshare.h"         // Header for inter-task shared data
#include "taskqueue.h"         // Header for inter-task data queues
#include "shares.h"            // Header for shares used in this project
#include <PrintStream.h>

// define global shares
Share<float> bath_temp_s("bath temp"); /// Share for most recent bath temperature
Share<float> sleeve_temp_s("sleeve temp"); /// Share for most recent sleeve temperature
Share<float> sleeve_pressure_s("sleeve pressure"); /// Share for most recent sleeve pressure
Share<float> pump_speed("pump speed setting"); /// Share for most recently requested pump speed

/** @brief   Arduino setup function which runs once at program startup.
 *  @details This function sets up a serial port for communication and creates
 *           the tasks which will be run. This is using the RTOS system.
 */
void setup () 
{
  // Define serial start up
  Serial.begin(115200);
  Serial.println("Sucessful serial connection to the Nucleo");
  delay(2000);

    // Create a task which prints a slightly disagreeable message
    xTaskCreate (task_bathsensor,
                 "Bath Temp",                     // Task name for printouts
                 4086,                            // Stack size
                 NULL,                            // Parameters for task fn.
                 4,                               // Priority
                 NULL);                           // Task handle

    // Create a task which prints a more agreeable message
    xTaskCreate (task_sleevesensor,
                 "Pressure Read",
                 4086,                            // Stack size
                 NULL,
                 3,                               // High priority
                 NULL);

    // Create a task which prints a more agreeable message
    xTaskCreate (task_overallcontroller,
                 "Controller Algorithm",
                 4086,                            // Stack size
                 NULL,
                 2,                               // Priority
                 NULL);
    xTaskCreate (task_pumpcontroller,
                 "Pump Control",
                 4086,                            // Stack size
                 NULL,
                 1,                               // Priority
                 NULL);

    vTaskStartScheduler ();
}

/**
 * @brief Loop function is unused in RTOS code.
 */
void loop() {
// nothing here for RTOS code!
}