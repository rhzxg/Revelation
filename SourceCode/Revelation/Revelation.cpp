#include "Revelation.h"
#include "IRevelationInterface.h"
#include "RevelationListView.h"
#include "RevelationListModel.h"
#include "RevelationRightSidebar.h"
#include "RevelationBottomBar.h"
#include "Utility/IUtilityInterface.h"
#include "DataPersistence/IDataPersistenceInterface.h"
#include <QLabel>
#include <QString>
#include <QMainWindow>
#include <QMessageBox>

Revelation::Revelation(IRevelationInterface* intf, QWidget* parent)
    : m_interface(intf), QWidget(parent)
{
    ui.setupUi(this);

    Initialize();
}

Revelation::~Revelation()
{
}

void Revelation::ReteiveDataFromDatabase()
{
    auto dataPersistenceIntf = m_interface->GetInterfaceById<IDataPersistenceInterface>("DataPersistence");
    if (nullptr == dataPersistenceIntf)
    {
        return;
    }

    std::vector<TaskPrototype> tasks;
    dataPersistenceIntf->GetTaskSerializer()->RetrieveTasks(tasks);

    for (const auto& viewPr : m_listViews)
    {
        TaskStatus           taskStatus = viewPr.first;
        RevelationListView*  view       = viewPr.second;
        RevelationListModel* model      = (RevelationListModel*)view->model();

        for (TaskPrototype& task : tasks)
        {
            if (task.m_taskStatus == taskStatus)
            {
                model->InsertTaskItem(task, true);
            }
        }
    }
}

void Revelation::resizeEvent(QResizeEvent* event)
{
    m_rightSidebar->resize(m_rightSidebar->width(), this->height());
    m_rightSidebar->move(this->width() - m_rightSidebar->width(), 0);

    m_bottomBar->move(this->width() / 2 - m_bottomBar->width() / 2,
                      this->height() - 100);
}

void Revelation::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    GetSidebar(RevelationSidebar::Bottom)->show();

    m_interface->Broadcast(BroadcastType::WidgetInitialized);
}

void Revelation::Initialize()
{
    InitWidget();
    InitSignalSlots();
}

void Revelation::InitWidget()
{
    auto mainWindow = (QMainWindow*)m_interface->GetMainWindow();
    if (nullptr != mainWindow)
    {
        QIcon icon("./resources/images/icon.ico");
        mainWindow->setWindowIcon(icon);
        mainWindow->setWindowTitle("Revelation");
    }

    GetSidebar(RevelationSidebar::Bottom)->hide();
    GetSidebar(RevelationSidebar::Right)->hide();

    std::vector<QLabel*>    labels{ui.labelTitleTodo, ui.labelTitleDoing, ui.labelTitleTesting, ui.labelTitleDone};
    std::vector<QWidget*>   widgets{ui.taskWidgetTodo, ui.taskWidgetDoing, ui.taskWidgetTesting, ui.taskWidgetDone};
    std::vector<TaskStatus> listTypes{TaskStatus::Todo, TaskStatus::Doing, TaskStatus::Testing, TaskStatus::Done};
    std::vector<QString>    objNames{"todo", "doing", "testing", "done"};
    for (int i = 0; i < 4; ++i)
    {
        QLabel*    label   = labels[i];
        QWidget*   widget  = widgets[i];
        TaskStatus type    = listTypes[i];
        QString    objName = objNames[i];

        label->setObjectName(objName);
        label->setAlignment(Qt::AlignCenter);

        widget->setObjectName(objName);

        RevelationListView* view = new RevelationListView(m_interface);
        view->SetViewType(type);
        QGridLayout* layout = new QGridLayout(widget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(view);
        widget->setLayout(layout);

        m_listViews.emplace(type, view);

        // [view => this] task re-parenting
        connect(view, SIGNAL(TaskItemReparenting(TaskPrototype, TaskStatus, TaskStatus)),
                this, SLOT(OnTaskItemReparenting(TaskPrototype, TaskStatus, TaskStatus)));

        // [view => right sidebar] task selection
        connect(view, SIGNAL(TaskItemSelected(const TaskPrototype&)),
                GetSidebar(RevelationSidebar::Right), SLOT(OnTaskItemSelected(const TaskPrototype&)));
    }

    FluStyleSheetUitls::setQssByFileName("/resources/qss/light/Revelation.qss", this);
}

void Revelation::InitSignalSlots()
{
    connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, [&](FluTheme theme) { OnThemeChanged(); });
}

