#pragma once
#include "RevelationSidebar.h"
#include "ui_RevelationRightSidebar.h"

class RevelationRightSidebar : public RevelationSidebar
{
    Q_OBJECT

  public:
    RevelationRightSidebar(QWidget* parent = nullptr);
    ~RevelationRightSidebar();

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

    virtual void showEvent(QShowEvent* event) override;

  public slots:
    void OnCentralWidgetMoved(const QPoint& point, const QSize& size);
    void OnCentralWidgetResized(const QSize& size);

  private:
    Ui::RevelationRightSidebarClass ui;
};
