#include "RevelationBottomBar.h"
#include "Utility/IUtilityInterface.h"
#include <QMainWindow>

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
    // this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    this->setWindowFlag(Qt::NoDropShadowWindowHint);
    this->setWindowFlag(Qt::FramelessWindowHint);
    // this->setWindowFlag(Qt::Popup);

    this->setAttribute(Qt::WA_TranslucentBackground);

    ui.frame->setStyleSheet("QFrame { background: #FB637E; border-radius: 10px 10px 10px 10px; }");
    ui.editTask->setStyleSheet("QLineEdit { background: transparent; border: none; color: black; }");
    ui.btnConfirm->setStyleSheet("QPushButton { background: transparent; border: none; color: black; }");

    auto iconPath = m_interface->GetResourcePath() / "images" / "up.png";
    ui.btnConfirm->setIcon(QIcon(QString::fromStdString(iconPath.u8string())));
}

void RevelationBottomBar::InitSignalSlots()
{
    connect(ui.btnConfirm, &QPushButton::clicked, this, &RevelationBottomBar::OnBtnConfirmClicked);
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

    emit TaskItemCreated(task, TaskStatus::None, TaskStatus::Todo);

    ui.editTask->setText("");
}
