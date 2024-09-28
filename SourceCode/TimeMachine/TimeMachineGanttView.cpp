#include "TimeMachineGanttView.h"
#include "IRevelationInterface.h"
#include "Utility/IUtilityInterface.h"
#include "Utility/IDateTimeFormatter.h"
#include "FluStyleSheetUitls.h"
#include <QMenu>
#include <QTreeView>
#include <QClipboard>
#include <QScrollBar>
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
    m_unityInterface = m_interface->GetInterfaceById<IUtilityInterface>("Utility");
    m_timeFormatter  = m_unityInterface->GetDateTimeFormatter();

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
    m_leftView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_rightView->setReadOnly(true);

    FluStyleSheetUitls::setQssByFileName("/resources/qss/light/TimeMachineGanttView.qss", this);
}

void TimeMachineGanttView::InitSignalSlots()
{
    connect(m_leftView, &QTreeView::customContextMenuRequested, this, &TimeMachineGanttView::OnContextMenuEvent);
    connect(m_rightView->verticalScrollBar(), &QScrollBar::valueChanged, this, &TimeMachineGanttView::OnRightViewVerticallyScrolled);
}

void TimeMachineGanttView::CopyTasksToClipboard(Node* summaryNode)
{
    if (nullptr == summaryNode)
    {
        return;
    }

    QStringList summaries;
    for (int i = 0; i < summaryNode->childCount(); ++i)
    {
        Node* taskNode = summaryNode->child(i);
        if (nullptr == taskNode)
        {
            continue;
        }

        QStringList summary;
        summary << taskNode->start().time().toString("hh:mm");
        summary << " ->";
        summary << taskNode->end().time().toString("hh:mm");
        summary << " ";
        summary << taskNode->label();
        if (taskNode->status() != KDGantt::Done)
        {
            summary << " " + tr("(Not done)");
        }

        summaries << summary.join("");
    }

    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(summaries.join("\n"));
}

void TimeMachineGanttView::SetupNodeTimeByTask(const std::string& date, Node* node, const TaskPrototype& task)
{
    std::string defaultStratTimeStr  = m_unityInterface->GetSettingsToolkit()->GetString("StartTime", "TimeMachine", "00:00:00");
    std::string defaultFinishTimeStr = m_unityInterface->GetSettingsToolkit()->GetString("FinishTime", "TimeMachine", "21:00:00");
    QTime       defaultStartTime     = QTime::fromString(QString::fromStdString(defaultStratTimeStr), "hh:mm:ss");
    QTime       defaultFinishTime    = QTime::fromString(QString::fromStdString(defaultFinishTimeStr), "hh:mm:ss");

    // category day
    QDateTime categoryDateTime;
    categoryDateTime.setDate(QDate::fromString(QString::fromStdString(date), "yyyy-MM-dd"));

    QDateTime createDateTime = QDateTime::fromString(QString::fromStdString(task.m_createTime), "yyyy-MM-dd hh:mm:ss");

    // start
    {
        QDateTime dayBegining = categoryDateTime;
        dayBegining.setTime(defaultStartTime);

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
        QDateTime dayEnding = categoryDateTime;
        dayEnding.setTime(defaultFinishTime);
        node->setEnd(dayEnding);
    }
    else
    {
        node->setEnd(QDateTime::fromString(QString::fromStdString(task.m_finishTime), "yyyy-MM-dd hh:mm:ss"));
    }

    //// test
    // QString from = node->start().toString();
    // QString to   = node->end().toString();
}

