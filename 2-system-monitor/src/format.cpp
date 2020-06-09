#include <string>
#include <sstream>

#include "format.h"

using std::string;

std::string Format::ConvertNumberToString(const long number) {
    std::stringstream ss;

    if (number < 10) {
        ss << "0";
    }
    ss << number;

    return ss.str();
}

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    const int SECONDS_PER_HOUR = 60*60;
    const int SECONDS_PER_MINUTE = 60;

    long hours = seconds/(SECONDS_PER_HOUR);
    long remainingSeconds = seconds%(SECONDS_PER_HOUR);
    
    long minutes = remainingSeconds/SECONDS_PER_MINUTE;
    remainingSeconds = seconds%SECONDS_PER_MINUTE;

    std::stringstream ss;
    ss << ConvertNumberToString(hours) << ":" <<
        ConvertNumberToString(minutes) << ":" <<
        ConvertNumberToString(remainingSeconds);

    return ss.str(); 
}