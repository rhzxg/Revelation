#include "RevelationListModel.h"

RevelationListModel::RevelationListModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

RevelationListModel::~RevelationListModel()
{
}

QModelIndex RevelationListModel::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
    return QAbstractListModel::createIndex(row, column, (qintptr)1);
}

QModelIndex RevelationListModel::parent(const QModelIndex& child) const
{
    return QModelIndex();
}

int RevelationListModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    return m_tasks.size();
}

int RevelationListModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    return 1;
}

bool RevelationListModel::hasChildren(const QModelIndex& parent /*= QModelIndex()*/) const
{
    return false;
}

QVariant RevelationListModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
    return QVariant();
}

Qt::ItemFlags RevelationListModel::flags(const QModelIndex& index) const
{
    return Qt::ItemFlag::NoItemFlags;
}
