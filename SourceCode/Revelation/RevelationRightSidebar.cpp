#include "RevelationRightSidebar.h"
#include "Utility/IUtilityInterface.h"
#include <QPropertyAnimation>
#include <QMouseEvent>

RevelationRightSidebar::RevelationRightSidebar(IRevelationInterface* intf, QWidget* parent)
    : RevelationSidebar(intf, parent), m_interface(intf)
{
    ui.setupUi(this);

    Initialize();
}

RevelationRightSidebar::~RevelationRightSidebar()
{
}

void RevelationRightSidebar::Initialize()
{
    m_timeFormatter = m_interface->GetInterfaceById<IUtilityInterface>("Utility")->GetDateTimeFormatter();

    InitWidget();
    InitSignalSlots();
}

void RevelationRightSidebar::InitWidget()
{
    ui.frame->setObjectName("RevelationFrame");
    ui.frame->setStyleSheet("QFrame#RevelationFrame { background: #F0F0F0; border-radius: 7px; }");

    std::vector<FluPushButton*> btns{ui.btnAddToRoutine, ui.btnHide};
    for (FluPushButton* btn : btns)
    {
        btn->setStyleSheet("QPushButton { background: transparent; border: none; }"
                           "QPushButton::hover { background-color: #E1E1E1; border-radius: 5px;}");
        btn->setIconSize(QSize(24, 24));
        btn->setFixedSize(32, 32);
    }

    ui.btnAddToRoutine->setToolTip(tr("Add to daily routine"));
    ui.btnAddToRoutine->setIcon(FluIconUtils::getFluentIcon(FluAwesomeType::FavoriteStar));
    ui.btnHide->setToolTip(tr("Hide"));
    ui.btnHide->setIcon(FluIconUtils::getFluentIcon(FluAwesomeType::Cancel));

    ui.editDesc->setFontPointSize(ui.editTitle->font().pointSize());

    ui.cbType->addItems({tr("None"), tr("Bug"), tr("Feature"), tr("Test"), tr("UI")});

    ui.labelCreateTime->setAlignment(Qt::AlignCenter);
}

void RevelationRightSidebar::InitSignalSlots()
{
    connect(ui.btnAddToRoutine, &QPushButton::clicked, this, &RevelationRightSidebar::OnBtnAddToRoutineClicled);
    connect(ui.btnHide, &QPushButton::clicked, this, [&]() {
        parentWidget()->hide();
    });

    connect(ui.btnDelete, &QPushButton::clicked, this, &RevelationRightSidebar::OnBtnDeleteTaskItemClicked);

    connect(ui.cbType, &QComboBox::currentIndexChanged, this, &RevelationRightSidebar::OnCbTypeIndexChanged);

    connect(ui.btnSelectStartTime, &FluCalendarDateTimePicker::selectedDate, this, &RevelationRightSidebar::OnStartDateSelected);
    connect(ui.btnSelectFinishTime, &FluCalendarDateTimePicker::selectedDate, this, &RevelationRightSidebar::OnFinishDateSelected);
    connect(ui.btnSelectDeadline, &FluCalendarDateTimePicker::selectedDate, this, &RevelationRightSidebar::OnDeadlineDateSelected);
    connect(ui.btnSelectStartTime, &FluCalendarDateTimePicker::selectedTime, this, &RevelationRightSidebar::OnStartTimeSelected);
    connect(ui.btnSelectFinishTime, &FluCalendarDateTimePicker::selectedTime, this, &RevelationRightSidebar::OnFinishTimeSelected);
    connect(ui.btnSelectDeadline, &FluCalendarDateTimePicker::selectedTime, this, &RevelationRightSidebar::OnDeadlineTimeSelected);
}

void RevelationRightSidebar::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
}

void RevelationRightSidebar::hideEvent(QHideEvent* event)
{
    OnTaskItemEdited();
}

void RevelationRightSidebar::closeEvent(QCloseEvent* event)
{
    OnTaskItemEdited();
}

