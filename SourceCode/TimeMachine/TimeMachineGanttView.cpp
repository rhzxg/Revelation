#include "TimeMachineGanttView.h"
#include <QItemSelectionModel>
#include <QGridLayout>
#include <QTreeView>
#include <QHeaderView>
#include <QAbstractItemView>
#include <KDGanttGraphicsView>

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

    m_view->leftView()->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->leftView()->setFixedWidth(200);
    ((QTreeView*)m_view->leftView())->header()->setSectionResizeMode(QHeaderView::Stretch);

    m_view->graphicsView()->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

void TimeMachineGanttView::InitSignalSlots()
{
}
