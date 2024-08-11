/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H
#include <QAbstractItemModel>
#include <KDGanttStyleOptionGanttItem>

class Node
{
  public:
    explicit Node(Node* parent = nullptr);
    virtual ~Node();

    void addChild(Node*);
    void insertChild(int i, Node*);
    void removeChild(Node*);

    Node* parent() const
    {
        return m_parent;
    }
    int childCount() const
    {
        return m_children.count();
    }
    int childNumber(Node* n) const
    {
        return m_children.indexOf(n);
    }
    Node* child(int i) const
    {
        return m_children.at(i);
    }
    void setParent(Node* p)
    {
        m_parent = p;
    }

    void setStart(const QDateTime& dt)
    {
        m_bStart = m_start;
        m_start  = dt;
    }
    void setEnd(const QDateTime& dt)
    {
        if (m_end.isValid())
            m_bEnd = m_end;
        m_end = dt;
    }
    void setLabel(const QString& str)
    {
        m_label = str;
    }
    void setType(int t)
    {
        m_type = static_cast<KDGantt::ItemType>(t);
        if (!m_start.isValid())
            m_start = m_bStart;
        if (!m_end.isValid())
            m_end = m_bEnd;
    }
    void setCompletion(int c)
    {
        m_completion = c;
    }
    void setPosition(KDGantt::StyleOptionGanttItem::Position p)
    {
        m_position = p;
    }

    QDateTime start() const
    {
        return m_start;
    }
    QDateTime end() const
    {
        return m_end;
    }

    QString label() const
    {
        return m_label;
    }

    KDGantt::ItemType type() const
    {
        return m_type;
    }

    int completion() const
    {
        return m_completion;
    }

    KDGantt::StyleOptionGanttItem::Position position() const
    {
        return m_position;
    }

  private:
    Node*        m_parent;
    QList<Node*> m_children;

    KDGantt::ItemType                       m_type = KDGantt::TypeTask;
    QDateTime                               m_start, m_end;
    QDateTime                               m_bStart, m_bEnd;
    QString                                 m_label;
    int                                     m_completion = -1;
    KDGantt::StyleOptionGanttItem::Position m_position   = KDGantt::StyleOptionGanttItem::Right;
};

class TimeMachineGanttModel : public QAbstractItemModel
{
    Q_OBJECT
  public:
    explicit TimeMachineGanttModel(QObject* parent = nullptr);
    ~TimeMachineGanttModel() override;

    virtual int rowCount(const QModelIndex& idx) const override;
    virtual int columnCount(const QModelIndex& idx) const override;

    virtual QModelIndex index(int row, int col, const QModelIndex& parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex& idx) const override;

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual QVariant data(const QModelIndex& idx, int role = Qt::DisplayRole) const override;
    virtual bool     setData(const QModelIndex& idx, const QVariant& value, int role = Qt::DisplayRole) override;

    virtual Qt::ItemFlags flags(const QModelIndex&) const override;

    virtual bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

    void clear();

  private:
    Node* m_root;
};

#endif /* PROJECTMODEL_H */
