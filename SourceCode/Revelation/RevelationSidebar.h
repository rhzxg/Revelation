#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include "IRevelationInterface.h"
#include "FluWidget.h"

class RevelationSidebar : public FluWidget
{
  public:
    RevelationSidebar(IRevelationInterface* intf, QWidget* parent = nullptr)
        : m_interface(intf), FluWidget(parent)
    {
    }

    ~RevelationSidebar()
    {
    }

    void SetVisible(bool visible)
    {
        visible ? this->show() : this->hide();
    }

    bool IsVisible()
    {
        return this->isVisible();
    }

  public:
    enum Side
    {
        Left = 0,
        Right,
        Bottom
    };

  protected:
    IRevelationInterface* m_interface = nullptr;
};
