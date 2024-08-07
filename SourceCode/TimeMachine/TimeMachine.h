#pragma once
#include <QWidget>
#include <QGridLayout>
#include "ui_TimeMachine.h"

class TimeMachineFilter;
class TimeMachineGanttView;

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

    TimeMachineFilter*    m_timeMachindFilter    = nullptr;
    TimeMachineGanttView* m_timeMachindGanttView = nullptr;
};
