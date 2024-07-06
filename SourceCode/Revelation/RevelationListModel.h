#pragma once
#include <QAbstractListModel>
#include "IRevelationDataDefine.h"
#include <mutex>

class IRevelationInterface;

class RevelationListModel : public QAbstractListModel
{
    Q_OBJECT

    friend class RevelationListView;
    friend class RevelationListDelegate;

  public:
    RevelationListModel(IRevelationInterface* intf, QObject* parent = nullptr);
    ~RevelationListModel();

    void SetModelType(TaskStatus modelType);

    void InsertTaskItem(TaskPrototype& task, bool fromDatabase = false);
    void ChangeTaskItem(const TaskPrototype& task);
    void RemoveTaskItem(const TaskPrototype& task);

  private:
    virtual QModelIndex   index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    virtual QModelIndex   parent(const QModelIndex& child) const override;
    virtual int           rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual int           columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual bool          hasChildren(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant      data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const override;

    private:
    void UpdateTaskData(TaskPrototype& task);

  private:
    IRevelationInterface* m_interface = nullptr;

    // cache
    static std::unordered_map<Uint64, TaskPrototype> s_taskCache;

    TaskStatus m_type = TaskStatus::None;

    std::mutex                 m_mutex;
    std::vector<TaskPrototype> m_tasks;
};
