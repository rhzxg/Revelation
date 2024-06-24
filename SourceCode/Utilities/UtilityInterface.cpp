#include "UtilityInterface.h"
#include "DateTimeFormatter.h"
#include "SnowFlakeGenerator.h"

UtilityInterface::UtilityInterface(IRevelationInterface* intf)
{
}

UtilityInterface::~UtilityInterface()
{
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

ISnowFlakeGenerator* UtilityInterface::GetSnowFlakeGenerator()
{
    if (nullptr == m_snowFlakeGenerator)
    {
        m_snowFlakeGenerator = new SnowFlakeGenerator();
    }
    return m_snowFlakeGenerator;
}
