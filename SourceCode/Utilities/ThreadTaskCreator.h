#pragma once
#include "Utility/IThreadTaskCreator.h"

class ThreadTaskCreator : public IThreadTaskCreator
{
  public:
    ThreadTaskCreator();
    ~ThreadTaskCreator();

    virtual void RunSyncTask(std::function<void()> task) override;
    virtual void RunAsyncTask(std::function<void()> task) override;
    virtual void WaitAllAsyncTasksComplete() override;
};

