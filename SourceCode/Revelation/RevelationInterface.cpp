#include "RevelationInterface.h"
#include <regex>

RevelationInterface::RevelationInterface()
{
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

#ifdef WIN32
    for (HINSTANCE library : m_libraries)
    {
        FreeLibrary(library);
    }
    m_libraries.clear();
#endif // WIN32
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
                    HINSTANCE library = LoadLibraryW(subEntry.path().wstring().c_str());
                    if (nullptr == library)
                    {
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

    for (auto& interfacePair : m_interfaces)
    {
        IExtensionInterface* extensionIntf = interfacePair.second;
        extensionIntf->Initialize();
    }
}
