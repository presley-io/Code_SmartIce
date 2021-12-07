/**
 * @file task_list.cpp
 * @brief Source code containing all tasks definitions
 * @details Sets up tasks for RTOS and defines their
 * specfic properties.
 */

#include <Arduino.h>
#include <motor_class.h>
#include <Adafruit_LPS35HW.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <STM32FreeRTOS.h>
#include <task_list.h>
#include <shares.h>

void task_bathsensor(void* p_params) {
    (void)p_params;
    // Define the Temperature Probe stuff
    #define ONE_WIRE_BUS A5
    OneWire oneWire(ONE_WIRE_BUS); 
    DallasTemperature T_probe(&oneWire);

    // Start up Temperature probe
    T_probe.begin(); 

    for(;;) {
        T_probe.requestTemperatures();
        bath_temp_s.put(T_probe.getTempCByIndex(0));
        vTaskDelay(500);
    }
}

void task_sleevesensor(void* p_params) {
    (void)p_params;   
    // Define the Adafruit LPS Object
    Adafruit_LPS35HW lps35hw = Adafruit_LPS35HW();

    // Establish connection with the LPS35 sensor
    Serial.println("Checking connection with Adafruit LPS35 Sensor");
    if (!lps35hw.begin_I2C()) {
        Serial.println("Couldn't find LPS35HW chip!");
    }
    //lps35hw.setDataRate(LPS35HW_RATE_75_HZ);
    //Serial.println("Found LPS35HW chip");

    for(;;) {
        // read temperature and pressure at sleeve
        //sleeve_pressure_s.put(lps35hw.readPressure());
        //sleeve_temp_s.put(lps35hw.readTemperature());
        sleeve_pressure_s.put(1010);
        sleeve_temp_s.put(20);
        vTaskDelay(500);
    }
}

void task_overallcontroller(void* p_params) {
    (void)p_params;   
    // Record last time controller was run
    TickType_t xLastWakeTime = xTaskGetTickCount();

    // define controller variable
    float error = 0; //[C]
    float error_sum = 0; // [C*seconds]
    float temp_setting = 4; // [C]
    float temp_maximum = 10; // [C] indicates that water/ice needs to be replenished
    float controller_kp = 10; // [%motor / C]

    // Define the pump object
    pumpDriver pump1(D2);

    for(;;) {
        float bath_temp = bath_temp_s.get();
        float sleeve_temp = sleeve_temp_s.get();
        float sleeve_pressure = sleeve_pressure_s.get();
        error = sleeve_temp - temp_setting; // [C]
        float new_pump_speed = error*controller_kp; // [%motor]

        pump1.set_Duty(new_pump_speed);

        // tell the world what is happening
        Serial.print("Bath temp: "); Serial.print(bath_temp); Serial.print("Sleeve temp: "); Serial.print(sleeve_temp); Serial.print("Pressure: "); Serial.println(sleeve_pressure); 

        // bath temperature too low warning
        if (bath_temp > temp_maximum) {
            Serial.println("Refill ice! I repeat -- refill ice! Bath is too warm!");
            for(int i = 0; i < 2; i++) {
            pump1.set_Duty(0);
            delay(200);
            pump1.set_Duty(new_pump_speed);
            delay(200);
            }
        }
        vTaskDelayUntil(&xLastWakeTime, 500);
    }
}

void task_pumpcontroller(void* p_params) {
    (void)p_params;   
    

    for(;;) {

        vTaskDelay(500);
    }
}
