#include <pch.h>

class tracker {
private:
    std::vector<int> predicted_menstrual_cycle_length;
    std::vector<int> actual_menstrual_cycle_length;
    // last calculated quartiles of actual_menstrual_cycle_length
    int latest_quartiles[3];

    // consecutive length difference
    std::vector<int> CLD;
    time_t last_system_date;
    time_t today_system_date;
    int latest_predicted_length;
    int latest_actual_length;

public:
    tracker() {};

    ~tracker() {};

    // returns the latest predicted length
    // takes no argument
    int Latest_Predicted_Length()
    {
        return latest_predicted_length;
    }

    // returns the latest actual menstrual length
    // takes no arguments
    int Latest_Actual_Length()
    {
        return latest_actual_length;
    }

    // sets the latest actual menstrual length
    // takes no arguments
    // calculate and insert CLD into the vector

    // calculate the quartiles of actual_menstrual_cycle_length
    int Calculate_Quartiles()
    {
        std::sort(actual_menstrual_cycle_length.begin(), actual_menstrual_cycle_length.end());
        int total_elements = actual_menstrual_cycle_length.size();
        float Q_index[3];
        float quart = (total_elements + 1) / 4;
        int abc[3] = { 0 };
        Q_index[0] = quart;
        Q_index[1] = quart * 2;
        Q_index[2] = quart * 3;

        abc[0] = Q_index[0]; // Q1 index without the decimal
        abc[1] = Q_index[1]; // Q2 index without the decimal
        abc[2] = Q_index[2]; // Q3 index without the decimal

        for (int i = 0; i < 3; i++) {
            // check the fractional part of the index to use a different formula if it's anything besides zero
            float frac_part = Q_index[i] - abc[i];
            if (frac_part == 0) {
                Q_index[i] = actual_menstrual_cycle_length[abc[i] - 1];
            }
            else {
                Q_index[i] = actual_menstrual_cycle_length[abc[i] - 1] + frac_part * (actual_menstrual_cycle_length[abc[i]] - actual_menstrual_cycle_length[abc[i] - 1]);
            }
        }
        return 0;
    }
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

int Algorithm_Predict_Next_Menstrual_Length(tracker& object)
{
    tracker* ob = &object;
    int next_predicted_length = 0;
    next_predicted_length = ob->latest_predicted_length;
    int max_index_of_vector = ob->predicted_menstrual_cycle_length.size() - 1;

    // if there is no previous predicted length i.e this is the first use of the app
    if (max_index_of_vector == -1) {
        next_predicted_length = 28;
        ob->latest_predicted_length = next_predicted_length;
        ob->predicted_menstrual_cycle_length.push_back(next_predicted_length);
        return 0;
    }

}