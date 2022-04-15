#include <pch.h>

class tracker {
private:
    std::vector<int> predicted_menstrual_cycle_length;
    std::vector<int> actual_menstrual_cycle_length;
    int actual_menstrual_vector_size;
    int predicted_menstrual_vector_size;
    // last calculated quartiles of actual_menstrual_cycle_length
    int latest_quartiles[3];

    // CLD = consecutive menstrual cycle length difference i.e previous menstrual cycle length - this cycle length and this cycle length - next cycle length and so on
    std::vector<int> CLD;

    time_t last_app_open_date;
    time_t start_of_next_cycle_date;

    int latest_predicted_length;
    int latest_actual_length;

public:
    tracker() 
    {
        this->actual_menstrual_vector_size = actual_menstrual_cycle_length.size();
        this->predicted_menstrual_vector_size = predicted_menstrual_cycle_length.size();
    };

    ~tracker() {};

    // returns the latest predicted length
    // takes no argument
    int Latest_Predicted_Length()
    {
        return this->latest_predicted_length;
    }

    // returns the latest actual menstrual length
    // takes no arguments
    int Latest_Actual_Length()
    {
        return this->latest_actual_length;
    }

    // calculate and insert CLD into the vector
    // CLD = consecutive menstrual cycle length difference i.e previous menstrual cycle length - this cycle length and this cycle length - next cycle length and so on
    int Calculate_CLD()
    {
        if (actual_menstrual_cycle_length.size() >= 2)
        {
            int consecutive_length_difference = actual_menstrual_cycle_length[this->actual_menstrual_vector_size - 1] - actual_menstrual_cycle_length[this->actual_menstrual_vector_size - 2];
            CLD.push_back(consecutive_length_difference);
        }
        return 0;
    }

    // calculate the quartiles of actual_menstrual_cycle_length
    int Calculate_Quartiles()
    {
        std::sort(actual_menstrual_cycle_length.begin(), actual_menstrual_cycle_length.end());
        int total_elements = this->actual_menstrual_vector_size;
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

    // insert latest_predicted_length and latest_actual_length into their respective vectors once the user presses the button
    int Insert_Latest_Lengths_In_Vectors()
    {
        this->predicted_menstrual_cycle_length.push_back(this->latest_predicted_length);
        this->actual_menstrual_cycle_length.push_back(this->latest_actual_length);
        return 0;
    }
    
    // check if a file exists and return a boolean

    // create a new file if not and return a boolean

    // retrieve date from the file and instantiate the object

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
    int next_predicted_length;
    int CLD_size = ob->CLD.size();
    int difference_in_CLD = ob->CLD[CLD_size - 1] - ob->CLD[CLD_size - 2];
    int max_index_of_vector = ob->predicted_menstrual_vector_size - 1;

    // if there is no previous predicted length i.e this is the first use of the app
    if (max_index_of_vector == -1) {
        next_predicted_length = 28;
        return 0;
    }

}