#include "NavigationViewInterface.h"

NavigationViewInterface::NavigationViewInterface(RevelationMainWindow* mainWindow)
    : m_mainWindow(mainWindow)
{
}

NavigationViewInterface::~NavigationViewInterface()
{
}

void NavigationViewInterface::AddStackedWidget(QWidget* widget, const QString& name, const QIcon& icon, Qt::AlignmentFlag pos /*= Qt::AlignCenter*/)
{
    m_mainWindow->AddStackedWidget(widget, name, icon, pos);
}

void NavigationViewInterface::AddStackedWidget(QWidget* widget, const QString& name, FluAwesomeType type, Qt::AlignmentFlag pos /*= Qt::AlignCenter*/)
{
    m_mainWindow->AddStackedWidget(widget, name, type, pos);
}
