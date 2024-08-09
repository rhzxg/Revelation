#pragma once
#include "ICommonWidgetInterface.h"
#include "RevelationMainWindow.h"

class CommonWidgetInterface : public ICommonWidgetInterface
{
  public:
    CommonWidgetInterface(RevelationMainWindow* mainWindow);
    ~CommonWidgetInterface();

    virtual void AddStackedWidget(QWidget* widget, const QString& name, const QIcon& icon, Qt::AlignmentFlag pos = Qt::AlignCenter)   override;
    virtual void AddStackedWidget(QWidget* widget, const QString& name, FluAwesomeType type, Qt::AlignmentFlag pos = Qt::AlignCenter) override;

  private:
    RevelationMainWindow* m_mainWindow = nullptr;
};