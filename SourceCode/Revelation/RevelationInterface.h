#pragma once
#include "IRevelationInterface.h"
#include "IExtensionInterface.h"

#ifdef WIN32
#include "windows.h"
#endif // WIN32

class RevelationInterface : public IRevelationInterface
{
  public:
    RevelationInterface();
    ~RevelationInterface();

    virtual std::filesystem::path GetApplicationPath() override;
    virtual std::filesystem::path GetResourcePath() override;

  private:
    void Initialize();
    void Uninitialize();

    void InitExtensions();

  private:
#ifdef WIN32
    std::vector<HINSTANCE> m_libraries;
#endif // WIN32

    std::filesystem::path m_applicationPath = "";
    std::filesystem::path m_resourcePath    = "";
};
