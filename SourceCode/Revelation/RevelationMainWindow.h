#pragma once
#include "FluFrameLessWidget.h"
#include "IRevelatioMainWindow.h"
#include "FluVNavigationView.h"
#include "FluStackedLayout.h"

class Revelation;

class RevelationMainWindow : public FluFrameLessWidget, public IRevelationMainWindow
{
    Q_OBJECT

  public:
    RevelationMainWindow(QWidget* parent = nullptr);
    ~RevelationMainWindow();

    void AlignToScreenCenter();

    virtual void AddStackedWidget(QWidget* widget, const QString& name, const QIcon& icon, Qt::AlignmentFlag pos  = Qt::AlignCenter) override;
    virtual void AddStackedWidget(QWidget* widget, const QString& name, FluAwesomeType type, Qt::AlignmentFlag pos = Qt::AlignCenter) override;

  private:
    void Initialize();
    void InitWidget();

  private:
    FluVNavigationView* m_navigationView;
    FluStackedLayout*   m_stackedLayout;
};
