#include "RevelationMainWindow.h"
#include "Revelation.h"

RevelationMainWindow::RevelationMainWindow(QWidget* parent /*= nullptr*/)
    : QMainWindow(parent)
{
    Initialize();
}

RevelationMainWindow::~RevelationMainWindow()
{
}

void RevelationMainWindow::Initialize()
{
}

void RevelationMainWindow::InitWidget()
{
}

void RevelationMainWindow::moveEvent(QMoveEvent* event)
{
    Revelation* revelation = qobject_cast<Revelation*>(centralWidget());
    if (nullptr != revelation)
    {
        emit revelation->CentralWidgetMoved(mapToGlobal(revelation->pos()), revelation->size());
    }
}
