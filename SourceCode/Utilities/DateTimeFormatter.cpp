#include "DateTimeFormatter.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <unordered_map>

DateTimeFormatter::DateTimeFormatter()
{
}

DateTimeFormatter::~DateTimeFormatter()
{
}

std::string DateTimeFormatter::GetCurrentDateTimeString(TimeMask::DetailLevel detailLevel)
{
    std::time_t now = std::time(nullptr);
    std::tm     tmNow;
    localtime_s(&tmNow, &now);

    static const std::unordered_map<TimeMask::DetailLevel, const char*> formatMap = {
        {TimeMask::Year, "%Y"},
        {TimeMask::Month, "%m"},
        {TimeMask::Day, "%d"},
        {TimeMask::Hour, "%H"},
        {TimeMask::Minute, "%M"},
        {TimeMask::Second, "%S"},
        {TimeMask::YMD, "%Y-%m-%d"},
        {TimeMask::YMDH, "%Y-%m-%d %H"},
        {TimeMask::YMDHM, "%Y-%m-%d %H:%M"},
        {TimeMask::YMDHMS, "%Y-%m-%d %H:%M:%S"}};

    auto it = formatMap.find(detailLevel);
    if (it != formatMap.end())
    {
        std::ostringstream oss;
        oss << std::put_time(&tmNow, it->second);
        return oss.str();
    }

    return "";
}

std::string DateTimeFormatter::ParsePartDateTimeFromString(const std::string& dataTimeStr, TimeMask::DetailLevel detailLevel)
{
    static const std::unordered_map<TimeMask::DetailLevel, std::pair<size_t, size_t>> formatMap = {
        {TimeMask::Year, {0, 4}},
        {TimeMask::Month, {5, 2}},
        {TimeMask::Day, {8, 2}},
        {TimeMask::Hour, {11, 2}},
        {TimeMask::Minute, {14, 2}},
        {TimeMask::Second, {17, 2}},
        {TimeMask::YMD, {0, 10}},
        {TimeMask::YMDH, {0, 13}},
        {TimeMask::YMDHM, {0, 16}},
        {TimeMask::YMDHMS, {0, 19}}};

    auto finder = formatMap.find(detailLevel);
    if (finder != formatMap.end())
    {
        size_t start  = finder->second.first;
        size_t length = finder->second.second;
        if (dataTimeStr.size() >= start + length)
        {
            return dataTimeStr.substr(start, length);
        }
    }

    return "";
}

time_t DateTimeFormatter::ConvertDateTimeFromString(const std::string& dataTimeStr)
{
    char* charTime = (char*)dataTimeStr.data();
    tm    tm_;
    int   year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, msec = 0;
    int   parsed = sscanf_s(charTime, "%d-%d-%d %d:%d:%d.%d", &year, &month, &day, &hour, &minute, &second, &msec);
    tm_.tm_year  = year - 1900;
    tm_.tm_mon   = month - 1;
    tm_.tm_mday  = day;
    tm_.tm_hour  = hour;
    tm_.tm_min   = minute;
    tm_.tm_sec   = second;
    tm_.tm_isdst = 0;
    time_t t_    = mktime(&tm_) * 1000 + msec;
    return t_;
}
