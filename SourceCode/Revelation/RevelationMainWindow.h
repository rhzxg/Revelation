#pragma once
#include <QMainWindow>

class RevelationMainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    RevelationMainWindow(QWidget* parent = nullptr);
    ~RevelationMainWindow();

  private:
    void Initialize();
    void InitWidget();

  private:
    virtual void moveEvent(QMoveEvent* event) override;
};
