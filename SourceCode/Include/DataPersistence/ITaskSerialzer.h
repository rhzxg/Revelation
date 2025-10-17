#pragma once
#include "IRevelationDataDefine.h"

class ITaskSerializer
{
  public:
    virtual void RecordTask(TaskPrototype task) = 0;
    virtual void RemoveTask(TaskPrototype task) = 0;

    virtual void RetrieveTasks(std::vector<TaskPrototype>& tasks)                          = 0;
    virtual void RetrieveTasks(std::vector<TaskPrototype>& tasks, const std::string& date) = 0;
    virtual void RetrieveTasks(std::vector<TaskPrototype>& tasks,
                               const std::string& from, const std::string& to)             = 0;
};