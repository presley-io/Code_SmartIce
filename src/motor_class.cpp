/**
 * @brief Source code for motor control library
 * @details This library code controls a pin
 * assigned to a motor. The parameters for the
 * function are very straightforward, and the filtering
 * process allows ease of implementation into a controller
 * system.
**/

#include <Arduino.h>
#include <motor_class.h>

/**
 * @brief Setup function for motor driver class
 * @details This function intializes the class and also
 * assigns the pin related to the pump object.
 * @param p_pin the pin associated with the pump object.
 */
pumpDriver::pumpDriver(uint32_t p_pin) {
    pin_assigned = p_pin;
}

/**
 * @brief Functions sets specific duty to a specific pin
 * @details The function simply transports the desired
 * PWM duty to the desired pin. There is some filtering that
 * occurs on the desired duty cycle. This function is useful
 * because we can take the floating point values from our 
 * controller algorithm, and send them straight into the
 * motor function.
 * @param p_dutycycle Duty Cycle desired
**/
void pumpDriver::set_Duty(float p_dutycycle) {
    // initialize the filtered duty cycle
    uint32_t clean_dutycycle; /// pwm that is postive and allowable

    // filter and save the duty cycle
    if (p_dutycycle > 100) {
        clean_dutycycle = 255;
    } 
    else if (p_dutycycle < 0) {
        clean_dutycycle = 0;
    }
    else {
        clean_dutycycle = uint32_t(p_dutycycle*255/100);
    }
    analogWrite(pin_assigned, clean_dutycycle);
}

/**
 * @brief Sets a maximum pump load.
 * @details Adjusts the maximum allowable pump speed. This
 * is useful for pwm applications as the controller can often
 * output speeds that are unnecessary or impossible to hit.
 * The value must be larger than the min_pwm_setting but not larger
 * than 100.
 * @param p_maxsetting max pwm setting allowed for pump
 */
void pumpDriver::set_max(float p_maxsetting) {
    if (p_maxsetting > 100) {
        Serial.println("impossible value of max pwm; set it to 100");
        max_pwm_setting = 100;
    } else if (p_maxsetting < min_pwm_setting) {
        Serial.println("max pwm must be larger than min pwm; set it to 100");
        max_pwm_setting = 100;
    } else {
        max_pwm_setting = p_maxsetting;
    }
}

/**
 * @brief Sets a minimum pump load.
 * @details Adjusts the minimum allowable pump speed. This
 * is useful for pwm applications as the controller can often
 * output speeds that are negative. The 
 * @param p_maxsetting min pwm setting allowed for pump
 */
void pumpDriver::set_min(float p_minsetting) {
    if (p_minsetting < 0) {
        Serial.println("impossible value of min pwm; set it to 0");
        min_pwm_setting = 0;
    } else if (p_minsetting > max_pwm_setting) {
        Serial.println("min pwm must be smaller than max pwm; set it to 0");
        min_pwm_setting = 0;
    } else {
        min_pwm_setting = p_minsetting;
    }
}