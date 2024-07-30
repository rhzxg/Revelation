#pragma once
#include "INavigationViewInterface.h"
#include "RevelationMainWindow.h"

class NavigationViewInterface : public INavigationViewInterface
{
  public:
    NavigationViewInterface(RevelationMainWindow* mainWindow);
    ~NavigationViewInterface();

    virtual void AddStackedWidget(QWidget* widget, const QString& name, const QIcon& icon, Qt::AlignmentFlag pos = Qt::AlignCenter)   override;
    virtual void AddStackedWidget(QWidget* widget, const QString& name, FluAwesomeType type, Qt::AlignmentFlag pos = Qt::AlignCenter) override;

  private:
    RevelationMainWindow* m_mainWindow = nullptr;
};