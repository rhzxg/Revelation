#include "IExtensionInterface.h"
#include "DataPersistenceInterface.h"

extern "C" __declspec(dllexport) IExtensionInterface* ExtensionEntrance()
{
    return new DataPersistenceInterface();
}