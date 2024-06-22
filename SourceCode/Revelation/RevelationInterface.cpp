#include "RevelationInterface.h"
#include <filesystem>
#include <regex>
#ifdef WIN32
#include "windows.h"
#endif // WIN32

RevelationInterface::RevelationInterface()
{
    Initialize();
}

RevelationInterface::~RevelationInterface()
{
    Uninitialize();
}

std::filesystem::path RevelationInterface::GetResourcePath()
{
    return m_resourcePath;
}

void RevelationInterface::Initialize()
{
#ifdef WIN32
    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(NULL, buffer, MAX_PATH);
    m_applicationPath = std::filesystem::path(buffer).parent_path();
    m_resourcePath    = m_applicationPath / "resources";
#endif // WIN32

    InitExtensions();
}

void RevelationInterface::Uninitialize()
{
    for (auto& interfacePair : m_interfaces)
    {
        IExtensionInterface* extensionIntf = interfacePair.second;
        extensionIntf->Uninitialize();
        delete extensionIntf;
    }
    m_interfaces.clear();
}

void RevelationInterface::InitExtensions()
{
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
                    HINSTANCE hDLL = LoadLibraryW(subEntry.path().wstring().c_str());
                    if (hDLL == NULL)
                    {
                        continue;
                    }

                    typedef IExtensionInterface* (*ExtensionEntranceFunction)();
                    ExtensionEntranceFunction CreateExtensionInstance = (ExtensionEntranceFunction)GetProcAddress(hDLL, "ExtensionEntrance");
                    if (CreateExtensionInstance == NULL)
                    {
                        continue;
                    }

                    IExtensionInterface* extension = CreateExtensionInstance();
                    if (extension == NULL)
                    {
                        continue;
                    }

                    m_interfaces[fileName] = extension;

                    FreeLibrary(hDLL);
#endif // WIN32
                }
            }
        }
    }
}
