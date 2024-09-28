#include "TimeMachine.h"
#include "TimeMachineFilter.h"
#include "TimeMachineGanttView.h"
#include "IRevelationInterface.h"

TimeMachine::TimeMachine(IRevelationInterface* intf, QWidget* parent)
    : m_interface(intf), QWidget(parent)
{
    ui.setupUi(this);

    Initialize();
}

TimeMachine::~TimeMachine()
{
}

void TimeMachine::Initialize()
{
    InitWidget();
    InitSignalSlots();
}

void TimeMachine::InitWidget()
{
    m_timeMachindFilter    = new TimeMachineFilter(m_interface);
    m_timeMachindGanttView = new TimeMachineGanttView(m_interface);

    std::vector<QWidget*> widgets{m_timeMachindFilter, m_timeMachindGanttView};
    std::vector<QWidget*> parents{ui.eFilterWidget, ui.eGanttWidget};
    for (int i = 0; i < 2; ++i)
    {
        QWidget* widget = widgets[i];
        QWidget* parent = parents[i];

        QGridLayout* layout = new QGridLayout(parent);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(widget);
        parent->setLayout(layout);
    }

    ui.eFilterWidget->setObjectName("filter");
    FluStyleSheetUitls::setQssByFileName("/resources/qss/light/TimeMachine.qss", this);
}

void TimeMachine::InitSignalSlots()
{
    connect(m_timeMachindFilter, &TimeMachineFilter::TaskFiltered, m_timeMachindGanttView, &TimeMachineGanttView::OnTaskFiltered);
}
