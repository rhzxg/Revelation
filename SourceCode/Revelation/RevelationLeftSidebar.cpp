#include "RevelationLeftSidebar.h"
#include "FluIconUtils.h"

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
    ui.btnHide->setStyleSheet("QPushButton { background: transparent; border: none; }");

    ui.btnHide->setFixedSize(15, 15);
    ui.btnHide->setIcon(FluIconUtils::getFluentIcon(FluAwesomeType::CaretLeftSolid8));
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
