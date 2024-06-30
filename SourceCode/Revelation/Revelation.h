#pragma once
#include <QWidget>
#include <QMouseEvent>
#include "ui_Revelation.h"
#include "RevelationLeftSidebar.h"
#include "RevelationBottomBar.h"

class RevelationInterface;

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

    RevelationLeftSidebar* GetSidebar();
    RevelationBottomBar*   GetBottomBar();

  signals:
    void CentralWidgetMoved(const QPoint& point, const QSize& size);
    void CentralWidgetResized(const QSize& size);

  private:
    Ui::RevelationClass ui;

    RevelationInterface* m_interface = nullptr;

    RevelationLeftSidebar* m_sidebar   = nullptr;
    RevelationBottomBar*   m_bottomBar = nullptr;
};
