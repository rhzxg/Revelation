#include "ThreadTaskCreator.h"

ThreadTaskCreator::ThreadTaskCreator()
{
}

ThreadTaskCreator::~ThreadTaskCreator()
{
}

void ThreadTaskCreator::RunSyncTask(std::function<void()> task)
{
    std::thread syncThread(task);
    syncThread.join();
}

void ThreadTaskCreator::RunAsyncTask(std::function<void()> task)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_asyncTasks.emplace_back(std::thread(task));
    m_asyncTasks.back().detach();
}

void ThreadTaskCreator::WaitAllAsyncTasksComplete()
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
