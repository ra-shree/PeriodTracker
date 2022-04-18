#include "pch.h"
#include "PeriodTracker.hpp"
#include <vector>
#include <algorithm>

// constructor
tracker::tracker()
{
    tracker::sorted_menstrual_cycle_length;
    tracker::actual_menstrual_cycle_length;
    tracker::actual_menstrual_vector_size = actual_menstrual_cycle_length.size();
    tracker::latest_quartiles[0] = { 0 };
    tracker::latest_quartiles[1] = { 0 };
    tracker::latest_quartiles[2] = { 0 };
    tracker::CLD;
    tracker::CLD_size = 0;
    tracker::latest_predicted_length = 0;
    tracker::latest_actual_length = 0;
    tracker::countdown_predicted_date = 0;
    tracker::last_app_open_date.tm_sec = 0;
    tracker::predicted_length_date.tm_sec = 0;
}
tracker::tracker(int n)
{
    tracker::sorted_menstrual_cycle_length;
    tracker::actual_menstrual_cycle_length;
    tracker::actual_menstrual_vector_size = actual_menstrual_cycle_length.size();
    tracker::latest_quartiles[0] = { 0 };
    tracker::latest_quartiles[1] = { 0 };
    tracker::latest_quartiles[2] = { 0 };
    tracker::CLD;
    tracker::CLD_size = 0;
    tracker::latest_predicted_length = 0;
    tracker::latest_actual_length = 0;
    tracker::countdown_predicted_date = 0;
    tracker::last_app_open_date.tm_sec = 0;
    tracker::predicted_length_date.tm_sec = 0;
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
    int days_between_app_open = Days_Between_Two_Dates(last_app_open_date);
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
    return 0;
}

// Set the value of latest_actual_length when user presses the button
int tracker::Set_Latest_Actual_Length()
{
    latest_actual_length = Days_Between_Two_Dates(predicted_length_date);
    sorted_menstrual_cycle_length.push_back(latest_actual_length);
    actual_menstrual_cycle_length.push_back(latest_actual_length);
    return 0;
}

// Calculate the date when the app is opened
int tracker::Last_App_Open_Date()
{
    Calc_Todays_Date(last_app_open_date);
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
    return 0;
}

// sort sorted_menstrual_cycle_length in ascending order
int Sort_Vectors(tracker& object)
{
    tracker* ob = &object;
    std::sort(ob->sorted_menstrual_cycle_length.begin(), ob->sorted_menstrual_cycle_length.end());
    return 0;
}

// Algorithm to calculate the next menstrual cycle length
int Algorithm_Predict_Next_Menstrual_Length(tracker& object)
{
    tracker* ob = &object;
    int next_predicted_length = 28;
    int max_index_of_vector = ob->actual_menstrual_vector_size - 1;

    // if there is no previous predicted length i.e this is the first use of the app
    if (max_index_of_vector <= -1) {
        next_predicted_length = 28;
    }

    // if there is one data stored so next predicted length is same as last length of menstrual cycle
    if (max_index_of_vector == 0) {
        next_predicted_length = ob->actual_menstrual_cycle_length[0];
    }

    // if there is two data stored next predicted length is average of both lengths
    if (max_index_of_vector == 1) {
        next_predicted_length = (ob->actual_menstrual_cycle_length[0] + ob->actual_menstrual_cycle_length[1]) / 2;
        ob->Calculate_CLD();
    }

    if (max_index_of_vector >= 2) {
        Sort_Vectors(*ob);
        ob->Calculate_Quartiles();
        ob->Calculate_CLD();
        // difference_in_CLD = CLD[n-1] - CLD[n] where n is the last element in CLD vector
        int CLD_size = ob->CLD.size();
        int last_calculated_CLD = ob->CLD[CLD_size - 1];
        if (last_calculated_CLD > -5 && last_calculated_CLD < 5) {
            next_predicted_length = ob->latest_quartiles[1];
        }
        else if (last_calculated_CLD >= 5) {
            next_predicted_length = ob->latest_quartiles[2];
        }
        else if (last_calculated_CLD <= 5) {
            next_predicted_length = ob->latest_quartiles[0];
        }
    }
    ob->countdown_predicted_date = next_predicted_length;
    ob->latest_predicted_length = next_predicted_length;
    return 0;
}

