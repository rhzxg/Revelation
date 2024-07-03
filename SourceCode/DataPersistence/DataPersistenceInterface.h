#pragma once
#include "DataPersistence/IDataPersistenceInterface.h"
#include "IRevelationInterface.h"
#include "IRevelationDataDefine.h"
#include <sqlite3.h>
#include <mutex>

class DataPersistenceInterface : public IDataPersistenceInterface
{
  public:
    DataPersistenceInterface(IRevelationInterface* intf);
    ~DataPersistenceInterface();

    virtual void Initialize() override;
    virtual void Uninitialize() override;

    virtual void InsertOrReplaceTaskInDatabase(TaskPrototype task) override;
    virtual void RemoveTaskFromDatabase(TaskPrototype task) override;
    virtual void ReteiveTasksFromDatabase(std::vector<TaskPrototype>& tasks) override;

  private:
    void ExecDatabaseCreationRoutine();

    void CreateDatabaseFolder();
    void CreateDatabaseByDate();

    bool CreateTable();
    void CollectInheritedRecords();

  private:
    IRevelationInterface* m_revelationIntf = nullptr;

    sqlite3* m_currentDatabase = nullptr;

    std::mutex m_insertMutex;
};