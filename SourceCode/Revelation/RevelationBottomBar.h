#pragma once
#include <QDialog>
#include "ui_RevelationBottomBar.h"

class RevelationBottomBar : public QDialog
{
    Q_OBJECT

  public:
    RevelationBottomBar(QWidget* parent = nullptr);
    ~RevelationBottomBar();

    void SetVisible(bool visible);
    bool IsVisible();

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

  public slots:
    void OnCentralWidgetMoved(const QPoint& point, const QSize& size);

  private:
    Ui::RevelationBottomBarClass ui;
};
