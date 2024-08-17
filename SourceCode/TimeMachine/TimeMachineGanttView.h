#pragma once
#include <QWidget>
#include "ui_TimeMachineGanttView.h"
#include <KDGanttView>
#include "TimeMachineGanttModel.h"
#include "IRevelationDataDefine.h"

class IRevelationInterface;
class IDateTimeFormatter;

class TimeMachineGanttView : public QWidget
{
    Q_OBJECT

  public:
    TimeMachineGanttView(IRevelationInterface* intf, QWidget* parent = nullptr);
    ~TimeMachineGanttView();

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

  public slots:
    void OnTaskFiltered(const std::vector<DateToTasks>& dateToTaskVec);

  private:
    Ui::TimeMachineGanttViewClass ui;

    IRevelationInterface* m_interface = nullptr;
    IDateTimeFormatter*   m_timeFormatter = nullptr;

    KDGantt::View*         m_view  = nullptr;
    TimeMachineGanttModel* m_model = nullptr;
};
