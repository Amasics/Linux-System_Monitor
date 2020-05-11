#include <string>

#include "format.h"

using std::string;

string Format::numberToString(int num) {
    string res = std::to_string(num);
    if (res.length() == 1) {
        res = "0" + res;
    }
    return res;
}

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int hours;
    int minutes;
    // convert seconds to Hour:Minutes:Second
    minutes = seconds / 60;
    seconds = seconds % 60;
    hours = minutes / 60;
    minutes = minutes % 60;

    return Format::numberToString(hours) + ":" + 
        Format::numberToString(minutes) + ":" +
        Format::numberToString(seconds);
}