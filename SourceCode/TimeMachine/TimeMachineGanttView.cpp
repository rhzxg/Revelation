#include "TimeMachineGanttView.h"
#include <QItemSelectionModel>
#include <QGridLayout>

TimeMachineGanttView::TimeMachineGanttView(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    Initialize();
}

TimeMachineGanttView::~TimeMachineGanttView()
{
}

void TimeMachineGanttView::Initialize()
{
    InitWidget();
    InitSignalSlots();
}

void TimeMachineGanttView::InitWidget()
{
    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_view  = new KDGantt::View();
    m_model = new TimeMachineGanttModel(m_view);
    m_view->setModel(m_model);
    m_view->setSelectionModel(new QItemSelectionModel(m_model));

    layout->addWidget(m_view);
    this->setLayout(layout); 
}

void TimeMachineGanttView::InitSignalSlots()
{
}
