/**
 * @file stat_tracker.cpp
 *  This file contains the source code for the class that
 *  is given measurements and computes some statistical 
 *  parameters from those measurements.
 * 
 * @author Ben Presley
 * @date 2021-Nov-06
 */


#include <Arduino.h>
#include <stat_tracker.h>

/** 
 * @brief Create a StatTracker Object
 * @details This constructor does nothing but exist to
 *          to create a StatTracking object and its internal
 *          protected variables.
 */
StatTracker::StatTracker (void) {
    // Number of the data points in StatTracker
    uint32_t data_points = 0;

    // Sum of the data points in StatTracker
    float data_sum = 0.0;

    // Sum of Squares of the data points in StatTracker
    float data_sos = 0.0;
}

/**
 * @brief Add a point to the StatTracker
 * @details This function is in a set of override functions
 * that adds data of different types to the StatTracker object.
 * This specific override function adds a float to the set of data.
 * 
 * @param new_pt A floating data point to be added.
 */
void StatTracker::add_data(float new_pt) {
    data_points++; // add one to the number of data points
    data_sum += new_pt; // add data point value to sum of data points
    data_sos += new_pt*new_pt; // add square of data point to sos of data points
}

/**
 * @brief Add a point to the StatTracker
 * @details This function is in a set of override functions
 * that adds data of different types to the StatTracker object.
 * This specific override function adds a 32-bit signed integer 
 * to the set of data.
 * 
 * @param new_pt A 32-bit signed integer data point to be added.
 */
void StatTracker::add_data(int32_t new_pt) {
    new_pt = float(new_pt); // convert the input parameter to float
    data_points++; // add one to the number of data points
    data_sum += new_pt; // add data point value to sum of data points
    data_sos += new_pt*new_pt; // add square of data point to sos of data points
}

/**
 * @brief Add a point to the StatTracker
 * @details This function is in a set of override functions
 * that adds data of different types to the StatTracker object.
 * This specific override function adds a 32-bit unsigned integer 
 * to the set of data.
 * 
 * @param new_pt A 32-bit unsigned integer data point to be added.
 */
void StatTracker::add_data(uint32_t new_pt) {
    new_pt = float(new_pt); // convert the input parameter to float
    data_points++; // add one to the number of data points
    data_sum += new_pt; // add data point value to sum of data points
    data_sos += new_pt*new_pt; // add square of data point to sos of data points
}

/**
 * @brief returns the number of points in the data set
 * @details This returns the number of the points in the data
 *          set that has been stored and tracked internally.
 */
uint32_t StatTracker::num_points(void) {
    return data_points;
}

/**
 * @brief returns the average of points in the data set
 * @details This returns the average of the points in the data
 *          set that has been calculated internally everytime
 *          a data point is added to the StatTracking object.
 */
float StatTracker::average(void) {
    return data_sum/data_points;
}

/**
 * @brief returns the average of standard deviation in the data set
 * @details This returns the standard deviation of the points in the data
 *          set that has been calculated internally everytime
 *          a data point is added to the StatTracking object.
 */
float StatTracker::std_dev(void) {
    return sqrt(data_sos/data_points - pow((data_sum/data_points),2));
}

/**
 * @brief Resets the data set to zero
 * @details Sets all of the internal class
 *          variables back to zero (their starting
 *          conditions).
 */
void StatTracker::clear(void) {
    uint32_t data_points = 0;
    float data_sum = 0.0;
    float data_sos = 0.0;
}