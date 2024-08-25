#pragma once
#include <QWidget>
#include "ui_RevelationConfig.h"
#include "IRevelationInterface.h"
#include "FluVScrollView.h"

class RevelationConfig : public QWidget
{
    Q_OBJECT

  public:
    RevelationConfig(IRevelationInterface* intf, QWidget* parent = nullptr);
    ~RevelationConfig();

    void AddSettingItem(QWidget* item);

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

    void SetupApplicationInfoItem();
    void SetupChangeThemeItem();

  private:
    Ui::RevelationConfigClass ui;

    IRevelationInterface* m_interface = nullptr;

    FluVScrollView* m_scrollArea = nullptr;
};
