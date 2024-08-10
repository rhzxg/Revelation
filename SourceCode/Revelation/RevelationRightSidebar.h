#pragma once
#include "RevelationSidebar.h"
#include "ui_RevelationRightSidebar.h"
#include "IRevelationDataDefine.h"

class IDateTimeFormatter;
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

    void RefreshTaskData(const TaskPrototype& task);

  signals:
    void TaskItemEdited(const TaskPrototype& task);
    void TaskItemDeleted(const TaskPrototype& task);

  public slots:
    void OnTaskReparenting(const TaskPrototype& task);
    void OnTaskItemSelected(const TaskPrototype& task);
    void OnTaskItemEdited();

    void OnBtnAddToRoutineClicled();
    void OnBtnDeleteTaskItemClicked();
    void OnCbTypeIndexChanged(int index);
    void OnStartDateSelected(QDate date);
    void OnFinishDateSelected(QDate date);
    void OnDeadlineDateSelected(QDate date);
    void OnStartTimeSelected(QTime time);
    void OnFinishTimeSelected(QTime time);
    void OnDeadlineTimeSelected(QTime time);

  private:
    void BlockSignals(bool block);

  private:
    Ui::RevelationRightSidebarClass ui;
    IRevelationInterface*           m_interface     = nullptr;
    IDateTimeFormatter*             m_timeFormatter = nullptr;

    TaskPrototype m_task;
    bool          m_taskValid = false;
};
