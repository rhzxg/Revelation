#include "SettingsToolkit.h"
#include "JsonOperator.h"

SettingsToolkit::SettingsToolkit(IRevelationInterface* intf)
    : m_interface(intf)
{
    Initialize();
}

SettingsToolkit::~SettingsToolkit()
{
    std::filesystem::path settingsPath = m_interface->GetResourcePath() / "settings" / "settings.json";
    JsonToFile(m_settings, settingsPath);
}

bool SettingsToolkit::GetBoolean(const std::string& key, const std::string& field, bool defaultValue /*= false*/)
{
    if (m_settings.isMember(field))
    {
        Json::Value fieldJson = m_settings[field];
        if (fieldJson.isMember(key))
        {
            return fieldJson[key].asBool();
        }
    }

    return defaultValue;
}

int64_t SettingsToolkit::GetInteger(const std::string& key, const std::string& field, int64_t defaultValue /*= 0*/)
{
    if (m_settings.isMember(field))
    {
        Json::Value fieldJson = m_settings[field];
        if (fieldJson.isMember(key))
        {
            return fieldJson[key].asInt64();
        }
    }

    return defaultValue;
}

std::string SettingsToolkit::GetString(const std::string& key, const std::string& field, const std::string& defaultValue /*= ""*/)
{
    if (m_settings.isMember(field))
    {
        Json::Value fieldJson = m_settings[field];
        if (fieldJson.isMember(key))
        {
            return fieldJson[key].asString();
        }
    }

    return defaultValue;
}

bool SettingsToolkit::SetBoolean(const std::string& key, const std::string& field, bool value)
{
    bool ret = m_settings.isMember(field) && m_settings[field].isMember(key);

    m_settings[field][key] = value;
    return ret;
}

bool SettingsToolkit::SetInteger(const std::string& key, const std::string& field, int64_t value)
{
    bool ret = m_settings.isMember(field) && m_settings[field].isMember(key);

    m_settings[field][key] = value;
    return ret;
}

bool SettingsToolkit::SetString(const std::string& key, const std::string& field, const std::string& value)
{
    bool ret = m_settings.isMember(field) && m_settings[field].isMember(key);

    m_settings[field][key] = value;
    return ret;
}

void SettingsToolkit::Initialize()
{
    std::filesystem::path settingsPath = m_interface->GetResourcePath() / "settings" / "settings.json";
    FileToJson(settingsPath, m_settings);
}
