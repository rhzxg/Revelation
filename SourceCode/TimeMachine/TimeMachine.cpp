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
    std::vector<QString>  objects{"TimeMachineFilter", "TimeMachineGantt"};
    std::vector<QString>  colors{"#CAF0FC", "AAAAAA"};

    for (int i = 0; i < 2; ++i)
    {
        QWidget* widget = widgets[i];
        QWidget* parent = parents[i];
        QString  name   = objects[i];
        QString  color  = colors[i];

        widget->setObjectName(name);
        QString widgetStyle = QString("QWidget#%1{background-color: %2; color: #FFFFFF; border-radius: 8px;}").arg(name).arg(color);
        widget->setStyleSheet(widgetStyle);

        QGridLayout* layout = new QGridLayout(parent);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(widget);
        parent->setLayout(layout);
    }
}

void TimeMachine::InitSignalSlots()
{
}
