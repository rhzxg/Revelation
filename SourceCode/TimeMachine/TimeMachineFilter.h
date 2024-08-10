#pragma once
#include <QWidget>
#include "ui_TimeMachineFilter.h"
#include "FluProgressBar.h"
#include "IRevelationDataDefine.h"

class IRevelationInterface;

class TimeMachineFilter : public QWidget
{
    Q_OBJECT

  public:
    TimeMachineFilter(IRevelationInterface* intf, QWidget* parent = nullptr);
    ~TimeMachineFilter();

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

  signals:
    void TaskFiltered(const std::vector<TaskPrototype>& tasks);

  private slots:
    void OnBtnFilterClicked();

  private:
    Ui::TimeMachineFilterClass ui;

    IRevelationInterface* m_interface = nullptr;
};
