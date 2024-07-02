#pragma once
#include <QDialog>
#include "ui_RevelationSplash.h"

class IRevelationInterface;

class RevelationSplash : public QDialog
{
    Q_OBJECT

  public:
    RevelationSplash(IRevelationInterface* intf, QWidget* parent = nullptr);
    ~RevelationSplash();

  private:
    void Initialize();
    void InitWidget();

  private:
    Ui::RevelationSplashClass ui;

    IRevelationInterface* m_interface = nullptr;
};
