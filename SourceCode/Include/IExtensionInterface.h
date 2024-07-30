#pragma once

#define DLL_EXPORT extern "C" __declspec(dllexport)

class IExtensionInterface
{
  public:
    virtual ~IExtensionInterface() {}

    virtual void Initialize()   = 0;
    virtual void Uninitialize() = 0;

    virtual void CollectNavigationViews(){};
};