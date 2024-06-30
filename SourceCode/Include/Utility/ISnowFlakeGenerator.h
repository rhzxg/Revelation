#pragma once
#include "IRevelationDataDefine.h"

class ISnowflakeGenerator
{
  public:
    virtual Uint64 GenerateNewSnowId() = 0;
};