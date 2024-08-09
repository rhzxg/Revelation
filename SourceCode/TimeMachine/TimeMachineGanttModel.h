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

    virtual bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

    virtual Qt::ItemFlags flags(const QModelIndex&) const override;

    bool load(const QString& filename);
    bool save(const QString& filename);

  private:
    class Node;

    Node* m_root;
};

#endif /* PROJECTMODEL_H */
