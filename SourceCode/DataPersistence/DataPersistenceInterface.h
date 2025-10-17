#pragma once
#include "DataPersistence/IDataPersistenceInterface.h"
#include "DataPersistence/IDatabase.h"
#include "IRevelationInterface.h"
#include "IRevelationDataDefine.h"
#include <sqlite3.h>
#include <mutex>

class TaskSerializer;
class DataPersistenceInterface : public IDataPersistenceInterface
{
  public:
    DataPersistenceInterface(IRevelationInterface* intf);
    ~DataPersistenceInterface();

    virtual void Initialize() override;
    virtual void Uninitialize() override;

    virtual ITaskSerializer* GetTaskSerializer() override;

    virtual bool       RegisterDatabase(DatabaseRole role, IDatabase* database) override;
    virtual IDatabase* GetDatabase(DatabaseRole role) override;

  private:
    IRevelationInterface* m_interface = nullptr;

    TaskSerializer* m_taskSerialzier = nullptr;

    std::unordered_map<DatabaseRole, IDatabase*> m_databases;
};
