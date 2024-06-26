#pragma once
#include <QWidget>
#include <QMouseEvent>
#include "ui_Revelation.h"
#include "RevelationSidebar.h"
#include "RevelationBottomBar.h"

class RevelationInterface;

class Revelation : public QWidget
{
    Q_OBJECT

  public:
    Revelation(RevelationInterface* intf, QWidget* parent = nullptr);
    ~Revelation();

    void ShowBottomBar();

  private:
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

    RevelationSidebar*   GetSidebar();
    RevelationBottomBar* GetBottomBar();

  signals:
    void CentralWidgetMoved(const QPoint& point);
    void CentralWidgetResized(const QSize& size);

  private:
    Ui::RevelationClass ui;

    RevelationInterface* m_interface = nullptr;

    RevelationSidebar*   m_sidebar   = nullptr;
    RevelationBottomBar* m_bottomBar = nullptr;
};
