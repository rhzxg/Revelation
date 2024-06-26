#include "RevelationSidebar.h"

RevelationSidebar::RevelationSidebar(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    Initialize();
}

RevelationSidebar::~RevelationSidebar()
{
}

void RevelationSidebar::Initialize()
{
    InitWidget();
    InitSignalSlots();
}

void RevelationSidebar::InitWidget()
{
    this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    this->setWindowFlag(Qt::WindowDoesNotAcceptFocus);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setWindowFlag(Qt::Tool);
}

void RevelationSidebar::InitSignalSlots()
{
}

void RevelationSidebar::OnCentralWidgetMoved(const QPoint& point)
{
    this->move(point);
}

void RevelationSidebar::OnCentralWidgetResized(const QSize& size)
{
    this->resize(this->width(), size.height());
}
