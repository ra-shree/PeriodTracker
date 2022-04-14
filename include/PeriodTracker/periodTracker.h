#include <pch.h>

class tracker {
private:
    std::vector<int> predicted_menstrual_cycle_length;
    std::vector<int> actual_menstrual_cycle_length;
    // consecutive length difference
    std::vector<int> CLD;
    time_t last_system_date;
    time_t today_system_date;
    float standard_deviation;
    std::vector<float> mean_of_actual_cycles;
    float latest_predicted_length;

public:

    ~tracker() {};

    // returns the latest predicted length
    // takes no argument
    int Latest_Predicted_Length();


    // returns the latest actual menstrual length
    // takes no arguments
    int Latest_Actual_Length();

    // sets the latest actual menstrual length
    // takes no arguments
    // calculate and insert CLD into the vector

    // calculate the quartiles of actual_menstrual_cycle_length
    int Calculate_Quartiles();

    // return the last system date

    // return the standard deviation of last predicted cycle length and last actual menstrual cycle length

    // insert new predicted date into the vector

    // insert new actual date into the vector

    // check if a file exists and return a boolean

    // create a new file if not and return a boolean

    // retrieve date from the file and instantiate the object

    /* Deepti's Job */

    // return today's system date as a string
    // no input
    // friend std::string Today_System_Date(tracker& object);

    // return an integer that is the difference between two dates
    // input two date arguments

    // algorithm to predict the next menstrual cycle length
    // take tracker object as the argument
    friend int Algorithm_Predict_Next_Menstrual_Length(tracker& object);
};

float Mean_Actual(tracker& object);

int Algorithm_Predict_Next_Menstrual_Length(tracker& object);