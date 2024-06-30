#pragma once
#include <QWidget>
#include <QMouseEvent>
#include "ui_Revelation.h"
#include "RevelationSidebar.h"

class RevelationInterface;
class RevelationBottomBar;

class Revelation : public QWidget
{
    Q_OBJECT

  public:
    Revelation(RevelationInterface* intf, QWidget* parent = nullptr);
    ~Revelation();

    void SetBottomBarVisible(bool visible);

  private:
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

    RevelationSidebar*   GetSidebar(RevelationSidebar::Side side);
    RevelationBottomBar* GetBottomBar();

  signals:
    void CentralWidgetMoved(const QPoint& point, const QSize& size);
    void CentralWidgetResized(const QSize& size);

  private:
    Ui::RevelationClass ui;

    RevelationInterface* m_interface = nullptr;

    RevelationSidebar*   m_leftSidebar  = nullptr;
    RevelationSidebar*   m_rightSidebar = nullptr;
    RevelationBottomBar* m_bottomBar    = nullptr;
};
