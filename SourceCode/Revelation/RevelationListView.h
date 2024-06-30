#pragma once
#include <QListView>

class RevelationListModel;

class RevelationListView : public QListView
{
    Q_OBJECT

  public:
    RevelationListView(QWidget* parent = nullptr);
    ~RevelationListView();

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

  private:
    RevelationListModel* m_model = nullptr;
};