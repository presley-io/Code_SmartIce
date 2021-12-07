/***
 * @brief Header code for motor control library
 * @details This library code controls a pin
 * assigned to a motor. The parameters for the
 * function are very straightforward. 
***/

#include <Arduino.h>

/**
 * @brief Pump driver for DC motorized pumps
 * @details This driver uses pwm functionality and logic
 * to control pumps and set limits to their uses. The pump
 * object is assigned to a pin, and its settings can be modified
 * and stored within the object.
 */
class pumpDriver
{
protected:
    
public:
    // class constructor initializes the pump driver
    pumpDriver(uint32_t p_pin);

    // set the pump speed -- its a pwm duty.
    void set_Duty(float P_dutycycle);

    // set min and max speeds
    void set_max(float p_maxsetting);
    void set_min(float p_minsetting);
    
    // define public variables
    float max_pwm_setting = 100; /// maximum allowable pwm duty for pump
    float min_pwm_setting = 0; /// minimum allowable pwm duty for pump
    uint32_t pin_assigned; /// assigned pin for pump (must have PWM capability)
};