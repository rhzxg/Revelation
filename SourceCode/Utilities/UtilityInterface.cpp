#include "UtilityInterface.h"
#include "DateTimeFormatter.h"

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
