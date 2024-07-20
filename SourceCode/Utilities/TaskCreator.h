#pragma once
#include "Utility/ITaskCreator.h"

class TaskCreator : public ITaskCreator
{
  public:
    TaskCreator();
    ~TaskCreator();

    virtual void RunSyncTask(std::function<void()> task) override;
    virtual void RunAsyncTask(std::function<void()> task) override;
    virtual void WaitAllAsyncTasksComplete() override;
};

