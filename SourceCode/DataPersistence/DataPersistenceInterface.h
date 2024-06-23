#pragma once
#include "DataPersistence/IDataPersistenceInterface.h"
#include "IRevelationInterface.h"
#include <sqlite3.h>

class DataPersistenceInterface : public IDataPersistenceInterface
{
  public:
    DataPersistenceInterface(IRevelationInterface* intf);
    ~DataPersistenceInterface();

    virtual void Initialize() override;
    virtual void Uninitialize() override;

  private:
    void CreateDatabaseFolder();
    void CreateDatabaseByDate();

    // todo: create table
    void CreateTable();

  private:
    IRevelationInterface* m_revelationIntf = nullptr;

    sqlite3* m_currentDatabase = nullptr;
};