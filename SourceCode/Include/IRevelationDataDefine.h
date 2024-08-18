// defines data prototypes
#pragma once
#include <string>
#include <vector>

using Int64  = int64_t;
using Uint64 = uint64_t;

enum class TaskStatus
{
    None = 0,
    Todo,
    Doing,
    Testing,
    Done
};

enum class TaskType
{
    None = 0,
    Bug,
    Feature,
    Test,
    UI
};

enum class TaskTag
{
    None = 0,
    Routine,
    Inherited
};

struct TaskPrototype
{
    Uint64      m_id;
    std::string m_title;
    std::string m_desc;
    std::string m_createTime;
    std::string m_startTime;
    std::string m_finishTime;
    std::string m_deadline;
    TaskStatus  m_taskStatus;
    TaskType    m_taskType;
    TaskTag     m_taskTag;

    bool operator==(const TaskPrototype& other) const
    {
        return this->m_id == other.m_id;
    }

    bool operator!=(const TaskPrototype& other) const
    {
        return this->m_id != other.m_id;
    }
};

using DateToTasks = std::pair<std::string, std::vector<TaskPrototype>>;
