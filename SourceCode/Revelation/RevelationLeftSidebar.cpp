#include "RevelationLeftSidebar.h"
#include <QPropertyAnimation>

RevelationLeftSidebar::RevelationLeftSidebar(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    Initialize();
}

RevelationLeftSidebar::~RevelationLeftSidebar()
{
}

void RevelationLeftSidebar::SetVisible(bool visible)
{
    visible ? this->show() : this->hide();
}

bool RevelationLeftSidebar::IsVisible()
{
    return this->isVisible();
}

void RevelationLeftSidebar::Initialize()
{
    InitWidget();
    InitSignalSlots();
}

void RevelationLeftSidebar::InitWidget()
{
    this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    this->setWindowFlag(Qt::WindowDoesNotAcceptFocus);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setWindowFlag(Qt::Tool);

    this->setAttribute(Qt::WA_TranslucentBackground);

    ui.frame->setStyleSheet("QFrame { background: #f8e16c; border-radius: 10px 10px 10px 10px; }");
}

void RevelationLeftSidebar::InitSignalSlots()
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

void RevelationLeftSidebar::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(this, "windowOpacity");
    opacityAnimation->setDuration(100);
    opacityAnimation->setStartValue(0.0);
    opacityAnimation->setEndValue(1.0);
    opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void RevelationLeftSidebar::OnCentralWidgetMoved(const QPoint& point)
{
    int x = point.x() + 20;
    int y = point.y() + 15;
    this->move(x, y);
}

void RevelationLeftSidebar::OnCentralWidgetResized(const QSize& size)
{
    this->resize(this->width(), size.height() - 30);
}
