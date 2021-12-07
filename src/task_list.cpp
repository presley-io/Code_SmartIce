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

/**
 * @brief Bath Temperature Sensor Task
 * @details This is the task for the bath temperature sensor.
 * State 0 is initialization, which occurs when the task is first
 * created. State 1 is what runs forever in the for(;;) loop.
 * Since there are only 2 states including initialization, the states
 * were not necessary to define. \n \n
 * 
 * This task uses the ADC temperature probe class constructed
 * by the manufacturer of this product.
 * 
 */
void task_bathsensor(void* p_params) {
    (void)p_params;
    // STATE 0
    // Define the Temperature Probe stuff
    #define ONE_WIRE_BUS A5
    OneWire oneWire(ONE_WIRE_BUS); 
    DallasTemperature T_probe(&oneWire);

    // Start up Temperature probe
    T_probe.begin(); 

    for(;;) {
        // STATE 1
        T_probe.requestTemperatures();
        bath_temp_s.put(T_probe.getTempCByIndex(0));
        vTaskDelay(500);
    }
}

/**
 * @brief Sleeve Sensor Task
 * @details This is the task for the sleeve temperature and pressure sensor.
 * State 0 is initialization, which occurs when the task is first
 * created. State 1 is what runs forever in the for(;;) loop.
 * Since there are only 2 states including initialization, the states
 * were not necessary to define. \n \n
 * 
 * This task uses the I2C temperature probe class constructed
 * by the manufacturer of this product.
 * 
 */
void task_sleevesensor(void* p_params) {
    (void)p_params;   
    //STATE 0
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
        //STATE 1
        // read temperature and pressure at sleeve
        //sleeve_pressure_s.put(lps35hw.readPressure());
        //sleeve_temp_s.put(lps35hw.readTemperature());
        sleeve_pressure_s.put(1010);
        sleeve_temp_s.put(20);
        vTaskDelay(500);
    }
}

/**
 * @brief Overal Controller Task
 * @details This is the task for the system controller. This controller
 * employs a PID controller, with only a P gain.
 * State 0 is initialization, which occurs when the task is first
 * created -- this defines the controller values as well as initializes the
 * pump objected, which yes belongs to this task.
 * State 1 is what runs forever in the for(;;) loop (besides the if statement
 * that detects a pop). State 2 occurs if a pop is detected.
 */
void task_overallcontroller(void* p_params) {
    (void)p_params;   

    //StATE 0
    // Record last time controller was run
    TickType_t xLastWakeTime = xTaskGetTickCount();

    // define controller variable
    float error = 0; /// [C], PID error -- difference between desired and measured.
    float temp_setting = 4; /// [C], desired temperature at arm
    float temp_maximum = 10; /// [C], Maximum temp of bath; indicates that water/ice needs to be replenished
    float controller_kp = 10; /// [%motor_PWM / C], proportional gain of the PID controller.
    float last_pressure = 0; /// [kPa], recording of last presure.
    float allow_pressure_drop_percentage = 0.25; /// [%], percentage pressure drop tolerance. If exceeded, will trigger shutdown.

    // Define the pump object
    pumpDriver pump1(D2);

    for(;;) {
        // StATE 1
        float bath_temp = bath_temp_s.get();
        float sleeve_temp = sleeve_temp_s.get();
        float sleeve_pressure = sleeve_pressure_s.get();
        error = sleeve_temp - temp_setting; // [C]
        float new_pump_speed = error*controller_kp; // [%motor]

        // Test for break in flow system
        if (sleeve_pressure < last_pressure*(1-allow_pressure_drop_percentage)) {
            //STATE 2 -- SHUTDOWN.
            pump1.set_Duty(0); // Turn off pump in case of pop.
            Serial.println("Pop detected! Shutting down");
            delay(100000000000000000000000000000);
        } else {
            pump1.set_Duty(new_pump_speed); // update pump speed.
            last_pressure = sleeve_pressure; // update the last pressure.
        }
        
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

/**
 * @brief Pump controller task (optional)
 * @details This was removed from the task because it felt unnecessary.
 * The pump can be controlled from the main controller in 1 line of code,
 * so coding this entire task felt unncessary.
 */
void task_pumpcontroller(void* p_params) {
    (void)p_params;   
    

    for(;;) {

        vTaskDelay(500);
    }
}
