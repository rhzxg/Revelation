#pragma once
#include <QDialog>

class RevelationSidebar : public QDialog
{
  public:
    RevelationSidebar(QWidget* parent = nullptr)
        : QDialog(parent)
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
        Right
    };
};
