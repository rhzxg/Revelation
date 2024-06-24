// defines data prototypes
#pragma once
#include <string>

using Int64  = int64_t;
using Uint64 = uint64_t;

enum class TaskStatus
{
    Todo,
    Programming,
    Testing,
    Done
};

enum class TaskType
{
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
};