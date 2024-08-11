#pragma once
#include <QWidget>
#include "ui_TimeMachineGanttView.h"
#include <KDGanttView>
#include "TimeMachineGanttModel.h"
#include "IRevelationDataDefine.h"

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

  public slots:
    void OnTaskFiltered(const std::map<std::string, std::vector<TaskPrototype>>& dateToTasks);

  private:
    Ui::TimeMachineGanttViewClass ui;

    KDGantt::View*         m_view  = nullptr;
    TimeMachineGanttModel* m_model = nullptr;
};
