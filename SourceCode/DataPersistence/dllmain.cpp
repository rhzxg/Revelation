#include "IExtensionInterface.h"
#include "DataPersistenceInterface.h"

IExtensionInterface* ExtensionEntrance()
{
    return new DataPersistenceInterface();
}