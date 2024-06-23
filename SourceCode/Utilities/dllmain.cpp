#include "IRevelationInterface.h"
#include "UtilityInterface.h"

DLL_EXPORT IExtensionInterface* ExtensionEntrance(IRevelationInterface* intf)
{
    return new UtilityInterface(intf);
}