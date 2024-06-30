#pragma once
#include <QDialog>
#include "ui_RevelationLeftSidebar.h"

class RevelationLeftSidebar : public QDialog
{
    Q_OBJECT

  public:
    RevelationLeftSidebar(QWidget* parent = nullptr);
    ~RevelationLeftSidebar();

    void SetVisible(bool visible);
    bool IsVisible();

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

    virtual void showEvent(QShowEvent* event) override;

  public slots:
    void OnCentralWidgetMoved(const QPoint& point);
    void OnCentralWidgetResized(const QSize& size);

  private:
    Ui::RevelationLeftSidebarClass ui;
};
