#pragma once
#include "IExtensionInterface.h"
#include "IDateTimeFormatter.h"
#include "ISnowFlakeGenerator.h"
#include "ITaskCreator.h"

class IUtilityInterface : public IExtensionInterface
{
  public:
    virtual IDateTimeFormatter*  GetDateTimeFormatter()  = 0;
    virtual ISnowflakeGenerator* GetSnowflakeGenerator() = 0;
    virtual ITaskCreator*        GetTaskCreator()        = 0;
};