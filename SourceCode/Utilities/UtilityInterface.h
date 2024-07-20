#pragma once
#include "Utility/IUtilityInterface.h"
#include "IRevelationInterface.h"

class DateTimeFormatter;
class SnowflakeGenerator;
class TaskCreator;

class UtilityInterface : public IUtilityInterface
{
  public:
    UtilityInterface(IRevelationInterface* intf);
    ~UtilityInterface();

    virtual void Initialize() override;
    virtual void Uninitialize() override;

    virtual IDateTimeFormatter*  GetDateTimeFormatter() override;
    virtual ISnowflakeGenerator* GetSnowflakeGenerator() override;
    virtual ITaskCreator*        GetTaskCreator() override;

  private:
    DateTimeFormatter*  m_dateTimeFormatter  = nullptr;
    SnowflakeGenerator* m_snowFlakeGenerator = nullptr;
    TaskCreator*        m_taskCreator        = nullptr;
};
