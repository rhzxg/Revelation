#include "RevelationRightSidebar.h"
#include <QPropertyAnimation>

RevelationRightSidebar::RevelationRightSidebar(IRevelationInterface* intf, QWidget* parent)
    : RevelationSidebar(intf, parent)
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
    this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setWindowFlag(Qt::Tool);

    this->setAttribute(Qt::WA_TranslucentBackground);

    ui.frame->setStyleSheet("QFrame { background: #F8E16C; border-radius: 8px 8px 8px 8px; }");
    ui.editTitle->setStyleSheet("QLineEdit { background-color: #F0F0F0; border-radius: 4px; }");
    ui.editDesc->setStyleSheet("QTextEdit { background-color: #F0F0F0; }");

    ui.labelCreateTime->setAlignment(Qt::AlignCenter);
}

void RevelationRightSidebar::InitSignalSlots()
{
    connect(ui.btnHide, &QPushButton::clicked, this, [&]() {
        QPropertyAnimation* animation = new QPropertyAnimation(this, "windowOpacity");
        animation->setDuration(100);
        animation->setStartValue(windowOpacity());
        animation->setEndValue(0.0);
        animation->start(QAbstractAnimation::DeleteWhenStopped);

        connect(animation, &QPropertyAnimation::finished, this, &QWidget::hide);
    });
}

void RevelationRightSidebar::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(this, "windowOpacity");
    opacityAnimation->setDuration(100);
    opacityAnimation->setStartValue(0.0);
    opacityAnimation->setEndValue(1.0);
    opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void RevelationRightSidebar::OnCentralWidgetMoved(const QPoint& point, const QSize& size)
{
    int x = point.x() + size.width() - this->width() - 18;
    int y = point.y() + 15;
    this->move(x, y);
}

void RevelationRightSidebar::OnCentralWidgetResized(const QSize& size)
{
    this->resize(this->width(), size.height() - 30);
}

void RevelationRightSidebar::OnTaskItemSelected(TaskPrototype task)
{
    this->show();

    ui.editTitle->setText(QString::fromStdString(task.m_title));
    ui.editDesc->setText(QString::fromStdString(task.m_desc));

    std::vector<QString> lutStatus{tr("None"), tr("Todo"), tr("Doing"), tr("Testing"), tr("Done")};
    std::vector<QString> lutTypes{tr("None"), tr("Bug"), tr("Feature"), tr("Test"), tr("UI")};
    std::vector<QString> lutTags{tr("None"), tr("Routine"), tr("Inherited")};

    ui.labelStatus->setText(lutStatus[(int)task.m_taskStatus]);
    ui.labelType->setText(lutTypes[(int)task.m_taskType]);
    ui.labelTag->setText(lutTags[(int)task.m_taskTag]);

    ui.btnSlectStartTime->setText(task.m_startTime.empty() ? tr("N/A") : QString::fromStdString(task.m_startTime));
    ui.btnSlectFinishTime->setText(task.m_finishTime.empty() ? tr("N/A") : QString::fromStdString(task.m_finishTime));
    ui.btnSlectDeadline->setText(task.m_deadline.empty() ? tr("N/A") : QString::fromStdString(task.m_deadline));

    ui.labelCreateTime->setText(tr("Created: ") + QString::fromStdString(task.m_createTime));
}
