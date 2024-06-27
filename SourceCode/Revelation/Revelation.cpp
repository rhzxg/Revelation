#include "Revelation.h"
#include "RevelationInterface.h"
#include <QLabel>
#include <QString>
#include <QMainWindow>

Revelation::Revelation(RevelationInterface* intf, QWidget* parent)
    : m_interface(intf), QWidget(parent)
{
    ui.setupUi(this);

    Initialize();
}

Revelation::~Revelation()
{
}

void Revelation::SetBottomBarVisible(bool visible)
{
    GetBottomBar()->SetVisible(visible);
}

void Revelation::mouseMoveEvent(QMouseEvent* event)
{
    if (event->pos().x() < 10 && event->pos().y() < 10)
    {
        GetSidebar()->SetVisible(true);
    }
}

void Revelation::mouseReleaseEvent(QMouseEvent* event)
{
    if (GetSidebar()->IsVisible())
    {
        GetSidebar()->SetVisible(false);
    }
}

void Revelation::resizeEvent(QResizeEvent* event)
{
    // sidebar
    emit CentralWidgetMoved(mapToGlobal(this->pos()), this->size());
    emit CentralWidgetResized(this->size());
}

void Revelation::Initialize()
{
    InitWidget();
    InitSignalSlots();
}

void Revelation::InitWidget()
{
    this->setMouseTracking(true);

    auto iconPath   = m_interface->GetResourcePath() / "images" / "icon.ico";
    auto mainWindow = (QMainWindow*)m_interface->GetMainWindow();
    if (std::filesystem::exists(iconPath) && nullptr != mainWindow)
    {
        QPixmap pixmap(QString::fromStdString(iconPath.u8string()));
        mainWindow->setWindowIcon(pixmap);
    }

    std::vector<QLabel*>  labels{ui.labelTitleTodo, ui.labelTitleProgramming, ui.labelTitleTesting, ui.labelTitleDone};
    std::vector<QWidget*> widgets{ui.taskWidgetTodo, ui.taskWidgetProgramming, ui.taskWidgetTesting, ui.taskWidgetDone};
    std::vector<QString>  colors{"#CAF0FC", "#ADE8F6", "#90E0EF", "#48CAE4"};
    for (int i = 0; i < 4; ++i)
    {
        QLabel*  label  = labels[i];
        QWidget* widget = widgets[i];

        QString labelStyle  = QString("QLabel {background-color:%1; color:#FFFFFF; border-radius:5px; font-size: 16px;}").arg(colors[i]);
        QString widgetStyle = QString("QWidget {background-color:%1; color:#FFFFFF; border-radius:5px;}").arg(colors[i]);

        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet(labelStyle);

        widget->setStyleSheet(widgetStyle);
    }
}

void Revelation::InitSignalSlots()
{
}

RevelationLeftSidebar* Revelation::GetSidebar()
{
    if (nullptr == m_sidebar)
    {
        m_sidebar      = new RevelationLeftSidebar(this);
        QPoint basePos = mapToGlobal(this->pos());
        int    x       = basePos.x() + 20;
        int    y       = basePos.y() + 15;
        m_sidebar->move(x, y);
        m_sidebar->resize(m_sidebar->width(), this->height() - 30);

        connect(this, SIGNAL(CentralWidgetMoved(const QPoint&, const QSize&)), m_sidebar, SLOT(OnCentralWidgetMoved(const QPoint&)));
        connect(this, SIGNAL(CentralWidgetResized(const QSize&)), m_sidebar, SLOT(OnCentralWidgetResized(const QSize&)));
    }
    return m_sidebar;
}

RevelationBottomBar* Revelation::GetBottomBar()
{
    if (nullptr == m_bottomBar)
    {
        m_bottomBar    = new RevelationBottomBar(this);
        QPoint basePos = mapToGlobal(this->pos());
        int    x       = basePos.x() + this->size().width() / 2 - m_bottomBar->size().width() / 2;
        int    y       = basePos.y() + this->size().height() / 5 * 4;
        y              = std::max(y, basePos.y() + this->size().height() - m_bottomBar->height() - 50);
        m_bottomBar->move(x, y);

        connect(this, SIGNAL(CentralWidgetMoved(const QPoint&, const QSize&)), m_bottomBar, SLOT(OnCentralWidgetMoved(const QPoint&, const QSize&)));
    }
    return m_bottomBar;
}
