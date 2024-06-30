#pragma once
#include <QWidget>
#include <QMouseEvent>
#include "ui_Revelation.h"
#include "RevelationSidebar.h"
#include <unordered_map>

class IRevelationInterface;
class RevelationBottomBar;
class RevelationListView;

class Revelation : public QWidget
{
    Q_OBJECT

  public:
    Revelation(IRevelationInterface* intf, QWidget* parent = nullptr);
    ~Revelation();

    void SetBottomBarVisible(bool visible);

  private:
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void showEvent(QShowEvent* event) override;

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

    RevelationSidebar*   GetSidebar(RevelationSidebar::Side side);

  signals:
    void CentralWidgetMoved(const QPoint& point, const QSize& size);
    void CentralWidgetResized(const QSize& size);

  private:
    Ui::RevelationClass ui;

    IRevelationInterface* m_interface = nullptr;

    RevelationSidebar*   m_leftSidebar  = nullptr;
    RevelationSidebar*   m_rightSidebar = nullptr;
    RevelationSidebar*   m_bottomBar    = nullptr;

    std::unordered_map<std::string, RevelationListView*> m_listViews;
};
