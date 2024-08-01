#include "TimeMachine.h"
#include "TimeMachineFilter.h"
#include "TimeMachineGantt.h"

TimeMachine::TimeMachine(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    Initialize();
}

TimeMachine::~TimeMachine()
{
    delete m_timeMachineFilterLayout;
    delete m_timeMachineGanttLayout;

    delete m_timeMachindFilter;
    delete m_timeMachindGantt;
}

void TimeMachine::Initialize()
{
    InitWidget();
    InitSignalSlots();
}

void TimeMachine::InitWidget()
{
    m_timeMachindFilter = new TimeMachineFilter;
    m_timeMachineFilterLayout = new QGridLayout(ui.eFilterWidget);
    m_timeMachineFilterLayout->setContentsMargins(0, 0, 0, 0);
    m_timeMachineFilterLayout->addWidget(m_timeMachindFilter);
    ui.eFilterWidget->setLayout(m_timeMachineFilterLayout);
    
    QString widgetStyle = QString("QWidget#widget {background-color:#AAAAAA; color:#FFFFFF; border-radius:8px;}");
    ui.eFilterWidget->setObjectName("widget");
    ui.eFilterWidget->setStyleSheet(widgetStyle);

    m_timeMachindGantt  = new TimeMachineGantt;
    m_timeMachineGanttLayout = new QGridLayout(ui.eGanttWidget);
    m_timeMachineFilterLayout->addWidget(m_timeMachindGantt);
    m_timeMachineGanttLayout->setContentsMargins(0, 0, 0, 0);
    ui.eGanttWidget->setLayout(m_timeMachineGanttLayout);

    QString widgetStyle1 = QString("QWidget#widget {background-color:#AAAAAA; color:#FFFFFF; border-radius:8px;}");
    ui.eGanttWidget->setObjectName("widget");
    ui.eGanttWidget->setStyleSheet(widgetStyle1);
}

void TimeMachine::InitSignalSlots()
{
}
