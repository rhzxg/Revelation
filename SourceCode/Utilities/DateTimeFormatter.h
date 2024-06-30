#pragma once
#include "Utility/IDateTimeFormatter.h"

class DateTimeFormatter : public IDateTimeFormatter
{
  public:
    DateTimeFormatter();
    ~DateTimeFormatter();

    virtual std::string GetCurrentDateTimeString(TimeMask::DetailLevel detailLevel);
    virtual std::string GetPartDataTimeFromString(const std::string& dataTimeStr, TimeMask::DetailLevel detailLevel);
};

