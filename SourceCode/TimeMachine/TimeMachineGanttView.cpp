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
    auto SetNodeByTask = [](Node* node, const TaskPrototype& task) {
        if (task.m_startTime.empty())
        {
            QDateTime dateTime = QDateTime::currentDateTime();
            dateTime.setTime(QTime::fromString("00:00:00", "hh:mm:ss"));
            node->setStart(dateTime);

            auto a = dateTime.toString();
        }
        else
        {
            node->setStart(QDateTime::fromString(QString::fromStdString(task.m_startTime), "yyyy-MM-dd hh:mm:ss"));
        }

        if (task.m_finishTime.empty())
        {
            QDateTime dateTime = QDateTime::currentDateTime();
            dateTime.setTime(QTime::fromString("23:59:59", "hh:mm:ss"));
            node->setEnd(dateTime);

            // test
            auto a = dateTime.toString();
        }
        else
        {
            node->setEnd(QDateTime::fromString(QString::fromStdString(task.m_finishTime), "yyyy-MM-dd hh:mm:ss"));
        }
    };

    m_model->clear();
    for (const auto& dateToTasksPair : dateToTasks)
    {
        std::string                date  = dateToTasksPair.first;
        std::vector<TaskPrototype> tasks = dateToTasksPair.second;

        // parent item:
        QModelIndex rootIndex = m_view->rootIndex();
        int         parentRow = m_model->rowCount(rootIndex);

        Node* node = new Node;
        node->setType(KDGantt::TypeSummary);
        node->setLabel(QString::fromStdString(date));
        QModelIndex parent = m_model->insertNode(node);

        // child items:
        for (int row = 0; row < tasks.size(); ++row)
        {
            TaskPrototype task = tasks[row];
            Node*         node = new Node;
            node->setType(KDGantt::TypeTask);
            node->setLabel(QString::fromStdString(task.m_title));
            SetNodeByTask(node, task);

            m_model->insertNode(node, parent);
        }
    }

    m_view->expandAll();
}
