// defines data prototypes
#pragma once
#include <string>

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

struct TaskPrototype
{
    Int64       m_id;
    std::string m_title;
    std::string m_desc;
    std::string m_createTime;
    std::string m_startTime;
    std::string m_finishTime;
    std::string m_deadline;
    TaskStatus  m_taskStatus;
    TaskType    m_taskType;

    bool operator==(const TaskPrototype& other) const
    {
        return this->m_id == other.m_id;
    }
};