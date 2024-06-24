#pragma once
#include "IRevelationDataDefine.h"

class ISnowFlakeGenerator
{
  public:
    virtual Uint64 GenerateNewSnowId() = 0;
};