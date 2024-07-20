#pragma once
#include <mutex>
#include <vector>
#include <thread>
#include <functional>

class ITaskCreator
{
  public:
    virtual void RunSyncTask(std::function<void()> task)  = 0;
    virtual void RunAsyncTask(std::function<void()> task) = 0;
    virtual void WaitAllAsyncTasksComplete()              = 0;

  protected:
    std::mutex               m_mutex;
    std::vector<std::thread> m_asyncTasks;
};