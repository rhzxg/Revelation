#pragma once
#include "IExtensionInterface.h"
#include "ITaskSerialzer.h"
#include "IDatabase.h"

class IDataPersistenceInterface : public IExtensionInterface
{
  public:
    virtual ITaskSerializer* GetTaskSerializer() = 0;

    virtual bool       RegisterDatabase(DatabaseRole role, IDatabase* database) = 0;
    virtual IDatabase* GetDatabase(DatabaseRole role)                           = 0;
};