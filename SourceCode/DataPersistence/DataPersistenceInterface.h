#pragma once
#include "IDataPersistenceInterface.h"

class DataPersistenceInterface : public IDataPersistenceInterface
{
  public:
    DataPersistenceInterface();
    ~DataPersistenceInterface();

    virtual void Initialize() override;
    virtual void Uninitialize() override;
};