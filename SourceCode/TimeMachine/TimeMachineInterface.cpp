#include "TimeMachineInterface.h"
#include "ICommonWidgetInterface.h"
#include "TimeMachine.h"
#include "FluDef.h"
#include "FluSettingsVersionBox.h"
#include "Utility/IUtilityInterface.h"
#include "FluLabel.h"
#include "FluTimePicker24H.h"
#include "FluTextToggleSwitchEx.h"

TimeMachineInterface::TimeMachineInterface(IRevelationInterface* intf)
    : m_interface(intf)
{
}

TimeMachineInterface::~TimeMachineInterface()
{
    delete m_timeMachine;
}

void TimeMachineInterface::Initialize()
{
}

void TimeMachineInterface::Uninitialize()
{
}

void TimeMachineInterface::HandleBroadcast(BroadcastType broadcastType, const std::any& param /* = std::any() */)
{
    if (broadcastType == BroadcastType::CollectNavigationView)
    {
        AddNavigationView();
    }
    else if (broadcastType == BroadcastType::CollectSettingsItem)
    {
        AddSettingsItem();
    }
    else if (broadcastType == BroadcastType::ChangeTheme)
    {
        FluTheme theme = std::any_cast<FluTheme>(param);
        FluThemeUtils::getUtils()->setTheme(theme);
    }
}

void TimeMachineInterface::AddNavigationView()
{
    auto commonWidgetIntf = m_interface->GetCommonWidgetInterface();
    if (nullptr != commonWidgetIntf)
    {
        m_timeMachine = new TimeMachine(m_interface);
        commonWidgetIntf->AddStackedWidget(m_timeMachine, QObject::tr("TimeMachine"), FluAwesomeType::DevUpdate, Qt::AlignCenter);
    }
}

void TimeMachineInterface::AddSettingsItem()
{
    auto settingsToolkit = m_interface->GetInterfaceById<IUtilityInterface>("Utility")->GetSettingsToolkit();

    auto        timeMachineItem = new FluSettingsVersionBox;
    std::string version         = settingsToolkit->GetString("Version", "TimeMachine", "1.0.0");
    timeMachineItem->getTitleLabel()->setText("TimeMachine");
    timeMachineItem->getInfoLabel()->setText("Tasks visualization over time based on gantt chart");
    timeMachineItem->getVersionLabel()->setText(QString::fromStdString(version));

    QIcon icon = FluIconUtils::getFluentIcon(FluAwesomeType::DevUpdate);
    timeMachineItem->getIconLabel()->setPixmap(icon.pixmap(40, 40));
    timeMachineItem->getIconLabel()->setFixedSize(40, 40);

    //////////////////////////////////////////////////////////////////////////
    bool syncHScroll = settingsToolkit->GetBoolean("SyncHorizontalScroll", "TimeMachine", true);

    auto syncHScrollLabel = new FluLabel;
    syncHScrollLabel->setLabelStyle(FluLabelStyle::BodyTextBlockStyle);
    syncHScrollLabel->setText("Sync horizontal scrolling: ");

    FluTextToggleSwitchEx* syncHScrollSwitch = new FluTextToggleSwitchEx;
    syncHScrollSwitch->setText("ON", "OFF");
    syncHScrollSwitch->blockSignals(true);
    syncHScrollSwitch->setChecked(syncHScroll);
    syncHScrollSwitch->blockSignals(false);
    QObject::connect(syncHScrollSwitch, &FluTextToggleSwitchEx::stateChanged, [=](bool checked) {
        settingsToolkit->SetBoolean("SyncHorizontalScroll", "TimeMachine", checked);
    });

    QHBoxLayout* syncHScrollLayout = new QHBoxLayout;
    syncHScrollLayout->setContentsMargins(0, 0, 0, 0);
    syncHScrollLayout->addWidget(syncHScrollLabel);
    syncHScrollLayout->addStretch();
    syncHScrollLayout->addWidget(syncHScrollSwitch);
    QWidget* syncHScrollWidget = new QWidget;
    syncHScrollWidget->setLayout(syncHScrollLayout);

    timeMachineItem->addWidget(syncHScrollWidget);
    timeMachineItem->addVSplitLine();

    //////////////////////////////////////////////////////////////////////////
    std::string defaultStartTimeStr = settingsToolkit->GetString("StartTime", "TimeMachine", "09:00:00");
    QTime       defaultStartTime    = QTime::fromString(QString::fromStdString(defaultStartTimeStr), "hh:mm:ss");
    auto        startTimeLabel      = new FluLabel;
    startTimeLabel->setLabelStyle(FluLabelStyle::BodyTextBlockStyle);
    startTimeLabel->setText("Default gantt node start time: ");

    FluTimePicker24H* startTimePicker = new FluTimePicker24H(true);
    startTimePicker->blockSignals(true);
    startTimePicker->setCurTime(defaultStartTime);
    startTimePicker->blockSignals(false);
    QObject::connect(startTimePicker, &FluTimePicker24H::selectedTime, [=](QTime time) {
        std::string timeStr = time.toString("hh:mm:ss").toStdString();
        settingsToolkit->SetString("StartTime", "TimeMachine", timeStr);
    });

    QHBoxLayout* startTimeItemLayout = new QHBoxLayout;
    startTimeItemLayout->setContentsMargins(0, 0, 0, 0);
    startTimeItemLayout->addWidget(startTimeLabel);
    startTimeItemLayout->addStretch();
    startTimeItemLayout->addWidget(startTimePicker);
    QWidget* startTimeItemWidget = new QWidget;
    startTimeItemWidget->setLayout(startTimeItemLayout);

    timeMachineItem->addWidget(startTimeItemWidget);
    timeMachineItem->addVSplitLine();

    //////////////////////////////////////////////////////////////////////////
    std::string defaultFinishTimeStr = settingsToolkit->GetString("FinishTime", "TimeMachine", "21:00:00");
    QTime       defaultFinishTime    = QTime::fromString(QString::fromStdString(defaultFinishTimeStr), "hh:mm:ss");
    auto        finishTimeLabel      = new FluLabel;
    finishTimeLabel->setLabelStyle(FluLabelStyle::BodyTextBlockStyle);
    finishTimeLabel->setText("Default gantt node finish time: ");

    FluTimePicker24H* finishTimePicker = new FluTimePicker24H(true);
    finishTimePicker->blockSignals(true);
    finishTimePicker->setCurTime(defaultFinishTime);
    finishTimePicker->blockSignals(false);
    QObject::connect(finishTimePicker, &FluTimePicker24H::selectedTime, [=](QTime time) {
        std::string timeStr = time.toString("hh:mm:ss").toStdString();
        settingsToolkit->SetString("FinishTime", "TimeMachine", timeStr);
    });

    QHBoxLayout* finishTimeItemLayout = new QHBoxLayout;
    finishTimeItemLayout->setContentsMargins(0, 0, 0, 0);
    finishTimeItemLayout->addWidget(finishTimeLabel);
    finishTimeItemLayout->addStretch();
    finishTimeItemLayout->addWidget(finishTimePicker);
    QWidget* finishTimeItemWidget = new QWidget;
    finishTimeItemWidget->setLayout(finishTimeItemLayout);

    timeMachineItem->addWidget(finishTimeItemWidget);

    //////////////////////////////////////////////////////////////////////////
    m_interface->GetCommonWidgetInterface()->AddSettingsItem(timeMachineItem);
}
