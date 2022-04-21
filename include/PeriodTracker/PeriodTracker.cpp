#include "pch.h"
#include "PeriodTracker.hpp"
#include <vector>
#include <algorithm>

// constructor
tracker::tracker()
{
    tracker::sorted_menstrual_cycle_length;
    tracker::actual_menstrual_cycle_length;
    tracker::actual_menstrual_vector_size = 0;
    tracker::latest_quartiles[0] = { 0 };
    tracker::latest_quartiles[1] = { 0 };
    tracker::latest_quartiles[2] = { 0 };
    tracker::CLD;
    tracker::CLD_size = 0;
    tracker::latest_predicted_length = 0;
    tracker::latest_actual_length = 0;
    tracker::countdown_predicted_date = 0;
    tracker::predicted_length_date.tm_sec = 0;
}

// Clear all data and start a new user session
void tracker::Clear_Data()
{
    tracker::actual_menstrual_cycle_length.clear();
    tracker::actual_menstrual_vector_size = 0;
    tracker::CLD.clear();
    tracker::CLD_size = 0;
    tracker::latest_predicted_length = 0;
    tracker::countdown_predicted_date = 0;    
}

// calculate today's system date and set it as the last app open date
int tracker::Calc_Todays_Date(struct tm& date)
{
    time_t now = time(0);
    struct tm current_time = *localtime(&now);
    // we are only concerned with the day month and year so others are set to 0
    current_time.tm_hour = 0;
    current_time.tm_min = 0;
    current_time.tm_sec = 0;
    date = current_time;
    return 0;
}

// calculate the days between today and some give time structure (date)
// return the result as an integer
int Days_Between_Two_Dates(struct tm& older_date)
{
    older_date.tm_hour = 0;
    older_date.tm_min = 0;
    older_date.tm_sec = 0;

    time_t latest_time = time(0);
    struct tm local_latest_time = *localtime(&latest_time);
    local_latest_time.tm_hour = 0;
    local_latest_time.tm_min = 0;
    local_latest_time.tm_sec = 0;

    // gets the difference between the two times in seconds
    double seconds = difftime(mktime(&local_latest_time), mktime(&older_date));

    // convert the seconds to days
    int days = seconds / 86400;
    return days;
}

// calculates the days between when the app was last opened and today and substracts from the countdown
int tracker::Days_Between_App_Open()
{
    int days_between_app_open = Days_Between_Two_Dates(predicted_length_date);
    countdown_predicted_date -= days_between_app_open;
    return 0;
}

// Set the predicted_length_date data member value
int tracker::Set_Predicted_Length_Date()
{
    Calc_Todays_Date(predicted_length_date);
    return 0;
}

// calculate and insert CLD into the vector
// CLD = consecutive menstrual cycle length difference i.e previous menstrual cycle length - this cycle length and this cycle length - next cycle length and so on
int tracker::Calculate_CLD()
{
    if (actual_menstrual_cycle_length.size() >= 2) {
        int consecutive_length_difference = actual_menstrual_cycle_length[actual_menstrual_vector_size - 1] - actual_menstrual_cycle_length[actual_menstrual_vector_size - 2];
        CLD.push_back(consecutive_length_difference);
    }
    CLD_size = CLD.size();
    return 0;
}

// Set the value of latest_actual_length when user presses the button and al
int tracker::Set_Latest_Actual_Length()
{
    int latest_actual_length1;
    latest_actual_length1 = Days_Between_Two_Dates(predicted_length_date);
    actual_menstrual_cycle_length.push_back(latest_actual_length1);
    return 0;
}

// calculate the quartiles of actual_menstrual_cycle_length
int tracker::Calculate_Quartiles()
{
    int total_elements = actual_menstrual_vector_size;
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
            Q_index[i] = sorted_menstrual_cycle_length[abc[i] - 1];
        }
        else {
            Q_index[i] = sorted_menstrual_cycle_length[abc[i] - 1] + frac_part * (sorted_menstrual_cycle_length[abc[i]] - sorted_menstrual_cycle_length[abc[i] - 1]);
        }
    }
    latest_quartiles[0] = Q_index[0];
    latest_quartiles[1] = Q_index[1];
    latest_quartiles[2] = Q_index[2];
    return 0;
}

// sort sorted_menstrual_cycle_length in ascending order
int tracker::Sorted_Vectors()
{
    sorted_menstrual_cycle_length = actual_menstrual_cycle_length;
    if (actual_menstrual_vector_size > 2) {
        std::sort(sorted_menstrual_cycle_length.begin(), sorted_menstrual_cycle_length.end());
    }
    return 0;
}

