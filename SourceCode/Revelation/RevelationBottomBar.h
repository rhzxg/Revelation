#pragma once
#include <QDialog>
#include "ui_RevelationBottomBar.h"

class RevelationBottomBar : public QDialog
{
    Q_OBJECT

  public:
    RevelationBottomBar(QWidget* parent = nullptr);
    ~RevelationBottomBar();

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

  public slots:
    void OnCentralWidgetChanged(const QPoint& point);

  private:
    Ui::RevelationBottomBarClass ui;
};
