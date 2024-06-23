#include "IRevelationInterface.h"
#include "DataPersistenceInterface.h"

DLL_EXPORT IExtensionInterface* ExtensionEntrance(IRevelationInterface* intf)
{
    return new DataPersistenceInterface(intf);
}