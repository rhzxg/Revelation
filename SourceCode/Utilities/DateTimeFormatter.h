#pragma once
#include "Utility/IDateTimeFormatter.h"

class DateTimeFormatter : public IDateTimeFormatter
{
  public:
    DateTimeFormatter();
    ~DateTimeFormatter();

    virtual std::string GetCurrentDateString(TimeMask::DetailLevel detailLevel);
};

