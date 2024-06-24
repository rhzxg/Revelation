#pragma once
#include "Utility/IUtilityInterface.h"
#include "IRevelationInterface.h"

class DateTimeFormatter;
class SnowFlakeGenerator;

class UtilityInterface : public IUtilityInterface
{
  public:
    UtilityInterface(IRevelationInterface* intf);
    ~UtilityInterface();

    virtual void Initialize() override;
    virtual void Uninitialize() override;

    virtual IDateTimeFormatter* GetDateTimeFormatter() override;
    virtual ISnowFlakeGenerator* GetSnowFlakeGenerator() override;

  private:
    DateTimeFormatter*  m_dateTimeFormatter  = nullptr;
    SnowFlakeGenerator* m_snowFlakeGenerator = nullptr;
};
