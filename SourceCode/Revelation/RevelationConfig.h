#pragma once
#include <QWidget>
#include "ui_RevelationConfig.h"

class FluVScrollView;

class RevelationConfig : public QWidget
{
    Q_OBJECT

  public:
    RevelationConfig(QWidget* parent = nullptr);
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

    FluVScrollView* m_scrollArea = nullptr;
};
