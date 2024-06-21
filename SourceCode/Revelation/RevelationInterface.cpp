#include "RevelationInterface.h"
#include <filesystem>
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
}

void RevelationInterface::InitExtensions()
{
    std::filesystem::path extensionPath = m_applicationPath / "extensions";
}
