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

void RevelationListItem::SetTaskData(TaskPrototype task)
{
    ui.labelTitle->setText(QString::fromStdString(task.m_title));
    ui.labelDeadline->setText(QString::fromStdString(task.m_deadline));
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

    ui.frame->setStyleSheet("QFrame { background: #F0F0F0; border-radius: 8px 8px 8px 8px; }");
}

void RevelationListItem::InitSignalSlots()
{
}
