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
    Ui::TimeMachineFilterClass ui;
};
