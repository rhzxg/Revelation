#pragma once
#include <QListView>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include "IRevelationDataDefine.h"

class IRevelationInterface;
class RevelationListModel;
class RevelationListDelegate;

class RevelationListView : public QListView
{
    Q_OBJECT

  public:
    RevelationListView(IRevelationInterface* intf, QWidget* parent = nullptr);
    ~RevelationListView();

    void SetViewType(TaskStatus viewType);

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

  private:
    //virtual void paintEvent(QPaintEvent* event) override;

    // drag & drop
    virtual void startDrag(Qt::DropActions supportedActions) override;
    virtual void dragEnterEvent(QDragEnterEvent* event) override;
    virtual void dragMoveEvent(QDragMoveEvent* event) override;
    virtual void dragLeaveEvent(QDragLeaveEvent* event) override;
    virtual void dropEvent(QDropEvent* event) override;

  signals:
    void TaskItemReparenting(TaskPrototype task, TaskStatus from, TaskStatus to);

  private:
    IRevelationInterface* m_interface = nullptr;

    RevelationListModel*    m_model    = nullptr;
    RevelationListDelegate* m_delegate = nullptr;

    TaskStatus m_type = TaskStatus::None;

    bool   m_dragging     = false;
    bool   m_dropPosValid = false;
    QPoint m_dropPos;
};
