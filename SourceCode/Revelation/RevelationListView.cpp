#include "RevelationListView.h"
#include "RevelationListModel.h"
#include "RevelationListDelegate.h"
#include "IRevelationInterface.h"
#include <QListWidgetItem>
#include <QPainter>
#include <QMimeData>
#include <QDrag>

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

void RevelationListView::SetViewType(TaskStatus viewType)
{
    m_type = viewType;
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
    this->setDropIndicatorShown(false);
    this->setDragDropMode(QAbstractItemView::DragDrop);

    this->setStyleSheet("QListView { background: transparent; border: none; }");
}

void RevelationListView::InitSignalSlots()
{
}

// void RevelationListView::paintEvent(QPaintEvent* event)
//{
//     QAbstractItemView::paintEvent(event);
//
//     if (m_dragging && m_dragging)
//     {
//         QPainter painter(viewport());
//         painter.setPen(Qt::red);
//         painter.drawLine(0, m_dropPos.y(), viewport()->width(), m_dropPos.y());
//     }
// }

void RevelationListView::startDrag(Qt::DropActions supportedActions)
{
    QModelIndexList indexes = selectedIndexes();
    if (indexes.count() > 0)
    {
        QByteArray  itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);

        Uint64 taskID = m_model->m_tasks.at(indexes.first().row()).m_id;
        dataStream << taskID;

        QMimeData* mimeData = new QMimeData;
        mimeData->setData("revelation/task_data", itemData);

        QDrag* drag = new QDrag(this);
        drag->setMimeData(mimeData);

        drag->exec(Qt::MoveAction);
    }
}

void RevelationListView::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("revelation/task_data"))
    {
        event->acceptProposedAction();

        m_dragging     = true;
        m_dropPosValid = true;
        m_dropPos      = event->pos();
        viewport()->update();
    }
}

void RevelationListView::dragMoveEvent(QDragMoveEvent* event)
{
    if (event->mimeData()->hasFormat("revelation/task_data"))
    {
        event->acceptProposedAction();
        m_dropPos      = event->pos();
        m_dropPosValid = true;
        viewport()->update();
    }
}

void RevelationListView::dragLeaveEvent(QDragLeaveEvent* event)
{
    m_dragging     = false;
    m_dropPosValid = false;
    viewport()->update();
}

void RevelationListView::dropEvent(QDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();
    if (mimeData->hasFormat("revelation/task_data"))
    {
        QByteArray  itemData = mimeData->data("revelation/task_data");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        Uint64 taskID;
        dataStream >> taskID;

        auto finder = m_model->s_taskCache.find(taskID);
        if (finder == m_model->s_taskCache.end())
        {
            return;
        }
        TaskPrototype task = finder->second;

        event->acceptProposedAction();

        emit TaskItemReparenting(task, task.m_taskStatus, m_type);
    }
}
