#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include "IRevelationInterface.h"
#include "FluControls/FluWidget.h"

class RevelationSidebar : public QWidget
{
  public:
    RevelationSidebar(IRevelationInterface* intf, QWidget* parent = nullptr)
        : m_interface(intf), QWidget(parent)
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

class RevelationSidebarWrapper : public FluWidget
{
  public:
    RevelationSidebarWrapper(QWidget* parent = nullptr)
        : FluWidget(parent)
    {
    }

    ~RevelationSidebarWrapper()
    {
        this->setLayout(nullptr);
        delete m_layout;
        delete m_sidebar;
    }

    void SetSidebar(RevelationSidebar* sidebar)
    {
        if (nullptr == sidebar)
        {
            return;
        }

        m_sidebar = sidebar;
        this->resize(sidebar->size());
        m_layout  = new QVBoxLayout(this);
        m_layout->addWidget(sidebar);
        this->setLayout(m_layout);
    }

    RevelationSidebar* GetSidebar()
    {
        return m_sidebar;
    }

  private:
    QVBoxLayout* m_layout = nullptr;

    RevelationSidebar* m_sidebar = nullptr;
};
