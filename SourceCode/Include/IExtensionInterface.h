#pragma once

class IExtensionInterface
{
    virtual void Initialize()   = 0;
    virtual void Uninitialize() = 0;
};