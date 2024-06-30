#include "RevelationListView.h"
#include "RevelationListModel.h"
#include "RevelationListDelegate.h"
#include "IRevelationInterface.h"
#include <QListWidgetItem>
#include <QLayout>

RevelationListView::RevelationListView(IRevelationInterface* intf, QWidget* parent /*= nullptr*/)
    : m_interface(intf), QListView(parent)
{
    Initialize();
}

RevelationListView::~RevelationListView()
{
    this->setModel(nullptr);
    this->setItemDelegate(nullptr);
    delete m_model;
    delete m_delegate;
}

void RevelationListView::Initialize()
{
    m_model    = new RevelationListModel(m_interface, this);
    m_delegate = new RevelationListDelegate(m_interface, this);
    this->setModel(m_model);
    this->setItemDelegate(m_delegate);

    InitWidget();
    InitSignalSlots();
}

void RevelationListView::InitWidget()
{
    this->setDragEnabled(true);
    this->setAcceptDrops(true);
    this->setDropIndicatorShown(true);
    this->setDragDropMode(QAbstractItemView::DragDrop);

    this->setStyleSheet("QListView { background: transparent; border: none; }");
}

void RevelationListView::InitSignalSlots()
{
}

void RevelationListView::OnTaskItemReparenting(TaskPrototype task)
{
    m_model->m_tasks.push_back(task);
    m_model->layoutChanged();
}