RevelationSidebar* Revelation::GetSidebar(RevelationSidebar::Side side)
{
    RevelationSidebar* sidebar = nullptr;

    if (RevelationSidebar::Right == side)
    {
        if (nullptr == m_rightSidebar)
        {
            m_rightSidebar = new RevelationRightSidebar(m_interface, this);

            // [right sidebar => this] task data edited
            connect(m_rightSidebar, SIGNAL(TaskItemEdited(const TaskPrototype&)),
                    this, SLOT(OnTaskItemEdited(const TaskPrototype&)));

            // [right sidebar => this] task data deleted
            connect(m_rightSidebar, SIGNAL(TaskItemDeleted(const TaskPrototype&)),
                    this, SLOT(OnTaskItemDeleted(const TaskPrototype&)));

            // [this => right sidebar] task re-parenting
            connect(this, SIGNAL(TaskItemReparenting(const TaskPrototype&)),
                    m_rightSidebar, SLOT(OnTaskReparenting(const TaskPrototype&)));
        }
        sidebar = m_rightSidebar;
    }
    else if (RevelationSidebar::Bottom == side)
    {
        if (nullptr == m_bottomBar)
        {
            m_bottomBar = new RevelationBottomBar(m_interface, this);

            // [bottom bar => this] task creation
            connect(m_bottomBar, SIGNAL(TaskItemCreated(TaskPrototype, TaskStatus, TaskStatus)),
                    this, SLOT(OnTaskItemReparenting(TaskPrototype, TaskStatus, TaskStatus)));
        }
        sidebar = m_bottomBar;
    }

    return sidebar;
}

void Revelation::OnThemeChanged()
{
    if (FluThemeUtils::isLightTheme())
    {
        FluStyleSheetUitls::setQssByFileName("/resources/qss/light/Revelation.qss", this);
    }
    else
    {
        FluStyleSheetUitls::setQssByFileName("/resources/qss/dark/Revelation.qss", this);
    }
}

void Revelation::OnTaskItemReparenting(TaskPrototype task, TaskStatus from, TaskStatus to)
{
    if (from == to)
    {
        return;
    }

    auto fromFinder = m_listViews.find(from);
    if (fromFinder != m_listViews.end())
    {
        RevelationListView*  view  = fromFinder->second;
        RevelationListModel* model = (RevelationListModel*)view->model();
        model->RemoveTaskItem(task);
    }

    auto toFinder = m_listViews.find(to);
    if (toFinder != m_listViews.end())
    {
        RevelationListView*  view  = toFinder->second;
        RevelationListModel* model = (RevelationListModel*)view->model();
        model->InsertTaskItem(task);
    }

    // refresh right sidebar item status
    emit TaskItemReparenting(task);

    // change data in database
    auto threadTaskCreator         = m_interface->GetInterfaceById<IUtilityInterface>("Utility")->GetThreadTaskCreator();
    auto dataPersistenceIntf = m_interface->GetInterfaceById<IDataPersistenceInterface>("DataPersistence");
    if (nullptr != threadTaskCreator && nullptr != dataPersistenceIntf)
    {
        threadTaskCreator->RunAsyncTask([=]() {
            dataPersistenceIntf->GetTaskSerializer()->RecordTask(task);
        });
    }
}

void Revelation::OnTaskItemEdited(const TaskPrototype& task)
{
    auto viewFinder = m_listViews.find(task.m_taskStatus);
    if (viewFinder != m_listViews.end())
    {
        RevelationListView*  view  = viewFinder->second;
        RevelationListModel* model = (RevelationListModel*)view->model();
        model->ChangeTaskItem(task);
    }

    auto threadTaskCreator         = m_interface->GetInterfaceById<IUtilityInterface>("Utility")->GetThreadTaskCreator();
    auto dataPersistenceIntf = m_interface->GetInterfaceById<IDataPersistenceInterface>("DataPersistence");
    if (nullptr != threadTaskCreator && nullptr != dataPersistenceIntf)
    {
        threadTaskCreator->RunAsyncTask([=]() {
            dataPersistenceIntf->GetTaskSerializer()->RecordTask(task);
        });
    }
}

void Revelation::OnTaskItemDeleted(const TaskPrototype& task)
{
    auto viewFinder = m_listViews.find(task.m_taskStatus);
    if (viewFinder != m_listViews.end())
    {
        RevelationListView*  view  = viewFinder->second;
        RevelationListModel* model = (RevelationListModel*)view->model();
        model->RemoveTaskItem(task);
    }

    auto threadTaskCreator         = m_interface->GetInterfaceById<IUtilityInterface>("Utility")->GetThreadTaskCreator();
    auto dataPersistenceIntf = m_interface->GetInterfaceById<IDataPersistenceInterface>("DataPersistence");
    if (nullptr != threadTaskCreator && nullptr != dataPersistenceIntf)
    {
        threadTaskCreator->RunAsyncTask([=]() {
            dataPersistenceIntf->GetTaskSerializer()->RemoveTask(task);
        });
    }
}