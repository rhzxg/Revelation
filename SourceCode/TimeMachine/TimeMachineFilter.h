#pragma once
#include <QWidget>
#include "ui_TimeMachineFilter.h"

class TimeMachineFilter : public QWidget
{
    Q_OBJECT

  public:
    TimeMachineFilter(QWidget* parent = nullptr);
    ~TimeMachineFilter();

  private:
    void Initialize();
    void InitWIdget();
    void InitSignalSlots();

  private:
    Ui::TimeMachineFilterClass ui;
};
