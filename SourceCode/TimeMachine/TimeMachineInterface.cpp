#include "TimeMachineInterface.h"
#include "INavigationViewInterface.h"
#include "TimeMachine.h"
#include "FluDef.h"

TimeMachineInterface::TimeMachineInterface(IRevelationInterface* intf)
    : m_interface(intf)
{
}

TimeMachineInterface::~TimeMachineInterface()
{
    delete m_timeMachine;
}

void TimeMachineInterface::Initialize()
{
}

void TimeMachineInterface::Uninitialize()
{
}

void TimeMachineInterface::CollectNavigationViews()
{
    auto navViewIntf = m_interface->GetNavViewInterface();
    if (nullptr != navViewIntf)
    {
        m_timeMachine = new TimeMachine;
        navViewIntf->AddStackedWidget(m_timeMachine, QObject::tr("TimeMachine"), FluAwesomeType::DevUpdate, Qt::AlignCenter);
    }
}
