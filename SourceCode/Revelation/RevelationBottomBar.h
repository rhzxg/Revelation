#pragma once
#include "RevelationSidebar.h"
#include "ui_RevelationBottomBar.h"
#include "IRevelationDataDefine.h"

class RevelationBottomBar : public RevelationSidebar
{
    Q_OBJECT

  public:
    RevelationBottomBar(IRevelationInterface* intf, QWidget* parent = nullptr);
    ~RevelationBottomBar();

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

  signals:
    void TaskItemCreated(TaskPrototype task);

  public slots:
    void OnCentralWidgetMoved(const QPoint& point, const QSize& size);
    void OnBtnConfirmClicked();

  private:
    Ui::RevelationBottomBarClass ui;
};
