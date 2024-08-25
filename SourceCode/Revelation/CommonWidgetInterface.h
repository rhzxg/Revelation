#pragma once
#include "ICommonWidgetInterface.h"
#include "RevelationMainWindow.h"
#include "IRevelationInterface.h"
#include "FluProgressBar.h"

class RevelationConfig;

class CommonWidgetInterface : public ICommonWidgetInterface
{
  public:
    CommonWidgetInterface(RevelationMainWindow* mainWindow);
    ~CommonWidgetInterface();

    virtual void AddStackedWidget(QWidget* widget, const QString& name, const QIcon& icon, Qt::AlignmentFlag pos = Qt::AlignCenter) override;
    virtual void AddStackedWidget(QWidget* widget, const QString& name, FluAwesomeType type, Qt::AlignmentFlag pos = Qt::AlignCenter) override;

    virtual void AddSettingsItem(QWidget* widget) override;

    virtual void SetProgressBarVisibility(bool visible, int value = 0) override;

    void SetInterface(IRevelationInterface* intf);
    void InitializeConfig();

  private:
    void Initialize();
    void InitWidget();

  private:
    IRevelationInterface* m_interface  = nullptr;
    RevelationMainWindow* m_mainWindow = nullptr;

    RevelationConfig* m_config = nullptr;

    FluProgressBar* m_progressBar = nullptr;
};