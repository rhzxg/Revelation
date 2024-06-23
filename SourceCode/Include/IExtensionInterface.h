#pragma once

#define DLL_EXPORT extern "C" __declspec(dllexport)

class IExtensionInterface
{
  public:
    virtual void Initialize()   = 0;
    virtual void Uninitialize() = 0;
};