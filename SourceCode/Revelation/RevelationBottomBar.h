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
    void TaskItemCreated(TaskPrototype task, TaskStatus from, TaskStatus to);

  public slots:
    void OnBtnConfirmClicked();

  private:
    Ui::RevelationBottomBarClass ui;
};
