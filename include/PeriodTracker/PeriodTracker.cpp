#include <pch.h>

class tracker {
private:
    std::vector<int> predicted_menstrual_cycle_length;
    std::vector<int> actual_menstrual_cycle_length;
    time_t last_system_date;
    float standard_deviation;
    std::vector<float> mean_of_actual_cycles;
    float latest_predicted_length;

public:
    // returns the latest predicted length
    // takes latest_predicted_length as argument
    int latest_predicted_length()
    {
        return latest_predicted_length;
    }

    // returns the last predicted length
    // takes menstrual_length_pairs as argument

    // return the last system date

    // return the mean of the actual_dates

    // return the standard deviation of last predicted cycle length and last actual menstrual cycle length

    // insert new predicted date into the vector

    // insert new actual date into the vector

    // check if a file exists and return a boolean

    // create a new file if not and return a boolean

    // retrieve date from the file and instantiate the object
};