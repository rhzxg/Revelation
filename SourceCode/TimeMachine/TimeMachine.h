#pragma once
#include <QWidget>
#include <QGridLayout>
#include "ui_TimeMachine.h"

class IRevelationInterface;
class TimeMachineFilter;
class TimeMachineGanttView;

class TimeMachine : public QWidget
{
    Q_OBJECT

  public:
    TimeMachine(IRevelationInterface* intf, QWidget* parent = nullptr);
    ~TimeMachine();

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

  private slots:
    void OnThemeChanged();

  private:
    Ui::TimeMachineClass ui;

    IRevelationInterface* m_interface = nullptr;

    TimeMachineFilter*    m_timeMachindFilter    = nullptr;
    TimeMachineGanttView* m_timeMachindGanttView = nullptr;
};
