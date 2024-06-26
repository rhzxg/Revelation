#include "Revelation.h"
#include "RevelationInterface.h"
#include <QLabel>
#include <QString>
#include <QPainter>
#include <QPushButton>
#include <QGridLayout>

Revelation::Revelation(RevelationInterface* intf, QWidget* parent)
    : m_interface(intf), QWidget(parent)
{
    ui.setupUi(this);

    Initialize();
}

Revelation::~Revelation()
{
}

void Revelation::ShowBottomBar()
{
    GetBottomBar()->show();
}

void Revelation::mouseMoveEvent(QMouseEvent* event)
{
    if (event->pos().x() < 10 && event->pos().y() < 10)
    {
        GetSidebar()->show();
    }
}

void Revelation::resizeEvent(QResizeEvent* event)
{
    emit CentralWidgetMoved(mapToGlobal(this->pos()));
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

    auto iconPath = m_interface->GetResourcePath() / "images" / "icon.ico";
    if (std::filesystem::exists(iconPath))
    {
        QPixmap pixmap(QString::fromStdString(iconPath.u8string()));
        this->setWindowIcon(QIcon(pixmap));
    }

    std::vector<QLabel*>  labels{ui.labelTitleTodo, ui.labelTitleProgramming, ui.labelTitleTesting, ui.labelTitleDone};
    std::vector<QWidget*> widgets{ui.taskWidgetTodo, ui.taskWidgetProgramming, ui.taskWidgetTesting, ui.taskWidgetDone};
    std::vector<QString>  colors{"#8DE2E2", "#38E6EB", "#33CED8", "#29A1B4"};
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

RevelationSidebar* Revelation::GetSidebar()
{
    if (nullptr == m_sidebar)
    {
        m_sidebar = new RevelationSidebar(this);
        m_sidebar->move(mapToGlobal(this->pos()));
        m_sidebar->resize(m_sidebar->width(), this->height());

        connect(this, SIGNAL(CentralWidgetMoved(const QPoint&)), m_sidebar, SLOT(OnCentralWidgetMoved(const QPoint&)));
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
        int    x       = basePos.x() / 2 + this->size().width() / 2 - m_bottomBar->size().width() / 2;
        int    y       = basePos.y() + this->size().height() / 4 * 3;
        m_bottomBar->move(x, y);
    }
    return m_bottomBar;
}
