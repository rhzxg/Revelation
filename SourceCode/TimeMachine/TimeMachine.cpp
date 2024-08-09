#include "TimeMachine.h"
#include "TimeMachineFilter.h"
#include "TimeMachineGanttView.h"

TimeMachine::TimeMachine(QWidget* parent)
    : QWidget(parent)
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
    m_timeMachindFilter    = new TimeMachineFilter;
    m_timeMachindGanttView = new TimeMachineGanttView;

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
    ui.eFilterWidget->setStyleSheet("QWidget#filter {background-color: #CAF0FC; color: #FFFFFF; border-radius: 8px;}");
}

void TimeMachine::InitSignalSlots()
{
}
