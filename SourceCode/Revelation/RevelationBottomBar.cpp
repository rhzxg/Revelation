#include "RevelationBottomBar.h"

RevelationBottomBar::RevelationBottomBar(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    Initialize();
}

RevelationBottomBar::~RevelationBottomBar()
{
}

void RevelationBottomBar::SetVisible(bool visible)
{
    visible ? this->show() : this->hide();
}

bool RevelationBottomBar::IsVisible()
{
    return this->isVisible();
}

void RevelationBottomBar::Initialize()
{
    InitWidget();
    InitSignalSlots();
}

void RevelationBottomBar::InitWidget()
{
    this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    this->setWindowFlag(Qt::WindowDoesNotAcceptFocus);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setWindowFlag(Qt::Tool);
    
    this->setAttribute(Qt::WA_TranslucentBackground);

    ui.frame->setStyleSheet("QFrame { background: #FB637E; border-radius: 10px 10px 10px 10px; }");
}

void RevelationBottomBar::InitSignalSlots()
{
}

void RevelationBottomBar::OnCentralWidgetMoved(const QPoint& point, const QSize& size)
{
    int x = point.x() + size.width() / 2 - this->size().width() / 2;
    int y = point.y() + size.height() / 5 * 4;
    y     = std::max(y, point.y() + size.height() - this->height() - 50);
    this->move(x, y);
}
