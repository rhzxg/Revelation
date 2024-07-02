#pragma once
#include "IExtensionInterface.h"
#include "IRevelationDataDefine.h"

class IDataPersistenceInterface : public IExtensionInterface
{
  public:
    virtual void InsertOrReplaceTask(TaskPrototype task) = 0;
};