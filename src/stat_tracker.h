/**
 * @file stat_tracker.h
 *  This file contains the headers for the class that
 *  is given measurements and computes some statistical 
 *  parameters from those measurements.
 * 
 * @author Ben Presley
 * @date 2021-Nov-06
 */

#include <Arduino.h>

/**
 * @brief
 * @details
 */
class StatTracker
{
protected:
    // Number of the data points in StatTracker
    uint32_t data_points;

    // Sum of the data points in StatTracker
    float data_sum;

    // Sum of Squares of the data points in StatTracker
    float data_sos;

public:
    // Constructor initializes variables for zero data
    StatTracker(void);

    // Add one data (override functions)
    void add_data(float new_pt);
    void add_data(int32_t new_pt);
    void add_data(uint32_t new_pt);

    // Returns the number of data points in StatTracker
    uint32_t num_points(void);

    // Returns the average of the data points
    float average(void);

    // Returns the standard deviation of the data
    float std_dev(void);

    // Deletes all the data, returning to no points added state
    void clear(void);
};