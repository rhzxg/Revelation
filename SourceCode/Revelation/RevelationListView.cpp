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

    m_model    = new RevelationListModel(m_interface, this);
    m_delegate = new RevelationListDelegate(m_interface, this);
    this->setModel(m_model);
    this->setItemDelegate(m_delegate);

    m_model->SetModelType(m_type);
}

void RevelationListView::InitSignalSlots()
{
    connect(this, &QListView::clicked, this, &RevelationListView::OnItemClicked);
}

void RevelationListView::startDrag(Qt::DropActions supportedActions)
{
    QModelIndexList indexes = selectedIndexes();
    if (indexes.count() > 0)
    {
        QModelIndex index = indexes.first();
        QByteArray  itemData;
        QDataStream dataStream(&itemData, QIODevice::WriteOnly);

        Uint64 taskID = m_model->m_tasks.at(index.row()).m_id;
        dataStream << taskID;

        QRect   rect = this->visualRect(index);
        QPixmap pixmap(rect.size());
        pixmap.fill(Qt::transparent);

        QPainter painter(&pixmap);
        this->viewport()->render(&painter, QPoint(), QRegion(rect));
        painter.end();

        QMimeData* mimeData = new QMimeData;
        mimeData->setData("revelation/task_data", itemData);

        QDrag* drag = new QDrag(this);
        drag->setHotSpot(pixmap.rect().center());
        drag->setMimeData(mimeData);
        drag->setPixmap(pixmap);
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
        m_dropPos      = event->position().toPoint();
        viewport()->update();
    }
}

void RevelationListView::dragMoveEvent(QDragMoveEvent* event)
{
    if (event->mimeData()->hasFormat("revelation/task_data"))
    {
        event->acceptProposedAction();
        m_dropPos      = event->position().toPoint();
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

void RevelationListView::OnItemClicked(const QModelIndex& index)
{
    if (!index.isValid())
    {
        return;
    }

    int taskIndex = (int)index.internalPointer();
    if (taskIndex >= m_model->m_tasks.size())
    {
        return;
    }

    TaskPrototype task = m_model->m_tasks.at(taskIndex);
    emit          TaskItemSelected(task);
}
