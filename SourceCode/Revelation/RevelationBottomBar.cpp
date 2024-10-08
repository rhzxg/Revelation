#include "RevelationBottomBar.h"
#include "Utility/IUtilityInterface.h"
#include "FluIconUtils.h"
#include "FluStyleSheetUitls.h"

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
    ui.btnConfirm->setToolTip(tr("Add"));
    ui.btnConfirm->setIconSize(QSize(24, 24));
    ui.btnConfirm->setFixedSize(32, 32);
    ui.btnConfirm->setIcon(FluIconUtils::getFluentIcon(FluAwesomeType::Accept));

    FluStyleSheetUitls::setQssByFileName("/resources/qss/light/RevelationBottomBar.qss", this);
}

void RevelationBottomBar::InitSignalSlots()
{
    connect(FluThemeUtils::getUtils(), &FluThemeUtils::themeChanged, [&](FluTheme theme) { OnThemeChanged(); });

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

void RevelationBottomBar::OnThemeChanged()
{
    if (FluThemeUtils::isLightTheme())
    {
        FluStyleSheetUitls::setQssByFileName("/resources/qss/light/RevelationBottomBar.qss", this);
    }
    else
    {
        FluStyleSheetUitls::setQssByFileName("/resources/qss/dark/RevelationBottomBar.qss", this);
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
    task.m_createTime = now;
    task.m_startTime  = "";
    task.m_finishTime = "";
    task.m_deadline   = timeFormatter->ParsePartDateTimeFromString(now, TimeMask::YMD);
    task.m_taskStatus = TaskStatus::Todo;
    task.m_taskType   = TaskType::None;
    task.m_taskTag    = TaskTag::None;

    emit TaskItemCreated(task, TaskStatus::None, TaskStatus::Todo);

    ui.editTask->setText("");
}
