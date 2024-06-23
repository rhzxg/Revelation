// defines data prototypes

#pragma once

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
    int64_t     m_id;
    std::string m_title;
    std::string m_desc;
    std::string m_createTime;
    std::string m_startTime;
    std::string m_finishTime;
    std::string m_deadline;
    TaskStatus  m_taskStatus;
    TaskType    m_taskType;
};