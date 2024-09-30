#pragma once
#include <QWidget>
#include <QMouseEvent>
#include "ui_Revelation.h"
#include "RevelationSidebar.h"
#include "IRevelationDataDefine.h"
#include <unordered_map>

class IRevelationInterface;
class RevelationBottomBar;
class RevelationListView;

class Revelation : public QWidget
{
    Q_OBJECT

  public:
    Revelation(IRevelationInterface* intf, QWidget* parent = nullptr);
    ~Revelation();

    void ReteiveDataFromDatabase();

  private:
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void showEvent(QShowEvent* event) override;

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

    RevelationSidebar* GetSidebar(RevelationSidebar::Side side);

  signals:
    void TaskItemReparenting(const TaskPrototype& task);

  public slots:
    void OnThemeChanged();
    void OnTaskItemReparenting(TaskPrototype task, TaskStatus from, TaskStatus to);
    void OnTaskItemEdited(const TaskPrototype& task);
    void OnTaskItemDeleted(const TaskPrototype& task);

  private:
    Ui::RevelationClass ui;

    IRevelationInterface* m_interface = nullptr;

    RevelationSidebar* m_rightSidebar = nullptr;
    RevelationSidebar* m_bottomBar    = nullptr;

    std::unordered_map<TaskStatus, RevelationListView*> m_listViews;
};
