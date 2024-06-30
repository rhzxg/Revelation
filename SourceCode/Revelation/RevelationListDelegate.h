#pragma once

#include <QStyledItemDelegate >

class RevelationListDelegate : public QStyledItemDelegate
{
    Q_OBJECT

  public:
    RevelationListDelegate(QObject* parent);
    ~RevelationListDelegate();

    virtual void  paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};
