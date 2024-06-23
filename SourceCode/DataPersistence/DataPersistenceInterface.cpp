#include "DataPersistenceInterface.h"

DataPersistenceInterface::DataPersistenceInterface(IRevelationInterface* intf)
    : m_revelationIntf(intf)
{
    Initialize();
}

DataPersistenceInterface::~DataPersistenceInterface()
{
}

void DataPersistenceInterface::Initialize()
{
    CreateDatabaseFolder();
}

void DataPersistenceInterface::Uninitialize()
{
}

void DataPersistenceInterface::CreateDatabaseFolder()
{
    std::filesystem::path databaseFolderPath = m_revelationIntf->GetApplicationPath() / "databases";
    if (!std::filesystem::exists(databaseFolderPath))
    {
        std::filesystem::create_directory(databaseFolderPath);
    }
}

void DataPersistenceInterface::CreateDatabaseByDate()
{
}
