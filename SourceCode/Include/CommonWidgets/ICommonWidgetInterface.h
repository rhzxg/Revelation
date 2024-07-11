#pragma once
#include "IExtensionInterface.h"
#include <QWidget>

class ICommonWidgetInterface : public IExtensionInterface
{
  public:
    virtual QWidget* CreateRevelationCalendarWidget() = 0;
};