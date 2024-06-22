#pragma once
#include <filesystem>
#include <unordered_map>
#include "IExtensionInterface.h"

class RevelationInterface
{
  public:
    RevelationInterface();
    ~RevelationInterface();

    std::filesystem::path GetResourcePath();

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

  private:
    void Initialize();
    void Uninitialize();

    void InitExtensions();

  private:
    std::filesystem::path m_applicationPath = "";
    std::filesystem::path m_resourcePath    = "";

    std::unordered_map<std::string, IExtensionInterface*> m_interfaces;
};
