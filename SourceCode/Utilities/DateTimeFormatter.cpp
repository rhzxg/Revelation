#include "DateTimeFormatter.h"
#include <ctime>
#include <iomanip>
#include <sstream>

DateTimeFormatter::DateTimeFormatter()
{
}

DateTimeFormatter::~DateTimeFormatter()
{
}

std::string DateTimeFormatter::GetCurrentDateString(TimeMask::DetailLevel detailLevel)
{
    std::time_t now   = std::time(nullptr);
    std::tm*    tmNow = std::localtime(&now); // todo fix

    if (detailLevel == TimeMask::Year)
    {
        return std::to_string(tmNow->tm_year);
    }
    else if (detailLevel == TimeMask::Month)
    {
        return std::to_string(tmNow->tm_mon + 1);
    }
    else if (detailLevel == TimeMask::Day)
    {
        return std::to_string(tmNow->tm_mday);
    }
    else if (detailLevel == TimeMask::Hour)
    {
        return std::to_string(tmNow->tm_hour);
    }
    else if (detailLevel == TimeMask::Minute)
    {
        return std::to_string(tmNow->tm_min);
    }
    else if (detailLevel == TimeMask::Second)
    {
        return std::to_string(tmNow->tm_sec);
    }
    // else if (detailLevel == TimeMask::MiliSecond)
    //{
    //     return std::to_string(tmNow->tm);
    // }
    else if (detailLevel == TimeMask::YMD)
    {
        // format: yyyy-mm-dd
        std::ostringstream oss;
        oss << std::put_time(tmNow, "%Y-%m-%d");
        return oss.str();
    }
    else if (detailLevel == TimeMask::YMDH)
    {
        // format: yyyy-mm-dd hh
        std::ostringstream oss;
        oss << std::put_time(tmNow, "%Y-%m-%d %H");
        return oss.str();
    }
    else if (detailLevel == TimeMask::YMDHM)
    {
        // format: yyyy-mm-dd hh::mm
        std::ostringstream oss;
        oss << std::put_time(tmNow, "%Y-%m-%d %H:%M");
        return oss.str();
    }
    else if (detailLevel == TimeMask::YMDHMS)
    {
        // format: yyyy-mm-dd hh::mm::ss
        std::ostringstream oss;
        oss << std::put_time(tmNow, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
    // else if (detailLevel == TimeMask::YMDHMSM)
    //{
    // }
    return "";
}
