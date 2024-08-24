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
    dataPersistenceIntf->RetrieveTasksFromDatabase(tasks);

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
    m_rightSidebarWrapper->resize(m_rightSidebarWrapper->width(), this->height() + 10);
    m_rightSidebarWrapper->move(this->width() - m_rightSidebarWrapper->width() + 5, -5);

    m_bottomBarWrapper->move(this->width() / 2 - m_bottomBarWrapper->width() / 2,
                             this->height() - 100);
}

void Revelation::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    GetSidebarWrapper(RevelationSidebar::Bottom)->show();
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

    GetSidebarWrapper(RevelationSidebar::Bottom)->hide();
    GetSidebarWrapper(RevelationSidebar::Right)->hide();

    std::vector<QLabel*>    labels{ui.labelTitleTodo, ui.labelTitleDoing, ui.labelTitleVerifying, ui.labelTitleDone};
    std::vector<QWidget*>   widgets{ui.taskWidgetTodo, ui.taskWidgetDoing, ui.taskWidgetVerifying, ui.taskWidgetDone};
    std::vector<QString>    colors{"#CAF0FC", "#ADE8F6", "#90E0EF", "#48CAE4"};
    std::vector<TaskStatus> listTypes{TaskStatus::Todo, TaskStatus::Doing, TaskStatus::Testing, TaskStatus::Done};
    for (int i = 0; i < 4; ++i)
    {
        QLabel*    label  = labels[i];
        QWidget*   widget = widgets[i];
        TaskStatus type   = listTypes[i];

        QString labelStyle  = QString("QLabel {background-color: %1; color: #FFFFFF; border-radius: 8px; font-size: 16px;}").arg(colors[i]);
        QString widgetStyle = QString("QWidget#widget {background-color: %1; color: #FFFFFF; border-radius: 8px;}").arg(colors[i]);

        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet(labelStyle);

        widget->setObjectName("widget");
        widget->setStyleSheet(widgetStyle);

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
                GetSidebarWrapper(RevelationSidebar::Right)->GetSidebar(), SLOT(OnTaskItemSelected(const TaskPrototype&)));
    }
}

void Revelation::InitSignalSlots()
{
}

RevelationSidebarWrapper* Revelation::GetSidebarWrapper(RevelationSidebar::Side side)
{
    RevelationSidebarWrapper* sidebarWrapper = nullptr;

    if (RevelationSidebar::Right == side)
    {
        if (nullptr == m_rightSidebarWrapper)
        {
            m_rightSidebarWrapper = new RevelationSidebarWrapper(this);
            auto rightSidebar     = new RevelationRightSidebar(m_interface, m_rightSidebarWrapper);
            m_rightSidebarWrapper->SetSidebar(rightSidebar);

            // [right sidebar => this] task data edited
            connect(rightSidebar, SIGNAL(TaskItemEdited(const TaskPrototype&)),
                    this, SLOT(OnTaskItemEdited(const TaskPrototype&)));

            // [right sidebar => this] task data deleted
            connect(rightSidebar, SIGNAL(TaskItemDeleted(const TaskPrototype&)),
                    this, SLOT(OnTaskItemDeleted(const TaskPrototype&)));

            // [this => right sidebar] task re-parenting
            connect(this, SIGNAL(TaskItemReparenting(const TaskPrototype&)),
                    rightSidebar, SLOT(OnTaskReparenting(const TaskPrototype&)));
        }
        sidebarWrapper = m_rightSidebarWrapper;
    }
    else if (RevelationSidebar::Bottom == side)
    {
        if (nullptr == m_bottomBarWrapper)
        {
            m_bottomBarWrapper = new RevelationSidebarWrapper(this);
            auto bottomBar     = new RevelationBottomBar(m_interface, m_bottomBarWrapper);
            m_bottomBarWrapper->SetSidebar(bottomBar);

            m_bottomBarWrapper->setFixedSize(bottomBar->width(), 60);

            // [bottom bar => this] task creation
            connect(bottomBar, SIGNAL(TaskItemCreated(TaskPrototype, TaskStatus, TaskStatus)),
                    this, SLOT(OnTaskItemReparenting(TaskPrototype, TaskStatus, TaskStatus)));
        }
        sidebarWrapper = m_bottomBarWrapper;
    }

    return sidebarWrapper;
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
    auto taskCreator         = m_interface->GetInterfaceById<IUtilityInterface>("Utility")->GetTaskCreator();
    auto dataPersistenceIntf = m_interface->GetInterfaceById<IDataPersistenceInterface>("DataPersistence");
    if (nullptr != taskCreator && nullptr != dataPersistenceIntf)
    {
        taskCreator->RunAsyncTask([=]() {
            dataPersistenceIntf->InsertOrReplaceTaskInDatabase(task);
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

    auto taskCreator         = m_interface->GetInterfaceById<IUtilityInterface>("Utility")->GetTaskCreator();
    auto dataPersistenceIntf = m_interface->GetInterfaceById<IDataPersistenceInterface>("DataPersistence");
    if (nullptr != taskCreator && nullptr != dataPersistenceIntf)
    {
        taskCreator->RunAsyncTask([=]() {
            dataPersistenceIntf->InsertOrReplaceTaskInDatabase(task);
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

    auto taskCreator         = m_interface->GetInterfaceById<IUtilityInterface>("Utility")->GetTaskCreator();
    auto dataPersistenceIntf = m_interface->GetInterfaceById<IDataPersistenceInterface>("DataPersistence");
    if (nullptr != taskCreator && nullptr != dataPersistenceIntf)
    {
        taskCreator->RunAsyncTask([=]() {
            dataPersistenceIntf->RemoveTaskFromDatabase(task);
        });
    }
}