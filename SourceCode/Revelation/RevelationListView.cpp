#include "RevelationListView.h"
#include "RevelationListModel.h"
#include "RevelationListDelegate.h"
#include "IRevelationInterface.h"
#include <QListWidgetItem>
#include <QPainter>
#include <QMimeData>
#include <QDrag>
#include "FluStyleSheetUitls.h"

RevelationListView::RevelationListView(IRevelationInterface* intf, QWidget* parent /*= nullptr*/)
    : m_interface(intf), QListView(parent)
{
    Initialize();
}

RevelationListView::~RevelationListView()
{
}

void RevelationListView::SetViewType(TaskStatus viewType)
{
    m_type = viewType;

    m_model->SetModelType(m_type);
}

void RevelationListView::Initialize()
{
    InitWidget();
    InitSignalSlots();
}

void RevelationListView::InitWidget()
{
    this->setMouseTracking(true);
    this->setDragEnabled(true);
    this->setAcceptDrops(true);
    this->setDropIndicatorShown(false);
    this->setDragDropMode(QAbstractItemView::DragDrop);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_model    = new RevelationListModel(m_interface, this);
    m_delegate = new RevelationListDelegate(m_interface, this);
    this->setModel(m_model);
    this->setItemDelegate(m_delegate);

    FluStyleSheetUitls::setQssByFileName("/resources/qss/light/RevelationListView.qss", this);
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

        QRect   rect             = this->visualRect(index);
        qreal   devicePixelRatio = this->devicePixelRatio();
        QSize   pixmapSize       = rect.size() * devicePixelRatio;
        QPixmap pixmap(pixmapSize);
        pixmap.setDevicePixelRatio(devicePixelRatio);
        pixmap.fill(Qt::transparent);

        QPainter painter(&pixmap);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setRenderHint(QPainter::TextAntialiasing, true);
        this->viewport()->render(&painter, QPoint(), QRegion(rect));
        painter.end();

        QMimeData* mimeData = new QMimeData;
        mimeData->setData("revelation/task_data", itemData);

        QDrag* drag = new QDrag(this);
        drag->setHotSpot(pixmap.rect().center() / devicePixelRatio);
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
        this->viewport()->update();
    }
}

void RevelationListView::dragMoveEvent(QDragMoveEvent* event)
{
    if (event->mimeData()->hasFormat("revelation/task_data"))
    {
        event->acceptProposedAction();
        this->viewport()->update();
    }
}

void RevelationListView::dragLeaveEvent(QDragLeaveEvent* event)
{
    this->viewport()->update();
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

        m_dropUpdateFlag = true;
        event->acceptProposedAction();

        emit TaskItemReparenting(task, task.m_taskStatus, m_type);
    }
}

void RevelationListView::mouseMoveEvent(QMouseEvent* event)
{
    QModelIndex index = indexAt(event->pos());
    m_delegate->SetMouseHoverRow(index.isValid() ? index.row() : -1);

    if (m_dropUpdateFlag)
    {
        this->viewport()->update();
        m_dropUpdateFlag = false;
    }

    QListView::mouseMoveEvent(event);
}

void RevelationListView::leaveEvent(QEvent* event)
{
    m_delegate->SetMouseHoverRow(-1);
    QListView::leaveEvent(event);
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
