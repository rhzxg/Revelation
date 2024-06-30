#include "RevelationListView.h"
#include "RevelationListModel.h"
#include "RevelationListDelegate.h"
#include <QListWidgetItem>

RevelationListView::RevelationListView(QWidget* parent /*= nullptr*/)
    : QListView(parent)
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
    m_model    = new RevelationListModel(this);
    m_delegate = new RevelationListDelegate(this);
    this->setModel(m_model);
    this->setItemDelegate(m_delegate);

    InitWidget();
    InitSignalSlots();
}

void RevelationListView::InitWidget()
{
    this->setSpacing(2);
    this->setStyleSheet("QListView { background: transparent; border: none;}");
}

void RevelationListView::InitSignalSlots()
{
}
