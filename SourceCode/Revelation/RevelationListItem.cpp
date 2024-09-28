#include "RevelationListItem.h"
#include "FluThemeUtils.h"
#include "FluStyleSheetUitls.h"

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

void RevelationListItem::SetMouseHoverd(bool hoverd)
{
    QString color = "", qss = "";
    if (m_lightMode)
    {
        color = hoverd ? "#F7F7F7" : "#F0F0F0";
        qss   = "QFrame { background: %1; border-radius: 8px 8px 8px 8px; }";
        qss   = qss.arg(color);
    }
    else
    {
        color = hoverd ? "#404040" : "#333333";
        qss   = "QFrame { background: %1; border-radius: 8px 8px 8px 8px; color: #FFFFFF}";
        qss   = qss.arg(color);
    }

    ui.frame->setStyleSheet(qss);
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

    m_lightMode   = FluThemeUtils::isLightTheme();
    QString color = m_lightMode ? "#F0F0F0" : "#333333";
    QString qss   = "QFrame { background: %1; border-radius: 8px 8px 8px 8px; }";
    ui.frame->setStyleSheet(qss.arg(color));
}

void RevelationListItem::InitSignalSlots()
{
}
