#pragma once
#include "IExtensionInterface.h"
#include "IRevelationDataDefine.h"
#include <vector>

class IDataPersistenceInterface : public IExtensionInterface
{
  public:
    virtual void InsertOrReplaceTaskInDatabase(TaskPrototype task) = 0;
    virtual void RemoveTaskFromDatabase(TaskPrototype task)        = 0;

    virtual void ReteiveTasksFromDatabase(std::vector<TaskPrototype>& tasks)              = 0;
    virtual void ReteiveTasksFromDatabase(std::vector<TaskPrototype>& tasks,
                                          const std::string& from, const std::string& to) = 0;
};