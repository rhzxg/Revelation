#include "UtilityInterface.h"
#include "DateTimeFormatter.h"
#include "SnowflakeGenerator.h"
#include "TaskCreator.h"

UtilityInterface::UtilityInterface(IRevelationInterface* intf)
{
}

UtilityInterface::~UtilityInterface()
{
    delete m_dateTimeFormatter;
    delete m_snowFlakeGenerator;
    delete m_taskCreator;
}

void UtilityInterface::Initialize()
{
}

void UtilityInterface::Uninitialize()
{
}

IDateTimeFormatter* UtilityInterface::GetDateTimeFormatter()
{
    if (nullptr == m_dateTimeFormatter)
    {
        m_dateTimeFormatter = new DateTimeFormatter();
    }
    return m_dateTimeFormatter;
}

ISnowflakeGenerator* UtilityInterface::GetSnowflakeGenerator()
{
    if (nullptr == m_snowFlakeGenerator)
    {
        m_snowFlakeGenerator = new SnowflakeGenerator();
    }
    return m_snowFlakeGenerator;
}

ITaskCreator* UtilityInterface::GetTaskCreator()
{
    if (nullptr == m_taskCreator)
    {
        m_taskCreator = new TaskCreator();
    }
    return m_taskCreator;
}
