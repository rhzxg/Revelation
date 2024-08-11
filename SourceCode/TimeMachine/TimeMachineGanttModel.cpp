/****************************************************************************
**
** This file is part of the KD Chart library.
**
** SPDX-FileCopyrightText: 2001 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "TimeMachineGanttModel.h"

#include <QDateTime>
#include <QFile>
#include <QList>

#include <QDebug>

#include <KDGanttGlobal>
#include <KDGanttStyleOptionGanttItem>

#include <cassert>

typedef QAbstractItemModel BASE;

static int unnamed_count = 0;

Node::Node(Node* parent)
    : m_parent(parent),
      m_start(QDateTime::currentDateTime()),
      m_end(QDateTime::currentDateTime().addDays(1)),
      m_label(QObject::tr("Unnamed task %1").arg(++unnamed_count))
{
    if (m_parent)
        m_parent->addChild(this);
}

Node::~Node()
{
    while (!m_children.isEmpty())
        delete m_children.front();
    if (m_parent)
        m_parent->removeChild(this);
}

void Node::addChild(Node* child)
{
    child->setParent(this);
    m_children.push_back(child);
}

void Node::insertChild(int i, Node* child)
{
    child->setParent(this);
    m_children.insert(i, child);
}

void Node::removeChild(Node* child)
{
    child->setParent(nullptr);
    m_children.removeAll(child);
}

TimeMachineGanttModel::TimeMachineGanttModel(QObject* parent)
    : QAbstractItemModel(parent), m_root(new Node)
{
}

TimeMachineGanttModel::~TimeMachineGanttModel()
{
    delete m_root;
}

int TimeMachineGanttModel::rowCount(const QModelIndex& idx) const
{
    if (idx.isValid())
        return static_cast<Node*>(idx.internalPointer())->childCount();
    else
        return m_root->childCount();
}

int TimeMachineGanttModel::columnCount(const QModelIndex& idx) const
{
    Q_UNUSED(idx);
    return 1;
}

QModelIndex TimeMachineGanttModel::index(int row, int col, const QModelIndex& parent) const
{
    Node* p = m_root;
    if (parent.isValid())
    {
        p = static_cast<Node*>(parent.internalPointer());
    }
    if (row < 0 || row >= p->childCount())
        return QModelIndex();
    else
        return createIndex(row, col, p->child(row));
}

QModelIndex TimeMachineGanttModel::parent(const QModelIndex& idx) const
{
    if (!idx.isValid())
        return QModelIndex();

    Node* n = static_cast<Node*>(idx.internalPointer());
    assert(n);
    Node* p = n->parent();
    if (p == m_root)
        return QModelIndex();

    Node* pp = p->parent();
    assert(pp);
    return createIndex(pp->childNumber(p), 0, p);
}

QVariant TimeMachineGanttModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();
    switch (section)
    {
        case 0:
            return tr("Date / Task");
        default:
            return QVariant();
    }
}

QVariant TimeMachineGanttModel::data(const QModelIndex& idx, int role) const
{
    if (!idx.isValid())
        return QVariant();

    Node* n = static_cast<Node*>(idx.internalPointer());
    assert(n);
    if (idx.column() == 0)
    {
        switch (role)
        {
            case Qt::DisplayRole:
            case Qt::EditRole:
                return n->label();
            case KDGantt::TextPositionRole:
                return n->position();
        }
    }
    else if (idx.column() == 1)
    {
        switch (role)
        {
            case Qt::DisplayRole:
            case Qt::EditRole:
                return QVariant::fromValue<int>(n->type());
        }
    }
    else if (idx.column() == 2)
    {
        switch (role)
        {
            case Qt::DisplayRole:
                return n->start().date().toString("dd-MM-yyyy");
            case Qt::EditRole:
            case KDGantt::StartTimeRole:
                return n->start();
        }
    }
    else if (idx.column() == 3)
    {
        switch (role)
        {
            case Qt::DisplayRole:
                return n->end().date().toString("dd-MM-yyyy");
            case Qt::EditRole:
            case KDGantt::EndTimeRole:
                return n->end();
        }
    }
    else if (idx.column() == 4 && n->completion() >= 0)
    {
        switch (role)
        {
            case Qt::DisplayRole:
            case Qt::EditRole:
                return n->completion();
        }
    }
    else if (idx.column() == 5)
    {
        switch (role)
        {
            case Qt::DisplayRole:
            case Qt::EditRole:
                return n->label();
            case KDGantt::ItemTypeRole:
                return n->type();
            case KDGantt::StartTimeRole:
                return n->start();
            case KDGantt::EndTimeRole:
                return n->end();
            case KDGantt::TaskCompletionRole:
                if (n->completion() >= 0)
                    return n->completion();
                break;
        }
    }
    return QVariant();
}

bool TimeMachineGanttModel::setData(const QModelIndex& idx, const QVariant& value, int role)
{
    if (!idx.isValid())
        return false;
    qDebug() << "TimeMachineGanttModel::setData" << idx.column() << role << value;

    Node* n = static_cast<Node*>(idx.internalPointer());
    assert(n);

    if (idx.column() == 0)
    {
        switch (role)
        {
            case Qt::DisplayRole:
            case Qt::EditRole:
                n->setLabel(value.toString());
                emit dataChanged(idx, idx);
                break;
            case KDGantt::TextPositionRole:
                n->setPosition(static_cast<KDGantt::StyleOptionGanttItem::Position>(value.toInt()));
                emit dataChanged(idx, idx);
                break;
        }
    }
    return true;
}

bool TimeMachineGanttModel::insertRows(int row, int count, const QModelIndex& parent)
{
    beginInsertRows(parent, row, row + count - 1);
    Node* p = m_root;
    if (parent.isValid())
        p = static_cast<Node*>(parent.internalPointer());
    assert(p);

    for (int i = 0; i < count; ++i)
    {
        Node* n = new Node;
        p->insertChild(row + i, n);
    }
    endInsertRows();
    return true;
}

void TimeMachineGanttModel::clear()
{
    beginResetModel();

    delete m_root;
    m_root = new Node;

    endResetModel();
}

QModelIndex TimeMachineGanttModel::insertNode(Node* node, const QModelIndex& parent /*= QModelIndex()*/)
{
    int row = rowCount(parent);
    beginInsertRows(parent, row, row);

    Node* p = m_root;
    if (parent.isValid())
    {
        p = static_cast<Node*>(parent.internalPointer());
    }

    p->insertChild(row, node);

    endInsertRows();
    return index(row, 0, parent);
}

Qt::ItemFlags TimeMachineGanttModel::flags(const QModelIndex& idx) const
{
    return BASE::flags(idx) | Qt::ItemIsEditable;
}
