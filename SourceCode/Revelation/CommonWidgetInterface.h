#pragma once
#include "ICommonWidgetInterface.h"
#include "RevelationMainWindow.h"
#include "FluProgressBar.h"

class RevelationConfig;

class CommonWidgetInterface : public ICommonWidgetInterface
{
  public:
    CommonWidgetInterface(RevelationMainWindow* mainWindow);
    ~CommonWidgetInterface();

    virtual void AddStackedWidget(QWidget* widget, const QString& name, const QIcon& icon, Qt::AlignmentFlag pos = Qt::AlignCenter) override;
    virtual void AddStackedWidget(QWidget* widget, const QString& name, FluAwesomeType type, Qt::AlignmentFlag pos = Qt::AlignCenter) override;

    virtual void AddSettingItem(QWidget* widget) override;

    virtual void SetProgressBarVisibility(bool visible, int value = 0) override;

  private:
    void Initialize();
    void InitWidget();

  private:
    RevelationMainWindow* m_mainWindow = nullptr;
    RevelationConfig*     m_config     = nullptr;

    FluProgressBar* m_progressBar = nullptr;
};