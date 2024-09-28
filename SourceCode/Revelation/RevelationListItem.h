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

    void SetTaskData(const TaskPrototype& task);
    void ResizeWidget(const QSize& size);
    void SetMouseHoverd(bool hoverd);

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

  private:
    Ui::RevelationListItemClass ui;

    bool m_lightMode = true;

    TaskPrototype m_task;
};
