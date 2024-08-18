#pragma once
#include <QWidget>
#include "ui_TimeMachineGanttView.h"
#include <KDGanttView>
#include <QTreeView>
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

    void CopyTasksToClipboard(Node* summaryNode);

  public slots:
    void OnTaskFiltered(const std::vector<DateToTasks>& dateToTaskVec);
    void OnContextMenuEvent(const QPoint& pos);

  private:
    Ui::TimeMachineGanttViewClass ui;

    IRevelationInterface* m_interface     = nullptr;
    IDateTimeFormatter*   m_timeFormatter = nullptr;

    KDGantt::View*         m_view      = nullptr;
    QTreeView*             m_leftView  = nullptr;
    KDGantt::GraphicsView* m_rightView = nullptr;

    TimeMachineGanttModel* m_model = nullptr;
};
