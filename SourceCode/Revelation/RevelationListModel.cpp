#include "RevelationListModel.h"
#include "IRevelationInterface.h"
#include "Utility/IUtilityInterface.h"

// shared cache
std::unordered_map<Uint64, TaskPrototype> RevelationListModel::s_taskCache;

RevelationListModel::RevelationListModel(IRevelationInterface* intf, QObject* parent /*= nullptr*/)
    : m_interface(intf), QAbstractListModel(parent)
{
}

RevelationListModel::~RevelationListModel()
{
}

void RevelationListModel::InsertTaskItem(const TaskPrototype& task, bool fromDatabase /*= false*/)
{
    // shared cache
    s_taskCache.emplace(task.m_id, task);

    std::lock_guard<std::mutex> lock(m_mutex);
    auto                        timeFormatter = m_interface->GetInterfaceById<IUtilityInterface>("Utility")->GetDateTimeFormatter();
    auto                        it            = std::lower_bound(m_tasks.begin(), m_tasks.end(), task, [&](TaskPrototype t1, TaskPrototype t2) {
        time_t t1Time = timeFormatter->ConvertDateTimeFromString(t1.m_createTime);
        time_t t2Time = timeFormatter->ConvertDateTimeFromString(t2.m_createTime);

        return t1Time < t2Time;
    });
    m_tasks.insert(it, task);

    layoutChanged();

    if (!fromDatabase)
    {
        // update database
    }
}

void RevelationListModel::RemoveTaskItem(const TaskPrototype& task)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto                        finder = std::find(m_tasks.begin(), m_tasks.end(), task);
    if (finder != m_tasks.end())
    {
        m_tasks.erase(finder);

        layoutChanged();
    }

    // shared cache
    s_taskCache.erase(task.m_id);
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
