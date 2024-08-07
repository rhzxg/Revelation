#pragma once
#include <QWidget>
#include "ui_TimeMachineGanttView.h"
#include <KDGanttView>
#include "TimeMachineGanttModel.h"

class TimeMachineGanttView : public QWidget
{
    Q_OBJECT

  public:
    TimeMachineGanttView(QWidget* parent = nullptr);
    ~TimeMachineGanttView();

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

  private:
    Ui::TimeMachineGanttViewClass ui;

    KDGantt::View*         m_view  = nullptr;
    TimeMachineGanttModel* m_model = nullptr;
};
