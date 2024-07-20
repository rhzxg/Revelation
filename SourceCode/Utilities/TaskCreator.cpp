#include "TaskCreator.h"

TaskCreator::TaskCreator()
{
}

TaskCreator::~TaskCreator()
{
}

void TaskCreator::RunSyncTask(std::function<void()> task)
{
    std::thread syncThread(task);
    syncThread.join();
}

void TaskCreator::RunAsyncTask(std::function<void()> task)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_asyncTasks.emplace_back(std::thread(task));
    m_asyncTasks.back().detach();
}

void TaskCreator::WaitAllAsyncTasksComplete()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& thread : m_asyncTasks)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
    m_asyncTasks.clear();
}
