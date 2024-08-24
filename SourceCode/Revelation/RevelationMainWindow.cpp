#include "RevelationMainWindow.h"
#include "Revelation.h"
#include <FramelessHelper/Core/framelessmanager.h>
#include <FramelessHelper/Widgets/framelesswidgetshelper.h>
#include <FramelessHelper/Widgets/standardsystembutton.h>
#include <FramelessHelper/Widgets/standardtitlebar.h>
#include "FluVNavigationIconTextItem.h"

RevelationMainWindow::RevelationMainWindow(QWidget* parent /*= nullptr*/)
    : FluFrameLessWidget(parent)
{
    Initialize();
}

RevelationMainWindow::~RevelationMainWindow()
{
}

void RevelationMainWindow::AlignToScreenCenter()
{
    this->resize(1050, 500);
    QScreen* screen = QGuiApplication::primaryScreen();
    if (nullptr != screen)
    {
        QRect screenGeometry = screen->geometry();
        int   x              = (screenGeometry.width() - this->width()) / 2;
        int   y              = (screenGeometry.height() - this->height()) / 2;
        this->move(screenGeometry.topLeft() + QPoint(x, y));
    }
}

void RevelationMainWindow::AddStackedWidget(QWidget* widget, const QString& name, const QIcon& icon, Qt::AlignmentFlag pos/* = Qt::AlignCenter*/)
{
    if (nullptr == widget)
    {
        return;
    }

    FluVNavigationIconTextItem* item = new FluVNavigationIconTextItem(icon, name, this);
    if (pos == Qt::AlignTop)
    {
        m_navigationView->addItemToTopLayout(item);
    }
    else if (pos == Qt::AlignCenter)
    {
        m_navigationView->addItemToMidLayout(item);
    }
    else
    {
        m_navigationView->addItemToBottomLayout(item);
    }

    m_stackedLayout->addWidget(name, widget);
    connect(item, &FluVNavigationIconTextItem::itemClicked, [=]() {
        m_stackedLayout->setCurrentWidget(name);
    });

    if (name == "Revelation")
    {
        item->clearAllItemsSelectState();
        item->updateSelected(true);
        item->updateAllItemsStyleSheet();
        m_stackedLayout->setCurrentWidget(name);
    }
}

void RevelationMainWindow::AddStackedWidget(QWidget* widget, const QString& name, FluAwesomeType type, Qt::AlignmentFlag pos /* = Qt::AlignCenter*/)
{
    if (nullptr == widget)
    {
        return;
    }

    FluVNavigationIconTextItem* item = new FluVNavigationIconTextItem(type, name, this);
    if (pos == Qt::AlignTop)
    {
        m_navigationView->addItemToTopLayout(item);
    }
    else if (pos == Qt::AlignCenter)
    {
        m_navigationView->addItemToMidLayout(item);
    }
    else
    {
        m_navigationView->addItemToBottomLayout(item);
    }

    m_stackedLayout->addWidget(name, widget);
    connect(item, &FluVNavigationIconTextItem::itemClicked, [=]() {
        m_stackedLayout->setCurrentWidget(name);
    });

    if (name == "Revelation")
    {
        item->clearAllItemsSelectState();
        item->updateSelected(true);
        item->updateAllItemsStyleSheet();
        m_stackedLayout->setCurrentWidget(name);
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

    m_navigationView = new FluVNavigationView(this);
    m_stackedLayout  = new FluStackedLayout;

    m_contentLayout->addWidget(m_navigationView);
    m_contentLayout->addLayout(m_stackedLayout, 1);

    // collapse
    m_navigationView->onMenuItemClicked();
}
