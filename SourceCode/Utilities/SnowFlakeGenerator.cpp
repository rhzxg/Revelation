#include "SnowflakeGenerator.h"

SnowflakeGenerator::SnowflakeGenerator()
{
    m_machineId     = 1;
    m_sequence      = 0;
    m_lastTimestamp = 0;
}

SnowflakeGenerator::~SnowflakeGenerator()
{
}

Uint64 SnowflakeGenerator::GenerateNewSnowId()
{
    uint64_t timestamp = GetCurrentTimestamp();
    if (timestamp < m_lastTimestamp)
    {
        // never happens
    }

    if (timestamp == m_lastTimestamp)
    {
        m_sequence = (m_sequence + 1) & m_maxSequence;
        if (m_sequence == 0)
        {
            timestamp = WaitUntilNextMillisecond(m_lastTimestamp);
        }
    }
    else
    {
        m_sequence = 0;
    }

    m_lastTimestamp = timestamp;
    return ((timestamp - m_epoch) << (m_machineIdBits + m_sequenceBits)) |
           (m_machineId << m_sequenceBits) |
           m_sequence;
}

Uint64 SnowflakeGenerator::GetCurrentTimestamp()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::system_clock::now().time_since_epoch())
        .count();
}

Uint64 SnowflakeGenerator::WaitUntilNextMillisecond(Uint64 lastTimestamp)
{
    uint64_t timestamp = GetCurrentTimestamp();
    while (timestamp <= lastTimestamp)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        timestamp = GetCurrentTimestamp();
    }
    return timestamp;
}