void RevelationRightSidebar::SetBtnAddToRoutineState(bool isRoutine)
{
    m_task.m_taskTag        = isRoutine ? TaskTag::Routine : TaskTag::None;
    QString        tooltip  = isRoutine ? tr("Remove from daily routine") : tr("Add to daily routine");
    FluAwesomeType iconType = isRoutine ? FluAwesomeType::FavoriteStarFill : FluAwesomeType::FavoriteStar;

    ui.btnAddToRoutine->setToolTip(tooltip);
    ui.btnAddToRoutine->setIcon(FluIconUtils::getFluentIcon(iconType));

    // sync tag label
    std::vector<QString> lutTags{tr("None"), tr("Routine"), tr("Inherited")};
    ui.cbTag->clear();
    ui.cbTag->addItem(lutTags[(int)m_task.m_taskTag]);
}

void RevelationRightSidebar::RefreshTaskData(const TaskPrototype& task)
{
    BlockSignals(true);

    ui.editTitle->setText(QString::fromStdString(task.m_title));
    ui.editDesc->setText(QString::fromStdString(task.m_desc));

    std::vector<QString> lutStatus{tr("None"), tr("Todo"), tr("Doing"), tr("Testing"), tr("Done")};
    std::vector<QString> lutTypes{tr("None"), tr("Bug"), tr("Feature"), tr("Test"), tr("UI")};
    std::vector<QString> lutTags{tr("None"), tr("Routine"), tr("Inherited")};

    ui.cbStatus->clear();
    ui.cbStatus->addItem(lutStatus[(int)task.m_taskStatus]);
    ui.cbType->setCurrentText(lutTypes[(int)task.m_taskType]);
    ui.cbTag->clear();
    ui.cbTag->addItem(lutTags[(int)task.m_taskTag]);
    SetBtnAddToRoutineState(task.m_taskTag == TaskTag::Routine);

    auto ConvertToQDate = [&](const std::string& timeString, QDate& date, QTime& time) {
        std::string yyyymmdd = m_timeFormatter->ParsePartDateTimeFromString(timeString, TimeMask::YMD);
        std::string hhmmss   = m_timeFormatter->ParsePartDateTimeFromString(timeString, TimeMask::HMS);
        if (yyyymmdd.empty())
        {
            return false;
        }
        else
        {
            date = QDate::fromString(QString::fromStdString(yyyymmdd), "yyyy-MM-dd");
            time = QTime::fromString(QString::fromStdString(hhmmss.empty() ? "00:00:00" : hhmmss), "hh:mm:ss");
            return date.isValid();
        }
    };

    QDate startDate, finishDate, deadlineDate;
    QTime startTime, finishTime, deadlineTime;
    bool  startTimeValid  = ConvertToQDate(task.m_startTime, startDate, startTime);
    bool  finishTimeValid = ConvertToQDate(task.m_finishTime, finishDate, finishTime);
    bool  deadlineValid   = ConvertToQDate(task.m_deadline, deadlineDate, deadlineTime);

    if (startTimeValid)
    {
        ui.btnSelectStartTime->setCurDate(startDate);
    }

    if (finishTimeValid)
    {
        ui.btnSelectFinishTime->setCurDate(finishDate);
    }
    ui.btnSelectFinishTime->setEnabled(finishTimeValid);

    if (deadlineValid)
    {
        ui.btnSelectDeadline->setCurDate(deadlineDate);
    }

    ui.btnSelectStartTime->setCurTime(startTime);
    ui.btnSelectFinishTime->setCurTime(finishTime);
    ui.btnSelectDeadline->setCurTime(deadlineTime);

    ui.labelCreateTime->setText(tr("Created: ") + QString::fromStdString(task.m_createTime));

    BlockSignals(false);
}

void RevelationRightSidebar::OnTaskReparenting(const TaskPrototype& task)
{
    if (this->parentWidget()->isVisible())
    {
        RefreshTaskData(task);
    }
}

