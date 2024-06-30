#include "RevelationListItem.h"

RevelationListItem::RevelationListItem(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    Initialize();
}

RevelationListItem::~RevelationListItem()
{
}

void RevelationListItem::SetItemTaskData(TaskPrototype task)
{
    m_task = task;

    ui.labelTitle->setText(QString::fromStdString(task.m_title));
    ui.labelDeadline->setText(QString::fromStdString(task.m_deadline));
}

TaskPrototype RevelationListItem::GetItemTaskData()
{
    return m_task;
}

void RevelationListItem::Initialize()
{
    InitWidget();
    InitSignalSlots();
}

void RevelationListItem::InitWidget()
{
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    ui.frame->setStyleSheet("QFrame { background: #f0f0f0; border-radius: 8px 8px 8px 8px; }");
}

void RevelationListItem::InitSignalSlots()
{
}
