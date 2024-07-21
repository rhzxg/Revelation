#include "RevelationBottomBar.h"
#include "Utility/IUtilityInterface.h"
#include "FluIconUtils.h"

RevelationBottomBar::RevelationBottomBar(IRevelationInterface* intf, QWidget* parent)
    : RevelationSidebar(intf, parent)
{
    ui.setupUi(this);

    Initialize();
}

RevelationBottomBar::~RevelationBottomBar()
{
}

void RevelationBottomBar::Initialize()
{
    InitWidget();
    InitSignalSlots();
}

void RevelationBottomBar::InitWidget()
{
    ui.frame->setStyleSheet("QFrame { background: #FB637E; border-radius: 10px 10px 10px 10px; }");
    ui.editTask->setStyleSheet("QLineEdit { background: transparent; border: none; color: black; }");
    ui.btnConfirm->setStyleSheet("QPushButton { background: transparent; border: none; }"
                                 "QPushButton::hover { background-color: #EC546F; border-radius: 5px;}");

    ui.btnConfirm->setFixedSize(20, 20);
    ui.btnConfirm->setIcon(FluIconUtils::getFluentIcon(FluAwesomeType::CaretUpSolid8));
}

void RevelationBottomBar::InitSignalSlots()
{
    connect(ui.btnConfirm, &QPushButton::clicked, this, &RevelationBottomBar::OnBtnConfirmClicked);
}

void RevelationBottomBar::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        OnBtnConfirmClicked();
    }
    else
    {
        QWidget::keyPressEvent(event);
    }
}

void RevelationBottomBar::OnBtnConfirmClicked()
{
    std::string taskTitle = ui.editTask->text().toStdString();
    if (taskTitle.empty())
    {
        return;
    }

    auto utilityIntf        = m_interface->GetInterfaceById<IUtilityInterface>("Utility");
    auto timeFormatter      = utilityIntf->GetDateTimeFormatter();
    auto snowflakeGenerator = utilityIntf->GetSnowflakeGenerator();

    std::string   now = timeFormatter->GetCurrentDateTimeString(TimeMask::YMDHMS);
    TaskPrototype task;
    task.m_id         = snowflakeGenerator->GenerateNewSnowId();
    task.m_title      = taskTitle;
    task.m_desc       = "";
    task.m_startTime  = now;
    task.m_finishTime = "";
    task.m_deadline   = timeFormatter->ParsePartDateTimeFromString(now, TimeMask::YMD);
    task.m_createTime = now;
    task.m_taskStatus = TaskStatus::Todo;
    task.m_taskType   = TaskType::None;
    task.m_taskTag    = TaskTag::None;

    emit TaskItemCreated(task, TaskStatus::None, TaskStatus::Todo);

    ui.editTask->setText("");
}
