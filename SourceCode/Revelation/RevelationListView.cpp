#include "RevelationListView.h"
#include "RevelationListModel.h"

RevelationListView::RevelationListView(QWidget* parent /*= nullptr*/)
    : QListView(parent)
{
    Initialize();
}

RevelationListView::~RevelationListView()
{
    QListView::setModel(nullptr);
    delete m_model;
}

void RevelationListView::Initialize()
{
    m_model = new RevelationListModel(this);
    QListView::setModel(m_model);

    InitWidget();
    InitSignalSlots();
}

void RevelationListView::InitWidget()
{
    this->setStyleSheet("QListView { background: transparent; border: none;}");
}

void RevelationListView::InitSignalSlots()
{
}
