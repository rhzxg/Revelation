#include "RevelationLeftSidebar.h"
#include <QPropertyAnimation>

RevelationLeftSidebar::RevelationLeftSidebar(IRevelationInterface* intf, QWidget* parent)
    : RevelationSidebar(intf, parent)
{
    ui.setupUi(this);

    Initialize();
}

RevelationLeftSidebar::~RevelationLeftSidebar()
{
}

void RevelationLeftSidebar::Initialize()
{
    InitWidget();
    InitSignalSlots();
}

void RevelationLeftSidebar::InitWidget()
{
    ui.frame->setObjectName("RevelationFrame");
    ui.frame->setStyleSheet("QFrame#RevelationFrame { background: #F0F0F0; border-radius: 7px; }");
}

void RevelationLeftSidebar::InitSignalSlots()
{
    connect(ui.btnHide, &QPushButton::clicked, this, [&]() {
        parentWidget()->hide();
    });
}

void RevelationLeftSidebar::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
}
