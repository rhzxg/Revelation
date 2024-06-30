#include "Revelation.h"
#include "IRevelationInterface.h"
#include "RevelationListView.h"
#include "RevelationLeftSidebar.h"
#include "RevelationRightSidebar.h"
#include "RevelationBottomBar.h"
#include <QLabel>
#include <QString>
#include <QMainWindow>
#include <QMessageBox>

Revelation::Revelation(IRevelationInterface* intf, QWidget* parent)
    : m_interface(intf), QWidget(parent)
{
    ui.setupUi(this);

    Initialize();
}

Revelation::~Revelation()
{
}

void Revelation::mouseMoveEvent(QMouseEvent* event)
{
    if (event->pos().x() < 10 && event->pos().y() < 10)
    {
        GetSidebar(RevelationSidebar::Left)->SetVisible(true);

        // test
        GetSidebar(RevelationSidebar::Right)->SetVisible(true);
    }
}

void Revelation::resizeEvent(QResizeEvent* event)
{
    // sidebar
    emit CentralWidgetMoved(mapToGlobal(this->pos()), this->size());
    emit CentralWidgetResized(this->size());
}

void Revelation::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    GetSidebar(RevelationSidebar::Bottom)->SetVisible(true);
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

    std::vector<QLabel*>     labels{ui.labelTitleTodo, ui.labelTitleDoing, ui.labelTitleTesting, ui.labelTitleDone};
    std::vector<QWidget*>    widgets{ui.taskWidgetTodo, ui.taskWidgetDoing, ui.taskWidgetTesting, ui.taskWidgetDone};
    std::vector<QString>     colors{"#CAF0FC", "#ADE8F6", "#90E0EF", "#48CAE4"};
    std::vector<std::string> listNames{"Todo", "Doing", "Testing", "Done"};
    for (int i = 0; i < 4; ++i)
    {
        QLabel*  label  = labels[i];
        QWidget* widget = widgets[i];

        QString labelStyle  = QString("QLabel {background-color:%1; color:#FFFFFF; border-radius:8px; font-size: 16px;}").arg(colors[i]);
        QString widgetStyle = QString("QWidget#widget {background-color:%1; color:#FFFFFF; border-radius:8px;}").arg(colors[i]);

        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet(labelStyle);

        widget->setObjectName("widget");
        widget->setStyleSheet(widgetStyle);

        RevelationListView* view   = new RevelationListView(m_interface);
        QGridLayout*        layout = new QGridLayout(widget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(view);
        widget->setLayout(layout);

        m_listViews.emplace(listNames[i], view);
    }
}

void Revelation::InitSignalSlots()
{
}

RevelationSidebar* Revelation::GetSidebar(RevelationSidebar::Side side)
{
    RevelationSidebar* sidebar = nullptr;

    if (RevelationSidebar::Left == side)
    {
        if (nullptr == m_leftSidebar)
        {
            m_leftSidebar = new RevelationLeftSidebar(m_interface, this);

            m_leftSidebar->resize(m_leftSidebar->width(), this->height() - 30);

            QPoint basePos = mapToGlobal(this->pos());
            int    x       = basePos.x() + 20;
            int    y       = basePos.y() + 15;
            m_leftSidebar->move(x, y);

            connect(this, SIGNAL(CentralWidgetMoved(const QPoint&, const QSize&)), m_leftSidebar, SLOT(OnCentralWidgetMoved(const QPoint&)));
            connect(this, SIGNAL(CentralWidgetResized(const QSize&)), m_leftSidebar, SLOT(OnCentralWidgetResized(const QSize&)));
        }
        sidebar = m_leftSidebar;
    }
    else if (RevelationSidebar::Right == side)
    {
        if (nullptr == m_rightSidebar)
        {
            m_rightSidebar = new RevelationRightSidebar(m_interface, this);

            m_rightSidebar->resize(m_rightSidebar->width(), this->height() - 30);

            QPoint basePos = mapToGlobal(this->pos());
            int    x       = basePos.x() + width() - m_rightSidebar->width() - 20;
            int    y       = basePos.y() + 15;
            m_rightSidebar->move(x, y);

            connect(this, SIGNAL(CentralWidgetMoved(const QPoint&, const QSize&)), m_rightSidebar, SLOT(OnCentralWidgetMoved(const QPoint&, const QSize&)));
            connect(this, SIGNAL(CentralWidgetResized(const QSize&)), m_rightSidebar, SLOT(OnCentralWidgetResized(const QSize&)));
        }
        sidebar = m_rightSidebar;
    }
    else if (RevelationSidebar::Bottom == side)
    {
        if (nullptr == m_bottomBar)
        {
            m_bottomBar    = new RevelationBottomBar(m_interface, this);
            QPoint basePos = mapToGlobal(this->pos());
            int    x       = basePos.x() + this->size().width() / 2 - m_bottomBar->size().width() / 2;
            int    y       = basePos.y() + this->size().height() / 5 * 4;
            y              = std::max(y, basePos.y() + this->size().height() - m_bottomBar->height() - 50);
            m_bottomBar->move(x, y);

            connect(this, SIGNAL(CentralWidgetMoved(const QPoint&, const QSize&)), m_bottomBar, SLOT(OnCentralWidgetMoved(const QPoint&, const QSize&)));

            auto finder = m_listViews.find("Todo");
            if (finder != m_listViews.end())
            {
                RevelationListView* view = finder->second;
                connect(m_bottomBar, SIGNAL(TaskItemCreated(TaskPrototype)), view, SLOT(OnTaskItemReparenting(TaskPrototype)));
            }
        }
        sidebar = m_bottomBar;
    }

    return sidebar;
}
