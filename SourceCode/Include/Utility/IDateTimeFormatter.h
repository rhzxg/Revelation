#pragma once
#include <string>

struct TimeMask
{
    enum DetailLevel
    {
        Year       = 1,
        Month      = 1 << 1,
        Day        = 1 << 2,
        Hour       = 1 << 3,
        Minute     = 1 << 4,
        Second     = 1 << 5,
        MiliSecond = 1 << 6,

        YMD     = Year | Month | Day,
        YMDH    = YMD | Hour,
        YMDHM   = YMDH | Minute,
        YMDHMS  = YMDHM | Second,
        YMDHMSM = YMDHMS | MiliSecond,
    };
};

class IDateTimeFormatter
{
  public:
    virtual std::string GetCurrentDateTimeString(TimeMask::DetailLevel detailLevel)                                    = 0;
    virtual std::string ParsePartDateTimeFromString(const std::string& dataTimeStr, TimeMask::DetailLevel detailLevel) = 0;
    virtual time_t      ConvertDateTimeFromString(const std::string& dataTimeStr)                                      = 0;
};