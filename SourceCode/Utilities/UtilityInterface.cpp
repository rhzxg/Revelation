#include "UtilityInterface.h"
#include "DateTimeFormatter.h"
#include "SettingsToolkit.h"
#include "SnowflakeGenerator.h"
#include "TaskCreator.h"

UtilityInterface::UtilityInterface(IRevelationInterface* intf)
    : m_interface(intf)
{
}

UtilityInterface::~UtilityInterface()
{
    delete m_dateTimeFormatter;
    delete m_settingsToolkit;
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
        m_dateTimeFormatter = new DateTimeFormatter;
    }
    return m_dateTimeFormatter;
}

ISettingsToolkit* UtilityInterface::GetSettingsToolkit()
{
    if (nullptr == m_settingsToolkit)
    {
        m_settingsToolkit = new SettingsToolkit(m_interface);
    }
    return m_settingsToolkit;
}

ISnowflakeGenerator* UtilityInterface::GetSnowflakeGenerator()
{
    if (nullptr == m_snowFlakeGenerator)
    {
        m_snowFlakeGenerator = new SnowflakeGenerator;
    }
    return m_snowFlakeGenerator;
}

ITaskCreator* UtilityInterface::GetTaskCreator()
{
    if (nullptr == m_taskCreator)
    {
        m_taskCreator = new TaskCreator;
    }
    return m_taskCreator;
}
