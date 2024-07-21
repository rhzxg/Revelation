#pragma once
#include <QStyledItemDelegate >

class IRevelationInterface;

class RevelationListDelegate : public QStyledItemDelegate
{
    Q_OBJECT

  public:
    RevelationListDelegate(IRevelationInterface* intf, QObject* parent = nullptr);
    ~RevelationListDelegate();

    void SetMouseHoverRow(int hoverRow);

  private:
    virtual void  paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

  private:
    IRevelationInterface* m_interface = nullptr;

    int m_hoverRow = -1;
};
