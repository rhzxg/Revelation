#pragma once
#include "Utility/IDateTimeFormatter.h"

class DateTimeFormatter : public IDateTimeFormatter
{
  public:
    DateTimeFormatter();
    ~DateTimeFormatter();

    virtual std::string GetCurrentDateTimeString(TimeMask::DetailLevel detailLevel);
    virtual std::string ParsePartDateTimeFromString(const std::string& dataTimeStr, TimeMask::DetailLevel detailLevel);
    virtual time_t      ConvertDateTimeFromString(const std::string& dataTimeStr) override;
};

