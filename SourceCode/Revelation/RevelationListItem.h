#pragma once
#include <QWidget>
#include "ui_RevelationListItem.h"
#include "IRevelationDataDefine.h"

class RevelationListItem : public QWidget
{
    Q_OBJECT

  public:
    RevelationListItem(QWidget* parent = nullptr);
    ~RevelationListItem();

    void          SetItemTaskData(TaskPrototype task);
    TaskPrototype GetItemTaskData();

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

  private:
    Ui::RevelationListItemClass ui;

    TaskPrototype m_task;
};
