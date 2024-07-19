#pragma once
#include "RevelationSidebar.h"
#include "ui_RevelationBottomBar.h"
#include "IRevelationDataDefine.h"
#include <QKeyEvent>

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

    virtual void keyPressEvent(QKeyEvent* event) override;

  signals:
    void TaskItemCreated(TaskPrototype task, TaskStatus from, TaskStatus to);

  public slots:
    void OnBtnConfirmClicked();

  private:
    Ui::RevelationBottomBarClass ui;
};
