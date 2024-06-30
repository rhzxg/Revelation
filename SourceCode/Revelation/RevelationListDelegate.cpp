#include "RevelationListDelegate.h"
#include "RevelationListItem.h"
#include <QPainter>

RevelationListDelegate::RevelationListDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

RevelationListDelegate::~RevelationListDelegate()
{
}

void RevelationListDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QString text = index.data().toString();

    QStyleOptionViewItem opt = option;
    opt.rect.adjust(2, 1, -2, -1);

    RevelationListItem widget;
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
