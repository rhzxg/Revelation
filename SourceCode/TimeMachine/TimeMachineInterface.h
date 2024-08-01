#pragma once
#include "TimeMachine/ITimeMacineInterface.h"
#include "IRevelationInterface.h"

class TimeMachine;

class TimeMachineInterface : public ITimeMachineInterface
{
  public:
    TimeMachineInterface(IRevelationInterface* intf);
    ~TimeMachineInterface();

    virtual void Initialize() override;
    virtual void Uninitialize() override;
    virtual void CollectNavigationViews() override;

  private:
    IRevelationInterface* m_interface = nullptr;

    TimeMachine* m_timeMachine = nullptr;
};
