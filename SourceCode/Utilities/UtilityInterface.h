#pragma once
#include "Utility/IUtilityInterface.h"
#include "IRevelationInterface.h"

class DateTimeFormatter;

class UtilityInterface : public IUtilityInterface
{
  public:
    UtilityInterface(IRevelationInterface* intf);
    ~UtilityInterface();

    virtual void Initialize() override;
    virtual void Uninitialize() override;

    virtual IDateTimeFormatter* GetDateTimeFormatter() override;

    private:
    DateTimeFormatter* m_dateTimeFormatter = nullptr;
};
