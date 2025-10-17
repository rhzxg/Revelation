#pragma once

// specify fewer than one parameter
// use json if there are plenty
enum class BroadcastType
{
    CollectNavigationView, // None
    CollectSettingsItem,   // None
    ChangeTheme,           // FluTheme
    ExtInitialized,        // None
    WidgetInitialized,     // None
    DatabaseCreated,       // None
};