void RevelationRightSidebar::OnTaskItemSelected(const TaskPrototype& task)
{
    if (m_taskValid && m_task != task)
    {
        // update previous task data first
        OnTaskItemEdited();
    }

    m_taskValid = true;
    m_task      = task;

    RefreshTaskData(task);

    this->parentWidget()->show();
}

void RevelationRightSidebar::OnTaskItemEdited()
{
    if (!m_taskValid)
    {
        return;
    }

    m_task.m_title = ui.editTitle->text().toStdString();
    m_task.m_desc  = ui.editDesc->toPlainText().toStdString();

    emit TaskItemEdited(m_task);
}

void RevelationRightSidebar::OnBtnAddToRoutineClicled()
{
    auto isRoutine = m_task.m_taskTag == TaskTag::Routine;
    SetBtnAddToRoutineState(!isRoutine);
}

void RevelationRightSidebar::OnBtnDeleteTaskItemClicked()
{
    m_taskValid = false;

    emit TaskItemDeleted(m_task);

    emit ui.btnHide->clicked();
}

void RevelationRightSidebar::OnCbTypeIndexChanged(int index)
{
    m_task.m_taskType = TaskType(index);
}

void RevelationRightSidebar::OnStartDateSelected(QDate date)
{
    std::string oriStartTime = m_timeFormatter->ParsePartDateTimeFromString(m_task.m_startTime, TimeMask::HMS);
    std::string newDate      = date.toString("yyyy-MM-dd").toStdString();
    std::string newDateTime  = newDate + " " + oriStartTime;

    m_task.m_startTime = newDateTime;
}

void RevelationRightSidebar::OnFinishDateSelected(QDate date)
{
    std::string oriStartTime = m_timeFormatter->ParsePartDateTimeFromString(m_task.m_finishTime, TimeMask::HMS);
    std::string newDate      = date.toString("yyyy-MM-dd").toStdString();
    std::string newDateTime  = newDate + " " + oriStartTime;

    m_task.m_finishTime = newDateTime;
}

void RevelationRightSidebar::OnDeadlineDateSelected(QDate date)
{
    std::string oriStartTime = m_timeFormatter->ParsePartDateTimeFromString(m_task.m_deadline, TimeMask::HMS);
    std::string newDate      = date.toString("yyyy-MM-dd").toStdString();
    std::string newDateTime  = newDate + " " + oriStartTime;

    m_task.m_deadline = newDateTime;
}

void RevelationRightSidebar::OnStartTimeSelected(QTime time)
{
    std::string oriStartDate = m_timeFormatter->ParsePartDateTimeFromString(m_task.m_startTime, TimeMask::YMD);
    std::string newTime      = time.toString("hh:mm:ss").toStdString();
    std::string newDateTime  = oriStartDate + " " + newTime;

    m_task.m_startTime = newDateTime;
}

void RevelationRightSidebar::OnFinishTimeSelected(QTime time)
{
    std::string oriStartDate = m_timeFormatter->ParsePartDateTimeFromString(m_task.m_finishTime, TimeMask::YMD);
    std::string newTime      = time.toString("hh:mm:ss").toStdString();
    std::string newDateTime  = oriStartDate + " " + newTime;

    m_task.m_finishTime = newDateTime;
}

void RevelationRightSidebar::OnDeadlineTimeSelected(QTime time)
{
    std::string oriStartDate = m_timeFormatter->ParsePartDateTimeFromString(m_task.m_deadline, TimeMask::YMD);
    std::string newTime      = time.toString("hh:mm:ss").toStdString();
    std::string newDateTime  = oriStartDate + " " + newTime;

    m_task.m_deadline = newDateTime;
}

void RevelationRightSidebar::BlockSignals(bool block)
{
    this->blockSignals(block);
    ui.btnAddToRoutine->blockSignals(block);
    ui.btnSelectStartTime->blockSignals(block);
    ui.btnSelectFinishTime->blockSignals(block);
    ui.btnSelectDeadline->blockSignals(block);
}
