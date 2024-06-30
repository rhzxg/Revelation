#include "RevelationListModel.h"
#include "IRevelationInterface.h"

RevelationListModel::RevelationListModel(IRevelationInterface* intf, QObject* parent /*= nullptr*/)
    : m_interface(intf), QAbstractListModel(parent)
{
}

RevelationListModel::~RevelationListModel()
{
}

QModelIndex RevelationListModel::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
    return QAbstractListModel::createIndex(row, column, (qintptr)row);
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
    if (!index.isValid() || index.row() >= m_tasks.size())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        return QString::fromStdString(m_tasks.at(index.row()).m_title);
    }

    return QVariant();
}

Qt::ItemFlags RevelationListModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled;
}
