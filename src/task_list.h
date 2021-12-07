/**
 * @file task_list.h
 * @brief Header file containing all tasks.
 * @details Sets up tasks for RTOS uses.
 */

#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <PrintStream.h>

void task_bathsensor(void* p_params);
void task_sleevesensor(void* p_params);
void task_overallcontroller(void* p_params);
void task_pumpcontroller(void* p_params);
