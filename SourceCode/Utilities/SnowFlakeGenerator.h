#pragma once
#include "Utility/ISnowflakeGenerator.h"
#include <mutex>

class SnowflakeGenerator : public ISnowflakeGenerator
{
  public:
    SnowflakeGenerator();
    ~SnowflakeGenerator();

    virtual Uint64 GenerateNewSnowId() override;

  private:
    Uint64 GetCurrentTimestamp();

    Uint64 WaitUntilNextMillisecond(Uint64 lastTimestamp);

  private:
    Uint64 m_machineId;
    Uint64 m_sequence;
    Uint64 m_lastTimestamp;

    static const Uint64 m_epoch         = 1664496000000ULL; // 2022-09-30 00:00:00 UTC when Revelation was released by G.E.M.
    static const Uint64 m_machineIdBits = 10;
    static const Uint64 m_sequenceBits  = 12;
    static const Uint64 m_maxMachineId  = (1L << m_machineIdBits) - 1;
    static const Uint64 m_maxSequence   = (1L << m_sequenceBits) - 1;
};
