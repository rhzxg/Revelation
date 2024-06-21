#pragma once
#include <QDialog>
#include "ui_RevelationSplash.h"
#include "RevelationInterface.h"

class RevelationSplash : public QDialog
{
    Q_OBJECT

  public:
    RevelationSplash(RevelationInterface* intf, QWidget* parent = nullptr);
    ~RevelationSplash();

  private:
    void Initialize();
    void InitWidget();

  private:
    Ui::RevelationSplashClass ui;

    RevelationInterface* m_interface = nullptr;
};
