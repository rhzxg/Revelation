#include "RevelationInterface.h"
#include "Utility/IUtilityInterface.h"
#include "CommonWidgetInterface.h"
#include <regex>

RevelationInterface::RevelationInterface(void* mainWindow, ICommonWidgetInterface* commonWidgetIntf)
{
    m_mainWindow       = mainWindow;
    m_commonWidgetIntf = commonWidgetIntf;

    Initialize();
}

RevelationInterface::~RevelationInterface()
{
    Uninitialize();
}

std::filesystem::path RevelationInterface::GetApplicationPath()
{
    return m_applicationPath;
}

std::filesystem::path RevelationInterface::GetResourcePath()
{
    return m_resourcePath;
}

void RevelationInterface::Broadcast(BroadcastType broadcastType, const std::any& param /*= std::any()*/)
{
    for (auto& interfacePair : m_interfaces)
    {
        IExtensionInterface* extensionIntf = interfacePair.second;
        extensionIntf->HandleBroadcast(broadcastType, param);
    }

    HandleBroadcast(broadcastType, param);
}

void RevelationInterface::InitExtensions()
{
    for (auto& interfacePair : m_interfaces)
    {
        IExtensionInterface* extensionIntf = interfacePair.second;
        extensionIntf->Initialize();
    }

    Broadcast(BroadcastType::ExtInitialized);
}

void RevelationInterface::Initialize()
{
    auto commonWidgetIntf = dynamic_cast<CommonWidgetInterface*>(m_commonWidgetIntf);
    commonWidgetIntf->SetInterface(this);

#ifdef WIN32
    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(NULL, buffer, MAX_PATH);
    m_applicationPath = std::filesystem::path(buffer).parent_path();
    m_resourcePath    = m_applicationPath / "resources";
#endif // WIN32

    LoadExtensions();

    commonWidgetIntf->InitializeConfig();
}

void RevelationInterface::Uninitialize()
{
    // wait all async tasks complete
    auto taskCreator = GetInterfaceById<IUtilityInterface>("Utility")->GetTaskCreator();
    taskCreator->WaitAllAsyncTasksComplete();

    for (auto& interfacePair : m_interfaces)
    {
        IExtensionInterface* extensionIntf = interfacePair.second;
        extensionIntf->Uninitialize();
        delete extensionIntf;
    }
    m_interfaces.clear();

    // dispose strong memory bindings
    m_commonWidgetIntf->DisposeMemoryBindings();

#ifdef WIN32
    for (HINSTANCE library : m_libraries)
    {
        FreeLibrary(library);
    }
    m_libraries.clear();
#endif // WIN32
}

void RevelationInterface::LoadExtensions()
{
#ifdef WIN32
    WCHAR currDir[MAX_PATH] = {0};
    GetCurrentDirectoryW(MAX_PATH, currDir);
#endif // WIN32

    std::filesystem::path extensionPath = m_applicationPath / "extensions";
    for (const auto& entry : std::filesystem::directory_iterator(extensionPath))
    {
        if (entry.is_directory())
        {
            for (const auto& subEntry : std::filesystem::directory_iterator(entry.path()))
            {
                std::regex  camelCaseRegex("([A-Z][a-z]*)*");
                std::string fileName      = subEntry.path().stem().string();
                std::string fileExtension = subEntry.path().extension().string();
                if (fileExtension == ".dll" && std::regex_match(fileName, camelCaseRegex))
                {
#ifdef WIN32
                    SetCurrentDirectoryW(entry.path().wstring().c_str());
                    HINSTANCE library = LoadLibraryW(subEntry.path().wstring().c_str());
                    if (nullptr == library)
                    {
                        DWORD errorCode = GetLastError();
                        continue;
                    }

                    typedef IExtensionInterface* (*ExtensionEntranceFunction)(IRevelationInterface*);
                    ExtensionEntranceFunction CreateExtensionInstance = (ExtensionEntranceFunction)GetProcAddress(library, "ExtensionEntrance");
                    if (nullptr == CreateExtensionInstance)
                    {
                        FreeLibrary(library);
                        continue;
                    }

                    IExtensionInterface* extension = CreateExtensionInstance(this);
                    if (nullptr == extension)
                    {
                        FreeLibrary(library);
                        continue;
                    }

                    m_libraries.push_back(library);
                    m_interfaces[fileName] = extension;
#endif // WIN32
                }
            }
        }
    }

#ifdef WIN32
    SetCurrentDirectoryW(currDir);
#endif // WIN32
}

void RevelationInterface::HandleBroadcast(BroadcastType broadcastType, const std::any& param /*= std::any()*/)
{
    if (broadcastType == BroadcastType::WidgetInitialized)
    {
        auto settingsToolkit = GetInterfaceById<IUtilityInterface>("Utility")->GetSettingsToolkit();
        auto themeStr        = settingsToolkit->GetString("Theme", "Revelation", "Light");

        if (themeStr == "Dark")
        {
            FluThemeUtils::getUtils()->setTheme(FluTheme::Dark);
            Broadcast(BroadcastType::ChangeTheme, FluTheme::Dark);
        }
    }
}
