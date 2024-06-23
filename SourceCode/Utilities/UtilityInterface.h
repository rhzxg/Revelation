#pragma once
#include "IUtilityInterface.h"
#include "IRevelationInterface.h"

class UtilityInterface : public IUtilityInterface
{
  public:
    UtilityInterface(IRevelationInterface* intf);
    ~UtilityInterface();

    virtual void Initialize() override;
    virtual void Uninitialize() override;
};
