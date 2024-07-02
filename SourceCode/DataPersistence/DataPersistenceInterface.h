#pragma once
#include "DataPersistence/IDataPersistenceInterface.h"
#include "IRevelationInterface.h"
#include "IRevelationDataDefine.h"
#include <sqlite3.h>

class DataPersistenceInterface : public IDataPersistenceInterface
{
  public:
    DataPersistenceInterface(IRevelationInterface* intf);
    ~DataPersistenceInterface();

    virtual void Initialize() override;
    virtual void Uninitialize() override;

    virtual void InsertOrReplaceTask(TaskPrototype task) override;

  private:
    void ExecDatabaseCreationRoutine();

    void CreateDatabaseFolder();
    void CreateDatabaseByDate();

    void CreateTable();
    void CollectInheritedRecords();

  private:
    IRevelationInterface* m_revelationIntf = nullptr;

    sqlite3* m_currentDatabase = nullptr;
};