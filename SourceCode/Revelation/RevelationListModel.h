#pragma once
#include <QAbstractListModel>
#include "DataModel/IDataModelInterface.h"
#include "IRevelationDataDefine.h"

class IRevelationInterface;

class RevelationListModel : public QAbstractListModel, public IDataModelInterface
{
    Q_OBJECT

    friend class RevelationListView;
    friend class RevelationListDelegate;

  public:
    RevelationListModel(IRevelationInterface* intf, QObject* parent = nullptr);
    ~RevelationListModel();

  private:
    virtual QModelIndex   index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    virtual QModelIndex   parent(const QModelIndex& child) const override;
    virtual int           rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual int           columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual bool          hasChildren(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant      data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const override;

  private:
    IRevelationInterface* m_interface = nullptr;

    std::vector<TaskPrototype> m_tasks;
};
