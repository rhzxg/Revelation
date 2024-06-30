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
    
    RevelationListItem widget;
    widget.setGeometry(option.rect);
    widget.setStyleSheet("background: transparent;");

    QPixmap pixmap(option.rect.size());
    pixmap.fill(Qt::transparent);

    widget.render(&pixmap);
    painter->drawPixmap(option.rect, pixmap);
}

QSize RevelationListDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(230, 60);
}