void TimeMachineGanttView::SetupNodeConstraints(const std::vector<Node*>& nodes, std::vector<QModelIndex>& indexes)
{
    if (m_prevIndexes.empty())
    {
        m_prevNodes   = nodes;
        m_prevIndexes = indexes;
    }
    else
    {
        // find same tasks
        std::vector<int>        sameTaskIndexes;
        std::map<uint64_t, int> prevID2IndexMap, currID2IndexMap;
        for (int index = 0; index < m_prevNodes.size(); ++index)
        {
            prevID2IndexMap.emplace(m_prevNodes[index]->id(), index);
        }
        for (int index = 0; index < nodes.size(); ++index)
        {
            currID2IndexMap.emplace(nodes[index]->id(), index);
        }

        m_constraintModel.clear();
        for (const auto& id2IndexPair : prevID2IndexMap)
        {
            Int64       prevID     = id2IndexPair.first;
            int         prevIndex  = id2IndexPair.second;
            QModelIndex prevQIndex = m_prevIndexes[prevIndex];

            auto finder = currID2IndexMap.find(prevID);
            if (finder != currID2IndexMap.end())
            {
                int         currIndex  = finder->second;
                QModelIndex currQIndex = indexes[currIndex];

                KDGantt::Constraint constraint(prevQIndex, currQIndex);
                m_constraintModel.addConstraint(constraint);
            }
        }
        m_view->setConstraintModel(&m_constraintModel);

        m_prevNodes   = nodes;
        m_prevIndexes = indexes;
    }
}

void TimeMachineGanttView::OnTaskFiltered(const std::vector<DateToTasks>& dateToTaskVec)
{
    // clear cache
    m_prevNodes.clear();
    m_prevIndexes.clear();

    // refresh model
    m_model->clear();
    for (const auto& dateToTasksPair : dateToTaskVec)
    {
        std::string                date  = dateToTasksPair.first;
        std::vector<TaskPrototype> tasks = dateToTasksPair.second;

        // parent item:
        QModelIndex rootIndex = m_view->rootIndex();
        int         parentRow = m_model->rowCount(rootIndex);

        Node* parentNode = new Node;
        parentNode->setType(KDGantt::TypeSummary);
        parentNode->setLabel(QString::fromStdString(date));
        QModelIndex parent = m_model->insertNode(parentNode);

        // child items:
        std::vector<Node*>       nodes;
        std::vector<QModelIndex> indexes;
        for (int row = 0; row < tasks.size(); ++row)
        {
            TaskPrototype task      = tasks[row];
            Node*         childNode = new Node;

            childNode->setID(task.m_id);
            childNode->setType(KDGantt::TypeTask);
            childNode->setStatus((int)task.m_taskStatus - 1);
            childNode->setLabel(QString::fromStdString(task.m_title));

            SetupNodeTimeByTask(date, childNode, task);

            QModelIndex childIndex = m_model->insertNode(childNode, parent);

            nodes.push_back(childNode);
            indexes.push_back(childIndex);
        }

        SetupNodeConstraints(nodes, indexes);
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

    QAction copyTasksToClipboard(tr("Copy tasks to clipboard"));
    connect(&copyTasksToClipboard, &QAction::triggered, this, [=]() {
        CopyTasksToClipboard(node);
    });

    menu.addAction(&copyTasksToClipboard);

    QPoint globalPos  = mapToGlobal(pos);
    QPoint correctPos = QPoint(globalPos.x(), globalPos.y() + 50);
    menu.exec(correctPos);
}

void TimeMachineGanttView::OnRightViewVerticallyScrolled()
{
    if (!m_unityInterface->GetSettingsToolkit()->GetBoolean("SyncHorizontalScroll", "TimeMachine", true))
    {
        return;
    }

    QRect       visibleRect    = m_leftView->viewport()->rect();
    QModelIndex firstRootIndex = QModelIndex();
    for (int y = visibleRect.top(); y < visibleRect.bottom(); y += 5)
    {
        QModelIndex currentIndex = m_leftView->indexAt(QPoint(0, y));
        if (!currentIndex.isValid() || currentIndex.parent().isValid())
        {
            continue;
        }

        firstRootIndex = currentIndex;
        break;
    }

    if (firstRootIndex.isValid() && firstRootIndex != m_rootIndex)
    {
        m_view->ensureVisible(firstRootIndex);
        m_rootIndex = firstRootIndex;
    }
}