// Algorithm to calculate the next menstrual cycle length
int tracker::Algorithm_Predict_Next_Menstrual_Length()
{
    int next_predicted_length = 28;
    actual_menstrual_vector_size = actual_menstrual_cycle_length.size();
    int max_index_of_vector = actual_menstrual_vector_size;

    // if there is no previous predicted length i.e this is the first use of the app
    if (max_index_of_vector <= 0) {
        next_predicted_length = 28;
    }

    // if there is one data stored so next predicted length is same as last length of menstrual cycle
     else if (max_index_of_vector == 1) {
        next_predicted_length = actual_menstrual_cycle_length[0];
    }

    // if there is two data stored next predicted length is average of both lengths
    else if (max_index_of_vector == 2) {
        next_predicted_length = (actual_menstrual_cycle_length[0] + actual_menstrual_cycle_length[1]) / 2;
        Calculate_CLD();
    }

    else if (max_index_of_vector >= 3) {
        Sorted_Vectors();
        Calculate_Quartiles();
        Calculate_CLD();
        // difference_in_CLD = CLD[n-1] - CLD[n] where n is the last element in CLD vector
        int last_calculated_CLD = CLD[CLD_size - 1];
        if (last_calculated_CLD > -5 && last_calculated_CLD < 5) {
            next_predicted_length = latest_quartiles[1];
        }
        else if (last_calculated_CLD >= 5) {
            next_predicted_length = latest_quartiles[2];
        }
        else if (last_calculated_CLD <= 5) {
            next_predicted_length = latest_quartiles[0];
        }
    }
    countdown_predicted_date = next_predicted_length;
    latest_predicted_length = next_predicted_length;
    return 0;
}

// Load data from the file into the object
/*
    latest_predicted_length
    actual_menstrual_vector_size, CLD_size,
    sorted_menstrual_cycle_length (vector),
    actual_menstrual_cycle_length (vector),
    CLD,
    predicted_length_date(tm structure) --> year, month, day
*/
int tracker::Load_Data_From_File()
{
    // size of actual_menstrual_cycle_length
    int n;

    // temporary variable to hold data that's going to be loaded to object
    int data;

    std::ifstream loader;
    loader.open("data.txt", std::ios::in);

    loader >> latest_predicted_length;
    loader >> countdown_predicted_date;
    loader >> n;
    actual_menstrual_vector_size = n;

    for (int i = 0; i < n; i++) {
        loader >> data;
        actual_menstrual_cycle_length.push_back(data);
    }

    loader >> n;
    CLD_size = n;
    for (int i = 0; i < n; i++) {
        loader >> data;
        CLD.push_back(data);
    }
    // load last_app_open_date with date
    loader >> predicted_length_date.tm_year;
    loader >> predicted_length_date.tm_mon;
    loader >> predicted_length_date.tm_mday;

    loader.close();
    return 0;
}

// Unload data to the file from the object
/*
    latest_predicted_length
    actual_menstrual_vector_size, CLD_size,
    sorted_menstrual_cycle_length (vector),
    actual_menstrual_cycle_length (vector),
    CLD,
    predicted_length_date(tm structure) --> year, month, day
*/
int tracker::Unload_Data_To_File()
{
    std::ofstream unloader("data.txt", std::ios::out | std::ios::trunc);
    unloader << latest_predicted_length << " ";
    unloader << countdown_predicted_date << " ";

    unloader << actual_menstrual_vector_size << " ";
    for (int item : actual_menstrual_cycle_length) {
        unloader << item << " ";
    }

    unloader << CLD_size << " ";
    for (int item : CLD) {
        unloader << item << " ";
    }

    // load last_app_open_date with date
    unloader << predicted_length_date.tm_year << " ";
    unloader << predicted_length_date.tm_mon << " ";
    unloader << predicted_length_date.tm_mday << " ";

    unloader.close();
    return 0;
}

// When user presses the button and a new menstrual date needs to be calculated
void tracker::Runner()
{
    Algorithm_Predict_Next_Menstrual_Length();
    // Set the date when the length was last predicted
    Set_Predicted_Length_Date();
}

void tracker::Delete_File()
{
    Clear_Data();
    Unload_Data_To_File();
}

// show the computer how to initialize the object
// never called. no other use
void TemporaryFunction()
{
    tracker object;
}