// check for existing file, return 0 if exists else return 1
bool Check_File_Exists()
{
    std::string name = "data.txt";
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

// Load data from the file into the object
/*
    countdown_predicted_date, latest_predicted_length, latest_actual_length,
    actual_menstrual_vector_size, CLD_size
    latest_quartiles(3),
    sorted_menstrual_cycle_length (vector),
    actual_menstrual_cycle_length (vector),

    last_app_open_date(tm structure) --> year, month, day
*/
int Load_Data_From_File(tracker& object)
{
    tracker* ob = &object;
    // size of actual_menstrual_cycle_length and sorted_menstrual_cycle_length
    int n, o;
    // temporary variable to hold data that's going to be loaded to object
    int data;
    std::ifstream loader;
    loader.open("data.txt", std::ios::in);
    loader >> ob->countdown_predicted_date >> ob->latest_predicted_length >> ob->latest_actual_length;
    loader >> n;
    ob->actual_menstrual_vector_size = n;
    loader >> o;
    ob->CLD_size = o;
    for (int i = 0; i < 3; i++) {
        loader >> data;
        ob->latest_quartiles[i] = data;
    }
    for (int i = 0; i < n; i++) {
        loader >> data;
        ob->sorted_menstrual_cycle_length.push_back(data);
    }
    for (int i = 0; i < n; i++) {
        loader >> data;
        ob->actual_menstrual_cycle_length.push_back(data);
    }
    for (int i = 0; i < o; i++) {
        loader >> data;
        ob->CLD.push_back(data);
    }
    // load last_app_open_date with date
    loader >> ob->predicted_length_date.tm_year;
    loader >> ob->predicted_length_date.tm_mon;
    loader >> ob->predicted_length_date.tm_mday;

    loader.close();
    return 0;
}

// Unload data to the file from the object
/*
    countdown_predicted_date, latest_predicted_length, latest_actual_length,
    actual_menstrual_vector_size, CLD_size,
    latest_quartiles(3),
    sorted_menstrual_cycle_length (vector),
    actual_menstrual_cycle_length (vector),
    CLD,
    last_app_open_date(tm structure) --> year, month, day
*/
int Unload_Data_To_File(tracker& object)
{
    tracker* ob = &object;
    std::ofstream unloader("data.txt", std::ios::app);

    unloader << ob->countdown_predicted_date << " " << ob->latest_predicted_length << " " << ob->latest_actual_length << " ";
    unloader << ob->actual_menstrual_vector_size << " " << ob->CLD_size << " ";
    for (int i = 0; i < 3; i++) {
        unloader << ob->latest_quartiles[i] << " ";
    }
    for (int i = 0; i < ob->actual_menstrual_vector_size; i++) {
        unloader << ob->sorted_menstrual_cycle_length[i] << " ";
    }
    for (int i = 0; i < ob->actual_menstrual_vector_size; i++) {
        unloader << ob->actual_menstrual_cycle_length[i] << " ";
    }
    for (int i = 0; i < ob->CLD_size; i++) {
        unloader << ob->CLD[i] << " ";
    }
    // load last_app_open_date with date
    unloader << ob->predicted_length_date.tm_year << " ";
    unloader << ob->predicted_length_date.tm_mon << " ";
    unloader << ob->predicted_length_date.tm_mday << " ";

    unloader.close();
    return 0;
}

// When user presses the button and a new menstrual date needs to be calculated
void Runner(tracker& object)
{
    tracker* ob = &object;
    Algorithm_Predict_Next_Menstrual_Length(*ob);
    // Set the date when the length was last predicted
    ob->Set_Predicted_Length_Date();
    if (ob->latest_actual_length != 0)
    {
        ob->Set_Latest_Actual_Length();
        if (ob->actual_menstrual_vector_size > 1)
        {
            ob->Calculate_CLD();
            Sort_Vectors(*ob);
        }
    }
}

void Delete_File(tracker& object)
{
    remove("data.txt");
    object = tracker(0);
}

// show the computer how to initialize the object
// never called. no other use
void TemporaryFunction()
{
    tracker object;
}
