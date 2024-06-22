#pragma once

class IExtensionInterface
{
  public:
    virtual void Initialize()   = 0;
    virtual void Uninitialize() = 0;
};