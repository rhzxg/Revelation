#pragma once
#include "RevelationSidebar.h"
#include "ui_RevelationRightSidebar.h"
#include "IRevelationDataDefine.h"

class RevelationRightSidebar : public RevelationSidebar
{
    Q_OBJECT

  public:
    RevelationRightSidebar(IRevelationInterface* intf, QWidget* parent = nullptr);
    ~RevelationRightSidebar();

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

    virtual void showEvent(QShowEvent* event) override;
    virtual void hideEvent(QHideEvent* event) override;
    virtual void closeEvent(QCloseEvent* event) override;

    void SetBtnAddToRoutineState(bool isRoutine);

  signals:
    void TaskItemEdited(const TaskPrototype& task);
    void TaskItemDeleted(const TaskPrototype& task);

  public slots:
    void OnTaskItemSelected(const TaskPrototype& task);
    void OnTaskItemEdited();
    
    void OnBtnAddToRoutineClicled();
    void OnBtnDeleteTaskItemClicked();
    void OnBtnSelectStartTimeClicked(QDate date);
    void OnBtnSelectFinishTimeClicked(QDate date);
    void OnBtnSelectDeadlineClicked(QDate date);

  private:
    void BlockSignals(bool block);

private:
    Ui::RevelationRightSidebarClass ui;
    IRevelationInterface*           m_interface = nullptr;

    TaskPrototype m_task;
    bool          m_taskValid = false;
};
