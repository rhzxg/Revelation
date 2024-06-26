#pragma once
#include <QDialog>
#include "ui_RevelationSidebar.h"

class RevelationSidebar : public QDialog
{
    Q_OBJECT

  public:
    RevelationSidebar(QWidget* parent = nullptr);
    ~RevelationSidebar();

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

  public slots:
    void OnCentralWidgetMoved(const QPoint& point);
    void OnCentralWidgetResized(const QSize& size);

  private:
    Ui::RevelationSidebarClass ui;
};
