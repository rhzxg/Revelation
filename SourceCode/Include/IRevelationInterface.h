#pragma once
#include <filesystem>
#include <unordered_map>
#include "IExtensionInterface.h"

class INavigationViewInterface;
class IRevelationInterface
{
  public:
    virtual std::filesystem::path GetApplicationPath() = 0;
    virtual std::filesystem::path GetResourcePath()    = 0;

    template <typename T>
    T* GetInterfaceById(const std::string& id)
    {
        auto finder = m_interfaces.find(id);
        if (finder != m_interfaces.end())
        {
            return dynamic_cast<T*>(finder->second);
        }
        return nullptr;
    }

    void* GetMainWindow()
    {
        return m_mainWindow;
    }

    INavigationViewInterface* GetNavViewInterface()
    {
        return m_navViewIntf;
    }

  protected:
    std::unordered_map<std::string, IExtensionInterface*> m_interfaces;

    void*                     m_mainWindow  = nullptr;
    INavigationViewInterface* m_navViewIntf = nullptr;
};
