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

    void ReteiveTasksFromDatabase(std::map<std::string, std::vector<TaskPrototype>>& dateToTasks);
    void FilterTasksByCondition(std::map<std::string, std::vector<TaskPrototype>>& dateToTasks);
    void SortTasksByTaskParameter(std::map<std::string, std::vector<TaskPrototype>>& dateToTasks);

  signals:
    void TaskFiltered(const std::map<std::string, std::vector<TaskPrototype>>& dateToTasks);

  private slots:
    void OnBtnFilterClicked();

  private:
    Ui::TimeMachineFilterClass ui;

    IRevelationInterface*      m_interface           = nullptr;
    ICommonWidgetInterface*    m_commonWidgetIntf    = nullptr;
    IDataPersistenceInterface* m_dataPersistenceIntf = nullptr;
    IDateTimeFormatter*        m_timeFormatter       = nullptr;
};
