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

void RevelationListItem::SetTaskData(const TaskPrototype& task)
{
    m_task = task;

    QString      fullText = QString::fromStdString(task.m_title);
    QFontMetrics metrics(ui.labelTitle->font());
    int          maxWidth   = this->width() - 20;
    QString      elidedText = metrics.elidedText(fullText, Qt::ElideRight, maxWidth);

    ui.labelTitle->setText(elidedText);
    ui.labelDeadline->setText(QString::fromStdString(task.m_deadline));
}

void RevelationListItem::ResizeWidget(const QSize& size)
{
    QString      fullText = QString::fromStdString(m_task.m_title);
    QFontMetrics metrics(ui.labelTitle->font());
    int          maxWidth   = size.width() - 20;
    QString      elidedText = metrics.elidedText(fullText, Qt::ElideRight, maxWidth);

    ui.labelTitle->setText(elidedText);
    this->setFixedSize(size);
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
