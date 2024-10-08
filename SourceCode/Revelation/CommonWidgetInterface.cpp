#include "CommonWidgetInterface.h"
#include "RevelationConfig.h"

CommonWidgetInterface::CommonWidgetInterface(RevelationMainWindow* mainWindow)
    : m_mainWindow(mainWindow)
{
    Initialize();
}

CommonWidgetInterface::~CommonWidgetInterface()
{
    m_progressBar->hide();
    m_progressBar->deleteLater();
}

void CommonWidgetInterface::AddStackedWidget(QWidget* widget, const QString& name, const QIcon& icon, Qt::AlignmentFlag pos /*= Qt::AlignCenter*/)
{
    m_mainWindow->AddStackedWidget(widget, name, icon, pos);
}

void CommonWidgetInterface::AddStackedWidget(QWidget* widget, const QString& name, FluAwesomeType type, Qt::AlignmentFlag pos /*= Qt::AlignCenter*/)
{
    m_mainWindow->AddStackedWidget(widget, name, type, pos);
}

void CommonWidgetInterface::AddSettingsItem(QWidget* widget)
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

void CommonWidgetInterface::DisposeMemoryBindings()
{
    m_config->RemoveSettingsItems();
}

void CommonWidgetInterface::SetInterface(IRevelationInterface* intf)
{
    m_interface = intf;
}

void CommonWidgetInterface::InitializeConfig()
{
    m_config = new RevelationConfig(m_interface, m_mainWindow);
    AddStackedWidget(m_config, QObject::tr("Settings"), FluAwesomeType::Settings, Qt::AlignBottom);
}

void CommonWidgetInterface::Initialize()
{
    InitWidget();
}

void CommonWidgetInterface::InitWidget()
{
    m_progressBar = new FluProgressBar;
    m_progressBar->setWindowFlag(Qt::FramelessWindowHint);
    m_progressBar->setFixedWidth(200);
    m_progressBar->setWindowModality(Qt::ApplicationModal);
    m_progressBar->hide();
}
