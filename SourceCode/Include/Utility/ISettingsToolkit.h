#pragma once
#include <string>

class ISettingsToolkit
{
  public:
    virtual bool GetBoolean(const std::string& key, const std::string& field, bool defaultValue = false) = 0;
    virtual bool SetBoolean(const std::string& key, const std::string& field, bool value)                = 0;

    virtual int64_t GetInteger(const std::string& key, const std::string& field, int64_t defaultValue = 0) = 0;
    virtual bool    SetInteger(const std::string& key, const std::string& field, int64_t value)            = 0;

    virtual std::string GetString(const std::string& key, const std::string& field, const std::string& defaultValue = "") = 0;
    virtual bool        SetString(const std::string& key, const std::string& field, const std::string& value)             = 0;
};