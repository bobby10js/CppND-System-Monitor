#include <string>
#include "format.h"
#include <iomanip>

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:seconds
// REMOVE: [[maybe_unused]] once you define the function
std ::string Format::ElapsedTime(long totalSecond) {
    std::time_t seconds(totalSecond); // you have to convert your input_seconds into time_t
    tm *p = gmtime(&seconds); // convert to broken down time
    std::stringstream str;
    if(p->tm_yday != 0)
        str << p->tm_yday << " days ";
    str  << std::setw(2) << std::setfill('0') << p->tm_hour << ":" << std::setw(2) << std::setfill('0') << p->tm_min << ":"<< std::setw(2) << std::setfill('0') <<  p->tm_sec;
    return str.str();
}