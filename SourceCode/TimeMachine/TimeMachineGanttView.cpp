#include "TimeMachineGanttView.h"
#include <QItemSelectionModel>
#include <QGridLayout>
#include <QTreeView>
#include <QHeaderView>
#include <QAbstractItemView>
#include <KDGanttGraphicsView>
#include <KDGanttGlobal>

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
    layout->setContentsMargins(0, 0, 4, 0);

    m_view  = new KDGantt::View();
    m_model = new TimeMachineGanttModel(this);
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

void TimeMachineGanttView::OnTaskFiltered(const std::map<std::string, std::vector<TaskPrototype>>& dateToTasks)
{
    m_model->clear();
    for (const auto& dateToTasksPair : dateToTasks)
    {
        std::string                date  = dateToTasksPair.first;
        std::vector<TaskPrototype> tasks = dateToTasksPair.second;

        // parent item:
        QModelIndex rootIndex  = m_view->rootIndex();
        int         parentRow  = m_model->rowCount(rootIndex);
        
        m_model->insertRows(0, 1, m_view->rootIndex());

        //// child items:
        //QModelIndex parentIndex = m_model->index(parentRow, 0, rootIndex);
        //for (int row = 0; row < tasks.size(); ++row)
        //{
        //    TaskPrototype task = tasks[row];
        //    Node*         node = new Node;
        //    node->setType(KDGantt::TypeTask);

        //    m_model->insertRow(row, 1, node, parentIndex);
        //}
    }
}
