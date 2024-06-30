#pragma once
#include <QWidget>
#include "ui_RevelationListItem.h"

class RevelationListItem : public QWidget
{
    Q_OBJECT

  public:
    RevelationListItem(QWidget* parent = nullptr);
    ~RevelationListItem();

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

  private:
    Ui::RevelationListItemClass ui;
};
