#pragma once
#include "IRevelationInterface.h"
#include "IExtensionInterface.h"

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
    std::filesystem::path m_applicationPath = "";
    std::filesystem::path m_resourcePath    = "";
};
