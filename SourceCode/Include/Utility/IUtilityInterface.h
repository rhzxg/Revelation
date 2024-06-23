#pragma once
#include "IExtensionInterface.h"
#include "IDateTimeFormatter.h"

class IUtilityInterface : public IExtensionInterface
{
  public:
    virtual IDateTimeFormatter* GetDateTimeFormatter() = 0;
};