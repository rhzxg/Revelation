#pragma once
#include <QWidget>
#include "ui_Revelation.h"
#include "FramelessWindow.h"

class RevelationInterface;

class Revelation : public CFramelessWindow
{
    Q_OBJECT

  public:
    Revelation(RevelationInterface* intf, CFramelessWindow* parent = nullptr);
    ~Revelation();

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

  private:
    Ui::RevelationClass ui;

    RevelationInterface* m_interface = nullptr;
};
