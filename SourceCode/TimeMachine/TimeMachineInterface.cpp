#include "TimeMachineInterface.h"
#include "ICommonWidgetInterface.h"
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
    auto commonWidgetIntf = m_interface->GetCommonWidgetInterface();
    if (nullptr != commonWidgetIntf)
    {
        m_timeMachine = new TimeMachine(m_interface);
        commonWidgetIntf->AddStackedWidget(m_timeMachine, QObject::tr("TimeMachine"), FluAwesomeType::DevUpdate, Qt::AlignCenter);
    }
}
