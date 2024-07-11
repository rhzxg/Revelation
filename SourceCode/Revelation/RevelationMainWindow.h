#pragma once
#include "FluControls/FluFrameLessWidget.h"

class Revelation;

class RevelationMainWindow : public FluFrameLessWidget
{
    Q_OBJECT

  public:
    RevelationMainWindow(QWidget* parent = nullptr);
    ~RevelationMainWindow();

    void SetCentralWidget(Revelation* widget);

  private:
    void Initialize();
    void InitWidget();

  private:
    virtual void moveEvent(QMoveEvent* event) override;

  private:
    Revelation* m_revelationWidget = nullptr;
};
