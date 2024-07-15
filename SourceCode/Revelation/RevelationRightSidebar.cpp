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

void RevelationRightSidebar::OnTaskItemSelected(TaskPrototype task)
{
    // update previous task data first
    OnTaskItemEdited();
    m_taskValid = true;

    this->blockSignals(true);

    m_task = task;

    ui.editTitle->setText(QString::fromStdString(task.m_title));
    ui.editDesc->setText(QString::fromStdString(task.m_desc));

    std::vector<QString> lutStatus{tr("None"), tr("Todo"), tr("Doing"), tr("Testing"), tr("Done")};
    std::vector<QString> lutTypes{tr("None"), tr("Bug"), tr("Feature"), tr("Test"), tr("UI")};
    std::vector<QString> lutTags{tr("None"), tr("Routine"), tr("Inherited")};

    ui.labelStatus->setText(lutStatus[(int)task.m_taskStatus]);
    ui.labelType->setText(lutTypes[(int)task.m_taskType]);
    ui.labelTag->setText(lutTags[(int)task.m_taskTag]);

    auto ConvertToQDate = [&](const std::string& timeString) {
        auto timeFormatter = m_interface->GetInterfaceById<IUtilityInterface>("Utility")->GetDateTimeFormatter();
        std::string yyyymmdd      = timeFormatter->ParsePartDateTimeFromString(timeString, TimeMask::YMD);
        if (yyyymmdd.empty())
        {
            return QDate::currentDate();
        }
        else
        {
            return QDate::fromString(QString::fromStdString(yyyymmdd), "yyyy-MM-dd");
        }
    };

    ui.btnSelectStartTime->setCurDate(ConvertToQDate(task.m_startTime));
    ui.btnSelectFinishTime->setCurDate(ConvertToQDate(task.m_finishTime));
    ui.btnSelectDeadline->setCurDate(ConvertToQDate(task.m_deadline));

    ui.labelCreateTime->setText(tr("Created: ") + QString::fromStdString(task.m_createTime));

    this->blockSignals(false);
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

    // TODO: other params

    // change render cache, memory cache, and database
    emit TaskItemEdited(m_task);
}

void RevelationRightSidebar::OnBtnDeleteTaskItemClicked()
{
    m_taskValid = false;

    emit TaskItemDeleted(m_task);

    emit ui.btnHide->clicked();
}
