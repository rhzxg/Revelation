#include "CommonWidgetInterface.h"

CommonWidgetInterface::CommonWidgetInterface(RevelationMainWindow* mainWindow)
    : m_mainWindow(mainWindow)
{
}

CommonWidgetInterface::~CommonWidgetInterface()
{
}

void CommonWidgetInterface::AddStackedWidget(QWidget* widget, const QString& name, const QIcon& icon, Qt::AlignmentFlag pos /*= Qt::AlignCenter*/)
{
    m_mainWindow->AddStackedWidget(widget, name, icon, pos);
}

void CommonWidgetInterface::AddStackedWidget(QWidget* widget, const QString& name, FluAwesomeType type, Qt::AlignmentFlag pos /*= Qt::AlignCenter*/)
{
    m_mainWindow->AddStackedWidget(widget, name, type, pos);
}
