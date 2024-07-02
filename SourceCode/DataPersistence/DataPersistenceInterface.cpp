#include "DataPersistenceInterface.h"
#include "Utility/IUtilityInterface.h"
#include "Utility/IDateTimeFormatter.h"
#include <fstream>

DataPersistenceInterface::DataPersistenceInterface(IRevelationInterface* intf)
    : m_revelationIntf(intf)
{
}

DataPersistenceInterface::~DataPersistenceInterface()
{
}

void DataPersistenceInterface::Initialize()
{
    CreateDatabaseFolder();
    CreateDatabaseByDate();
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
    // todo: update database when it is another day
    IUtilityInterface* utilityIntf = m_revelationIntf->GetInterfaceById<IUtilityInterface>("Utility");
    if (nullptr == utilityIntf)
    {
        return;
    }

    std::string           currentDate      = utilityIntf->GetDateTimeFormatter()->GetCurrentDateTimeString(TimeMask::YMD);
    std::string           suffix           = ".rev";
    std::string           fileName         = currentDate + suffix;
    std::filesystem::path databaseFilePath = m_revelationIntf->GetApplicationPath() / "databases" / fileName;

    sqlite3_open(databaseFilePath.u8string().c_str(), &m_currentDatabase);

    CreateTable();
}

void DataPersistenceInterface::CreateTable()
{
    std::string createTableSql = "";


}
