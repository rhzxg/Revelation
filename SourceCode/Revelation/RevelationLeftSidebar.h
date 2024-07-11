#pragma once
#include "RevelationSidebar.h"
#include "ui_RevelationLeftSidebar.h"

class RevelationLeftSidebar : public RevelationSidebar
{
    Q_OBJECT

  public:
    RevelationLeftSidebar(IRevelationInterface* intf, QWidget* parent = nullptr);
    ~RevelationLeftSidebar();

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

    virtual void showEvent(QShowEvent* event) override;

  public slots:
    void OnCentralWidgetResized(const QSize& size);

  private:
    Ui::RevelationLeftSidebarClass ui;
};
