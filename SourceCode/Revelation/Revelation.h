#pragma once
#include <QWidget>
#include "ui_Revelation.h"

class RevelationInterface;

class Revelation : public QWidget
{
    Q_OBJECT

  public:
    Revelation(RevelationInterface* intf, QWidget* parent = nullptr);
    ~Revelation();

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

  private:
    Ui::RevelationClass ui;

    RevelationInterface* m_interface = nullptr;
};
