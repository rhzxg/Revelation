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

  signals:
    void TaskItemEdited(const TaskPrototype& task);
    void TaskItemDeleted(const TaskPrototype& task);

  public slots:
    void OnCentralWidgetMoved(const QPoint& point, const QSize& size);
    void OnCentralWidgetResized(const QSize& size);

    void OnTaskItemSelected(TaskPrototype task);
    void OnTaskItemEdited();
    void OnBtnDeleteTaskItemClicked();

  private:
    Ui::RevelationRightSidebarClass ui;

    TaskPrototype m_task;
    bool          m_taskValid = false;
};
