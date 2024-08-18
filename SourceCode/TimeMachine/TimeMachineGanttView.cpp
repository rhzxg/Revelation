#include "TimeMachineGanttView.h"
#include "IRevelationInterface.h"
#include "Utility/IUtilityInterface.h"
#include "Utility/IDateTimeFormatter.h"
#include <QMenu>
#include <QTreeView>
#include <QClipboard>
#include <QHeaderView>
#include <QGridLayout>
#include <QAbstractItemView>
#include <QItemSelectionModel>
#include <KDGanttGlobal>
#include <KDGanttGraphicsView>
#include <KDGanttDateTimeGrid>

TimeMachineGanttView::TimeMachineGanttView(IRevelationInterface* intf, QWidget* parent)
    : m_interface(intf), QWidget(parent)
{
    ui.setupUi(this);

    Initialize();
}

TimeMachineGanttView::~TimeMachineGanttView()
{
}

void TimeMachineGanttView::Initialize()
{
    m_timeFormatter = m_interface->GetInterfaceById<IUtilityInterface>("Utility")->GetDateTimeFormatter();

    InitWidget();
    InitSignalSlots();
}

void TimeMachineGanttView::InitWidget()
{
    m_view      = new KDGantt::View();
    m_leftView  = (QTreeView*)m_view->leftView();
    m_rightView = m_view->graphicsView();

    m_model = new TimeMachineGanttModel(this);
    m_view->setModel(m_model);
    m_view->setSelectionModel(new QItemSelectionModel(m_model));

    auto* const grid = qobject_cast<KDGantt::DateTimeGrid*>(m_view->grid());
    grid->setScale(KDGantt::DateTimeGrid::ScaleHour);
    grid->setDayWidth(710);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 4, 0);
    layout->addWidget(m_view);
    this->setLayout(layout);

    m_leftView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_leftView->setFixedWidth(200);
    m_leftView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_leftView->header()->setSectionResizeMode(QHeaderView::Stretch);

    m_rightView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_rightView->setReadOnly(true);
}

void TimeMachineGanttView::InitSignalSlots()
{
    connect(m_leftView, &QTreeView::customContextMenuRequested, this, &TimeMachineGanttView::OnContextMenuEvent);
}

void TimeMachineGanttView::SummarizeTasks(Node* summaryNode)
{
    if (nullptr == summaryNode)
    {
        return;
    }

    QString summary;
    for (int i = 0; i < summaryNode->childCount(); ++i)
    {
        Node* taskNode = summaryNode->child(i);
        if (nullptr == taskNode)
        {
            continue;
        }

        summary += taskNode->label();
        if (i != summaryNode->childCount() - 1)
        {
            summary += "\n";
        }
    }

    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(summary);
}

void TimeMachineGanttView::OnTaskFiltered(const std::vector<DateToTasks>& dateToTaskVec)
{
    auto SetNodeByTask = [&](std::string date, Node* node, const TaskPrototype& task) {
        // category day
        QDateTime categoryDateTime;
        categoryDateTime.setDate(QDate::fromString(QString::fromStdString(date), "yyyy-MM-dd"));

        QDateTime createDateTime = QDateTime::fromString(QString::fromStdString(task.m_createTime), "yyyy-MM-dd hh:mm:ss");

        // start
        {
            QDateTime dayBegining     = categoryDateTime;
            QTime     dayBeginingTime = dayBegining.time();
            dayBeginingTime.setHMS(0, 0, 0);
            dayBegining.setTime(dayBeginingTime);

            // later one
            if (task.m_startTime.empty())
            {
                node->setStart(dayBegining > createDateTime ? dayBegining : createDateTime);
            }
            else
            {
                QDateTime startTime = QDateTime::fromString(QString::fromStdString(task.m_startTime), "yyyy-MM-dd hh:mm:ss");
                node->setStart(dayBegining > startTime ? dayBegining : startTime);
            }
        }

        // end
        if (task.m_finishTime.empty())
        {
            QDateTime dayEnding     = categoryDateTime;
            QTime     dayEndingTime = dayEnding.time();
            dayEndingTime.setHMS(23, 59, 59);
            dayEnding.setTime(dayEndingTime);
            node->setEnd(dayEnding);
        }
        else
        {
            node->setEnd(QDateTime::fromString(QString::fromStdString(task.m_finishTime), "yyyy-MM-dd hh:mm:ss"));
        }

        //// test
        // QString from = node->start().toString();
        // QString to   = node->end().toString();
    };

    m_model->clear();
    for (const auto& dateToTasksPair : dateToTaskVec)
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
            node->setStatus((int)task.m_taskStatus - 1);
            node->setLabel(QString::fromStdString(task.m_title));
            SetNodeByTask(date, node, task);

            m_model->insertNode(node, parent);
        }
    }

    m_view->expandAll();
}

void TimeMachineGanttView::OnContextMenuEvent(const QPoint& pos)
{
    QModelIndex currentIndex = m_leftView->selectionModel()->currentIndex();
    if (!currentIndex.isValid())
    {
        return;
    }

    Node* node = static_cast<Node*>(currentIndex.internalPointer());
    if (nullptr == node || node->type() != KDGantt::TypeSummary)
    {
        return;
    }

    QMenu menu;

    QAction summarizeTasks(tr("Summarize tasks"));
    connect(&summarizeTasks, &QAction::triggered, this, [=]() {
        SummarizeTasks(node);
    });

    menu.addAction(&summarizeTasks);

    QPoint globalPos  = mapToGlobal(pos);
    QPoint correctPos = QPoint(globalPos.x(), globalPos.y() + 50);
    menu.exec(correctPos);
}
