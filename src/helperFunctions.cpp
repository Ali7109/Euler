#include "helperFunctions.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>

std::string getCurrentDateTimePrefix()
{
    // Get current time
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);

    // Convert to tm structure
    std::tm tm;
#if defined(_MSC_VER) // Microsoft Visual C++
    localtime_s(&tm, &now_c);
#else // POSIX compliant systems
    localtime_r(&now_c, &tm);
#endif

    // Format the current time
    std::stringstream timeStream;
    timeStream << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    return timeStream.str() + " :: ";
}

std::string getOutputLog(std::string identifer, std::string value)
{
    return getCurrentDateTimePrefix() + identifer + " " + value;
}