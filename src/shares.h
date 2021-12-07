/** @file shares.h
 *  This file contains extern declarations of shares and queues which are used
 *  in more than one file of a demonstration project.
 * 
 *  @author JR Ridgely
 *  @date   2021-Oct-23 Original file
 *  @copyright (c) 2021 by JR Ridgely, released under the LGPL 3.0. 
 */

#ifndef _SHARES_H_
#define _SHARES_H_

#include <Arduino.h>
#include <STM32FreeRTOS.h> 
#include <PrintStream.h>
#include <taskqueue.h>
#include <taskshare.h>

// Define shares - a share holds a global variable of various system measurements and the pump speed
extern Share<float> bath_temp_s; /// A float share storing the current ice bath temperature
extern Share<float> sleeve_temp_s; /// A float share storing the current sleeve temperature
extern Share<float> sleeve_pressure_s; /// A float share storing the current sleeve pressure
extern Share<float> pump_speed_s; /// A float share storing the desired pump speed

#endif // _SHARES_H_