#pragma once
#include "IRevelationInterface.h"
#include "IExtensionInterface.h"
#include "ICommonWidgetInterface.h"

#ifdef WIN32
#include <windows.h>
#endif // WIN32

class RevelationInterface : public IRevelationInterface
{
  public:
    RevelationInterface(void* mainWindow, ICommonWidgetInterface* commonWidgetIntf);
    ~RevelationInterface();

    virtual std::filesystem::path GetApplicationPath() override;
    virtual std::filesystem::path GetResourcePath() override;

    virtual void Broadcast(BroadcastType broadcastType, const std::any& param = std::any()) override;

    void InitExtensions();

  private:
    void Initialize();
    void Uninitialize();

    void LoadExtensions();

    void HandleBroadcast(BroadcastType broadcastType, const std::any& param = std::any());

  private:
#ifdef WIN32
    std::vector<HINSTANCE> m_libraries;
#endif // WIN32

    std::filesystem::path m_applicationPath = "";
    std::filesystem::path m_resourcePath    = "";
};
