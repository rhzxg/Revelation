#pragma once
#include <QDialog>
#include "IRevelationInterface.h"

class RevelationSidebar : public QDialog
{
  public:
    RevelationSidebar(IRevelationInterface* intf, QWidget* parent = nullptr)
        : m_interface(intf), QDialog(parent)
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
