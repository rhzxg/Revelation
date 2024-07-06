#include "Revelation.h"
#include "IRevelationInterface.h"
#include "RevelationListView.h"
#include "RevelationListModel.h"
#include "RevelationLeftSidebar.h"
#include "RevelationRightSidebar.h"
#include "RevelationBottomBar.h"
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
    dataPersistenceIntf->ReteiveTasksFromDatabase(tasks);

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

void Revelation::mouseMoveEvent(QMouseEvent* event)
{
    if (event->pos().x() < 10 && event->pos().y() < 10)
    {
        GetSidebar(RevelationSidebar::Left)->SetVisible(true);
    }
}

void Revelation::resizeEvent(QResizeEvent* event)
{
    // sidebar
    emit CentralWidgetMoved(mapToGlobal(this->pos()), this->size());
    emit CentralWidgetResized(this->size());
}

void Revelation::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    GetSidebar(RevelationSidebar::Bottom)->SetVisible(true);
}

void Revelation::Initialize()
{
    InitWidget();
    InitSignalSlots();
}

void Revelation::InitWidget()
{
    this->setMouseTracking(true);

    auto iconPath   = m_interface->GetResourcePath() / "images" / "icon.ico";
    auto mainWindow = (QMainWindow*)m_interface->GetMainWindow();
    if (std::filesystem::exists(iconPath) && nullptr != mainWindow)
    {
        QPixmap pixmap(QString::fromStdString(iconPath.u8string()));
        mainWindow->setWindowIcon(pixmap);
    }

    std::vector<QLabel*>    labels{ui.labelTitleTodo, ui.labelTitleDoing, ui.labelTitleTesting, ui.labelTitleDone};
    std::vector<QWidget*>   widgets{ui.taskWidgetTodo, ui.taskWidgetDoing, ui.taskWidgetTesting, ui.taskWidgetDone};
    std::vector<QString>    colors{"#CAF0FC", "#ADE8F6", "#90E0EF", "#48CAE4"};
    std::vector<TaskStatus> listTypes{TaskStatus::Todo, TaskStatus::Doing, TaskStatus::Testing, TaskStatus::Done};
    for (int i = 0; i < 4; ++i)
    {
        QLabel*    label  = labels[i];
        QWidget*   widget = widgets[i];
        TaskStatus type   = listTypes[i];

        QString labelStyle  = QString("QLabel {background-color:%1; color:#FFFFFF; border-radius:8px; font-size: 16px;}").arg(colors[i]);
        QString widgetStyle = QString("QWidget#widget {background-color:%1; color:#FFFFFF; border-radius:8px;}").arg(colors[i]);

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

        // task re-parenting
        connect(view, SIGNAL(TaskItemReparenting(TaskPrototype, TaskStatus, TaskStatus)),
                this, SLOT(OnTaskItemReparenting(TaskPrototype, TaskStatus, TaskStatus)));

        // task selection
        connect(view, SIGNAL(TaskItemSelected(TaskPrototype)),
                GetSidebar(RevelationSidebar::Right), SLOT(OnTaskItemSelected(TaskPrototype)));
    }

    GetSidebar(RevelationSidebar::Left)->SetVisible(false);
    GetSidebar(RevelationSidebar::Right)->SetVisible(false);
    GetSidebar(RevelationSidebar::Bottom)->SetVisible(false);
}

void Revelation::InitSignalSlots()
{
}

RevelationSidebar* Revelation::GetSidebar(RevelationSidebar::Side side)
{
    RevelationSidebar* sidebar = nullptr;

    if (RevelationSidebar::Left == side)
    {
        if (nullptr == m_leftSidebar)
        {
            m_leftSidebar = new RevelationLeftSidebar(m_interface, this);

            connect(this, SIGNAL(CentralWidgetMoved(const QPoint&, const QSize&)),
                    m_leftSidebar, SLOT(OnCentralWidgetMoved(const QPoint&)));

            connect(this, SIGNAL(CentralWidgetResized(const QSize&)),
                    m_leftSidebar, SLOT(OnCentralWidgetResized(const QSize&)));
        }
        sidebar = m_leftSidebar;
    }
    else if (RevelationSidebar::Right == side)
    {
        if (nullptr == m_rightSidebar)
        {
            m_rightSidebar = new RevelationRightSidebar(m_interface, this);

            connect(this, SIGNAL(CentralWidgetMoved(const QPoint&, const QSize&)),
                    m_rightSidebar, SLOT(OnCentralWidgetMoved(const QPoint&, const QSize&)));

            connect(this, SIGNAL(CentralWidgetResized(const QSize&)),
                    m_rightSidebar, SLOT(OnCentralWidgetResized(const QSize&)));

            // task data edited
            connect(m_rightSidebar, SIGNAL(TaskItemEdited(const TaskPrototype&)),
                    this, SLOT(OnTaskItemEdited(const TaskPrototype&)));
            
            // task data deleted
            connect(m_rightSidebar, SIGNAL(TaskItemDeleted(const TaskPrototype&)),
                    this, SLOT(OnTaskItemDeleted(const TaskPrototype&)));
        }
        sidebar = m_rightSidebar;
    }
    else if (RevelationSidebar::Bottom == side)
    {
        if (nullptr == m_bottomBar)
        {
            m_bottomBar = new RevelationBottomBar(m_interface, this);

            connect(this, SIGNAL(CentralWidgetMoved(const QPoint&, const QSize&)),
                    m_bottomBar, SLOT(OnCentralWidgetMoved(const QPoint&, const QSize&)));

            // task creation
            connect(m_bottomBar, SIGNAL(TaskItemCreated(TaskPrototype, TaskStatus, TaskStatus)),
                    this, SLOT(OnTaskItemReparenting(TaskPrototype, TaskStatus, TaskStatus)));
        }
        sidebar = m_bottomBar;
    }

    return sidebar;
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

    auto dataPersistenceIntf = m_interface->GetInterfaceById<IDataPersistenceInterface>("DataPersistence");
    if (nullptr != dataPersistenceIntf)
    {
        std::thread databaseThread([=]() {
            dataPersistenceIntf->InsertOrReplaceTaskInDatabase(task);
        });
        databaseThread.detach();
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

    auto dataPersistenceIntf = m_interface->GetInterfaceById<IDataPersistenceInterface>("DataPersistence");
    if (nullptr != dataPersistenceIntf)
    {
        std::thread databaseThread([=]() {
            dataPersistenceIntf->InsertOrReplaceTaskInDatabase(task);
        });
        databaseThread.detach();
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

    auto dataPersistenceIntf = m_interface->GetInterfaceById<IDataPersistenceInterface>("DataPersistence");
    if (nullptr != dataPersistenceIntf)
    {
        std::thread databaseThread([=]() {
            dataPersistenceIntf->RemoveTaskFromDatabase(task);
        });
        databaseThread.detach();
    }
}