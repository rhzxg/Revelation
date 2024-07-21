#include "RevelationRightSidebar.h"
#include <QPropertyAnimation>
#include <QMouseEvent>
#include "Utility/IUtilityInterface.h"

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
    InitWidget();
    InitSignalSlots();
}

void RevelationRightSidebar::InitWidget()
{
    ui.frame->setObjectName("RevelationFrame");
    ui.frame->setStyleSheet("QFrame#RevelationFrame { background: #F0F0F0; border-radius: 7px; }");

    ui.labelCreateTime->setAlignment(Qt::AlignCenter);
}

void RevelationRightSidebar::InitSignalSlots()
{
    connect(ui.btnHide, &QPushButton::clicked, this, [&]() {
        parentWidget()->hide();
    });

    connect(ui.btnDelete, &QPushButton::clicked, this, &RevelationRightSidebar::OnBtnDeleteTaskItemClicked);

    connect(ui.btnSelectStartTime, &FluCalendarDatePicker::selectedDate, this, &RevelationRightSidebar::OnBtnSelectStartTimeClicked);
    connect(ui.btnSelectFinishTime, &FluCalendarDatePicker::selectedDate, this, &RevelationRightSidebar::OnBtnSelectFinishTimeClicked);
    connect(ui.btnSelectDeadline, &FluCalendarDatePicker::selectedDate, this, &RevelationRightSidebar::OnBtnSelectDeadlineClicked);
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

void RevelationRightSidebar::OnTaskItemSelected(const TaskPrototype& task)
{
    if (m_taskValid && m_task != task)
    {
        // update previous task data first
        OnTaskItemEdited();
    }

    BlockSignals(true);

    m_taskValid = true;
    m_task      = task;

    ui.editTitle->setText(QString::fromStdString(task.m_title));
    ui.editDesc->setText(QString::fromStdString(task.m_desc));

    std::vector<QString> lutStatus{tr("None"), tr("Todo"), tr("Doing"), tr("Testing"), tr("Done")};
    std::vector<QString> lutTypes{tr("None"), tr("Bug"), tr("Feature"), tr("Test"), tr("UI")};
    std::vector<QString> lutTags{tr("None"), tr("Routine"), tr("Inherited")};

    ui.labelStatus->setText(lutStatus[(int)task.m_taskStatus]);
    ui.labelType->setText(lutTypes[(int)task.m_taskType]);
    ui.labelTag->setText(lutTags[(int)task.m_taskTag]);

    auto ConvertToQDate = [&](const std::string& timeString, QDate& date, QTime& time) {
        auto        timeFormatter = m_interface->GetInterfaceById<IUtilityInterface>("Utility")->GetDateTimeFormatter();
        std::string yyyymmdd      = timeFormatter->ParsePartDateTimeFromString(timeString, TimeMask::YMD);
        std::string hhmmss        = timeFormatter->ParsePartDateTimeFromString(timeString, TimeMask::HMS);
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

    ui.btnSelectFinishTime->setCurDate(finishDate);
    ui.btnSelectFinishTime->setEnabled(finishTimeValid);

    if (deadlineValid)
    {
        ui.btnSelectDeadline->setCurDate(deadlineDate);
    }

    ui.labelCreateTime->setText(tr("Created: ") + QString::fromStdString(task.m_createTime));

    BlockSignals(false);
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

    // change render cache, memory cache, and database
    emit TaskItemEdited(m_task);
}

void RevelationRightSidebar::OnBtnDeleteTaskItemClicked()
{
    m_taskValid = false;

    emit TaskItemDeleted(m_task);

    emit ui.btnHide->clicked();
}

void RevelationRightSidebar::OnBtnSelectStartTimeClicked(QDate date)
{
    m_task.m_startTime = date.toString("yyyy-MM-dd").toStdString();

    // hour minute second?
    // change render cache, memory cache, and database
    emit TaskItemEdited(m_task);
}

void RevelationRightSidebar::OnBtnSelectFinishTimeClicked(QDate date)
{
    m_task.m_finishTime = date.toString("yyyy-MM-dd").toStdString();

    // hour minute second?
    // change render cache, memory cache, and database
    emit TaskItemEdited(m_task);
}

void RevelationRightSidebar::OnBtnSelectDeadlineClicked(QDate date)
{
    m_task.m_deadline = date.toString("yyyy-MM-dd").toStdString();

    // hour minute second?
    // change render cache, memory cache, and database
    emit TaskItemEdited(m_task);
}

void RevelationRightSidebar::BlockSignals(bool block)
{
    this->blockSignals(block);
    ui.btnSelectStartTime->blockSignals(block);
    ui.btnSelectFinishTime->blockSignals(block);
    ui.btnSelectDeadline->blockSignals(block);
}
