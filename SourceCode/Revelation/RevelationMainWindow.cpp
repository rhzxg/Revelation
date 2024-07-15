#include "RevelationMainWindow.h"
#include "Revelation.h"
#include <FramelessHelper/Core/framelessmanager.h>
#include <FramelessHelper/Widgets/framelesswidgetshelper.h>
#include <FramelessHelper/Widgets/standardsystembutton.h>
#include <FramelessHelper/Widgets/standardtitlebar.h>

RevelationMainWindow::RevelationMainWindow(QWidget* parent /*= nullptr*/)
    : FluFrameLessWidget(parent)
{
    Initialize();
}

RevelationMainWindow::~RevelationMainWindow()
{
}

void RevelationMainWindow::SetCentralWidget(Revelation* widget)
{
    if (nullptr == widget)
    {
        return;
    }

    m_revelationWidget = widget;
    m_contentLayout->addWidget(widget);
    this->resize(widget->size());
    
    QScreen* screen = QGuiApplication::primaryScreen();
    if (nullptr != screen)
    {
        QRect screenGeometry = screen->geometry();
        int   x              = (screenGeometry.width() - this->width()) / 2;
        int   y              = (screenGeometry.height() - this->height()) / 2;
        this->move(screenGeometry.topLeft() + QPoint(x, y));
    }
}

void RevelationMainWindow::Initialize()
{
    InitWidget();
}

void RevelationMainWindow::InitWidget()
{
    this->setMinimumSize(600, 400);
    this->m_titleBar->chromePalette()->setTitleBarActiveBackgroundColor(Qt::transparent);
    this->m_titleBar->chromePalette()->setTitleBarInactiveBackgroundColor(Qt::transparent);
    this->m_titleBar->chromePalette()->setTitleBarActiveForegroundColor(Qt::black);
    this->m_titleBar->chromePalette()->setTitleBarInactiveForegroundColor(Qt::black);
    this->m_titleBar->setFixedHeight(30);
}
