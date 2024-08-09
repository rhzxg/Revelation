#pragma once
#include "ICommonWidgetInterface.h"
#include "RevelationMainWindow.h"
#include "FluProgressBar.h"

class CommonWidgetInterface : public ICommonWidgetInterface
{
  public:
    CommonWidgetInterface(RevelationMainWindow* mainWindow);
    ~CommonWidgetInterface();

    virtual void AddStackedWidget(QWidget* widget, const QString& name, const QIcon& icon, Qt::AlignmentFlag pos = Qt::AlignCenter) override;
    virtual void AddStackedWidget(QWidget* widget, const QString& name, FluAwesomeType type, Qt::AlignmentFlag pos = Qt::AlignCenter) override;

    virtual void SetProgressBarVisibility(bool visible, int value = 0) override;

  public:
    void UpdateProgressBarPos(const QPoint& point);

  private:
    void Initialize();
    void InitWidget();

  private:
    RevelationMainWindow* m_mainWindow = nullptr;

    FluProgressBar* m_progressBar = nullptr;
};