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

void RevelationBottomBar::Initialize()
{
    this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    this->setWindowFlag(Qt::WindowDoesNotAcceptFocus);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setWindowFlag(Qt::Tool);

    InitWidget();
    InitSignalSlots();
}

void RevelationBottomBar::InitWidget()
{
}

void RevelationBottomBar::InitSignalSlots()
{
}

void RevelationBottomBar::OnCentralWidgetChanged(const QPoint& point)
{
    this->move(point);
}
