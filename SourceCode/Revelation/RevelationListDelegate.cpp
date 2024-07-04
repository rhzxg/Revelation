#include "RevelationListDelegate.h"
#include "RevelationListItem.h"
#include "RevelationListModel.h"
#include <QPainter>

RevelationListDelegate::RevelationListDelegate(IRevelationInterface* intf, QObject* parent /*= nullptr*/)
    : m_interface(intf), QStyledItemDelegate(parent)
{
}

RevelationListDelegate::~RevelationListDelegate()
{
}

void RevelationListDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    int                  taskIndex = (int)index.internalPointer();
    RevelationListModel* model     = (RevelationListModel*)index.model();
    if (taskIndex >= model->m_tasks.size())
    {
        return;
    }

    TaskPrototype task = model->m_tasks.at(taskIndex);

    QStyleOptionViewItem opt = option;
    opt.rect.adjust(2, 1, -2, -1);

    RevelationListItem widget;
    widget.SetTaskData(task);
    widget.setGeometry(opt.rect);
    widget.setStyleSheet("background: transparent;");

    QPixmap pixmap(opt.rect.size());
    pixmap.fill(Qt::transparent);

    widget.render(&pixmap);
    painter->drawPixmap(opt.rect, pixmap);
}

QSize RevelationListDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QSize preffered = QStyledItemDelegate::sizeHint(option, index);
    int   width     = preffered.width();
    int   height    = 60;
    return QSize(width, height);
}
