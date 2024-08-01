#include "IRevelationInterface.h"
#include "TimeMachineInterface.h"

DLL_EXPORT IExtensionInterface* ExtensionEntrance(IRevelationInterface* intf)
{
    return new TimeMachineInterface(intf);
}
