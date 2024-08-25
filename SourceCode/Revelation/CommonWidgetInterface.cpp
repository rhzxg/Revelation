#include "CommonWidgetInterface.h"
#include "RevelationConfig.h"

CommonWidgetInterface::CommonWidgetInterface(RevelationMainWindow* mainWindow)
    : m_mainWindow(mainWindow)
{
    Initialize();
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

void CommonWidgetInterface::AddSettingItem(QWidget* widget)
{
    m_config->AddSettingItem(widget);
}

void CommonWidgetInterface::SetProgressBarVisibility(bool visible, int value /*= 0*/)
{
    // threads supported
    if (visible)
    {
        QPoint pt(m_mainWindow->x() + m_mainWindow->width() / 2 - 100, m_mainWindow->y() + m_mainWindow->height() / 2);
        m_progressBar->move(pt);

        QMetaObject::invokeMethod(m_progressBar, "show", Qt::QueuedConnection);
        QMetaObject::invokeMethod(m_progressBar, "setValue", Qt::QueuedConnection, Q_ARG(int, value));
    }
    else
    {
        QMetaObject::invokeMethod(m_progressBar, "hide", Qt::QueuedConnection);
    }
}

void CommonWidgetInterface::Initialize()
{
    InitWidget();
}

void CommonWidgetInterface::InitWidget()
{
    m_config = new RevelationConfig(m_mainWindow);
    AddStackedWidget(m_config, QObject::tr("Settings"), FluAwesomeType::Settings, Qt::AlignBottom);

    m_progressBar = new FluProgressBar(m_mainWindow);
    m_progressBar->setWindowFlag(Qt::FramelessWindowHint);
    m_progressBar->setFixedWidth(200);
    m_progressBar->setWindowModality(Qt::ApplicationModal);
    m_progressBar->hide();
}
