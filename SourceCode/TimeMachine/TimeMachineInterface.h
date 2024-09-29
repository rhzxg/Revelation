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

    virtual void HandleBroadcast(BroadcastType broadcastType, const std::any& param /* = std::any() */) override;

  private:
    void AddNavigationView();
    void AddSettingsItem();

  private:
    IRevelationInterface* m_interface = nullptr;

    TimeMachine* m_timeMachine = nullptr;
};
