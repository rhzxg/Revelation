#include "RevelationLeftSidebar.h"

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
