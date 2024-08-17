#pragma once
#include <QWidget>
#include "ui_TimeMachineFilter.h"
#include "FluProgressBar.h"
#include "IRevelationDataDefine.h"

class IRevelationInterface;
class ICommonWidgetInterface;
class IDataPersistenceInterface;
class IDateTimeFormatter;

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

    void ReteiveTasksFromDatabase(std::vector<DateToTasks>& dateToTaskVec);
    void FilterTasksByCondition(std::vector<DateToTasks>& dateToTaskVec);
    void SortTasksByTaskParameter(std::vector<DateToTasks>& dateToTaskVec);

  signals:
    void TaskFiltered(const std::vector<DateToTasks>& dateToTaskVec);

  private slots:
    void OnBtnFilterClicked();

  private:
    Ui::TimeMachineFilterClass ui;

    IRevelationInterface*      m_interface           = nullptr;
    ICommonWidgetInterface*    m_commonWidgetIntf    = nullptr;
    IDataPersistenceInterface* m_dataPersistenceIntf = nullptr;
    IDateTimeFormatter*        m_timeFormatter       = nullptr;
};
