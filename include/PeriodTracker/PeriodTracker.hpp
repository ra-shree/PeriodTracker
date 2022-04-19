#include "pch.h"
#include <vector>
#include <algorithm>
#ifndef _PERIODTRACKER_H_
#define _PERIODTRACKER_H_

class tracker {
private:
    std::vector<int> sorted_menstrual_cycle_length;
    std::vector<int> actual_menstrual_cycle_length;
    int actual_menstrual_vector_size;
    // last calculated quartiles of actual_menstrual_cycle_length
    int latest_quartiles[3];

    // CLD = consecutive menstrual cycle length difference i.e previous menstrual cycle length - this cycle length and this cycle length - next cycle length and so on
    std::vector<int> CLD;
    int CLD_size;
    int latest_predicted_length;
    int latest_actual_length;

public:
    // constructor
    tracker();

    tracker(int n);

    // starts equal to latest_predicted_length but counts down with each day until zero then negative until next predicted date is calculated
    int countdown_predicted_date;
    // stores the date when the app was last opened
    struct tm last_app_open_date;

    // stores the date when the app last predicted the date of menstrual cycle
    struct tm predicted_length_date;

    // calculate today's system date and set it as the last app open date
    int Calc_Todays_Date(struct tm& date);

    // calculate the days between today and some give time structure (date)
    // return the result as an integer
    friend int Days_Between_Two_Dates(struct tm& older_date);

    // calculates the days between when the app was last opened and today and substracts from the countdown
    int Days_Between_App_Open();

    // Set the predicted_length_date data member value
    int Set_Predicted_Length_Date();

    // calculate and insert CLD into the vector
    // CLD = consecutive menstrual cycle length difference i.e previous menstrual cycle length - this cycle length and this cycle length - next cycle length and so on
    int Calculate_CLD();

    // Set the value of latest_actual_length when user presses the button
    int Set_Latest_Actual_Length();
    
    // function to recalculate the vector lengths when necessary
    int Calculate_Vector_Lengths();

    // calculate the quartiles of actual_menstrual_cycle_length
    int Calculate_Quartiles();

    // sort sorted_menstrual_cycle_length in ascending order
    friend int Sort_Vectors(tracker& object);

    // Calculate the date when the app is opened
    int Last_App_Open_Date();

    // runs the entire program from start to finish
    friend void Runner(tracker& object);

    // algorithm to predict the next menstrual cycle length
    // take tracker object as the argument
    friend int Algorithm_Predict_Next_Menstrual_Length(tracker& object);

    // Load data from the file into the object
    friend int Load_Data_From_File(tracker& object);

    // Unload data to the file from the object
    friend int Unload_Data_To_File(tracker& object);

    // Run this function if the user presses the delete button
    // Deletes any existing data and creates a new file and also runs the algorithm
    friend void Delete_File(tracker& object);
};

#endif

