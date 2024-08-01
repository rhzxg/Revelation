#pragma once
#include <QWidget>
#include <QGridLayout>
#include "ui_TimeMachine.h"

class TimeMachineFilter;
class TimeMachineGantt;

class TimeMachine : public QWidget
{
    Q_OBJECT

  public:
    TimeMachine(QWidget* parent = nullptr);
    ~TimeMachine();

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

  private:
    Ui::TimeMachineClass ui;

    QGridLayout* m_timeMachineFilterLayout = nullptr;
    QGridLayout* m_timeMachineGanttLayout  = nullptr;

    TimeMachineFilter* m_timeMachindFilter = nullptr;
    TimeMachineGantt*  m_timeMachindGantt  = nullptr;
};
