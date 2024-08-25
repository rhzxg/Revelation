#pragma once
#include "Utility/ISettingsToolkit.h"
#include "IRevelationInterface.h"
#include <json/value.h>

class SettingsToolkit : public ISettingsToolkit
{
  public:
    SettingsToolkit(IRevelationInterface* intf);
    ~SettingsToolkit();

    virtual bool GetBoolean(const std::string& key, const std::string& field, bool defaultValue = false) override;
    virtual bool SetBoolean(const std::string& key, const std::string& field, bool value) override;

    virtual int64_t GetInteger(const std::string& key, const std::string& field, int64_t defaultValue = 0) override;
    virtual bool    SetInteger(const std::string& key, const std::string& field, int64_t value) override;

    virtual std::string GetString(const std::string& key, const std::string& field, const std::string& defaultValue = "") override;
    virtual bool        SetString(const std::string& key, const std::string& field, const std::string& value) override;

  private:
    void Initialize();

  private:
    IRevelationInterface* m_interface = nullptr;

    Json::Value m_settings;
};
