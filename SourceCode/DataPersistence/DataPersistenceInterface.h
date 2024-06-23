#pragma once
#include "DataPersistence/IDataPersistenceInterface.h"
#include "IRevelationInterface.h"

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

  private:
    IRevelationInterface* m_revelationIntf = nullptr;
};