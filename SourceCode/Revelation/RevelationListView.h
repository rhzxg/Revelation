#pragma once
#include <QListView>
#include "IRevelationDataDefine.h"

class IRevelationInterface;
class RevelationListModel;
class RevelationListDelegate;

class RevelationListView : public QListView
{
    Q_OBJECT

  public:
    RevelationListView(IRevelationInterface* intf, QWidget* parent = nullptr);
    ~RevelationListView();

  private:
    void Initialize();
    void InitWidget();
    void InitSignalSlots();

  public slots:
    void OnTaskItemReparenting(TaskPrototype task);

  private:
    IRevelationInterface* m_interface = nullptr;

    RevelationListModel*    m_model    = nullptr;
    RevelationListDelegate* m_delegate = nullptr;
};
