#include "DataPersistenceInterface.h"
#include "Utility/IUtilityInterface.h"
#include "Utility/IDateTimeFormatter.h"
#include <fstream>
#include <iostream>

DataPersistenceInterface::DataPersistenceInterface(IRevelationInterface* intf)
    : m_revelationIntf(intf)
{
}

DataPersistenceInterface::~DataPersistenceInterface()
{
}

void DataPersistenceInterface::Initialize()
{
    ExecDatabaseCreationRoutine();
}

void DataPersistenceInterface::Uninitialize()
{
    if (nullptr != m_currentDatabase)
    {
        sqlite3_close(m_currentDatabase);
    }
}

void DataPersistenceInterface::InsertOrReplaceTask(TaskPrototype task)
{
    if (nullptr == m_currentDatabase)
    {
        return;
    }

    sqlite3_stmt* stmt;
    std::string   insertSql = R"(
        REPLACE INTO t_tasks (
            c_id, 
            c_title,
            c_desc, 
            c_create_time, 
            c_start_time, 
            c_finish_time, 
            c_deadline, 
            c_status, 
            c_type, 
            c_tag) 
            VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    )";

    auto rc = sqlite3_prepare_v2(m_currentDatabase, insertSql.c_str(), -1, &stmt, nullptr);
    if (rc == SQLITE_OK)
    {
        sqlite3_bind_int64(stmt, 1, task.m_id);
        sqlite3_bind_text(stmt, 2, task.m_title.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, task.m_desc.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, task.m_createTime.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, task.m_startTime.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 6, task.m_finishTime.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 7, task.m_deadline.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 8, (int)task.m_taskStatus);
        sqlite3_bind_int(stmt, 9, (int)task.m_taskType);
        sqlite3_bind_int(stmt, 10, (int)task.m_taskTag);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE)
        {
            std::cerr << "Failed to insert data: " << sqlite3_errmsg(m_currentDatabase) << std::endl;
        }

        sqlite3_finalize(stmt);
    }
}

void DataPersistenceInterface::ExecDatabaseCreationRoutine()
{
    // clean up
    Uninitialize();

    // check and create folder
    CreateDatabaseFolder();

    // create database
    CreateDatabaseByDate();

    // create table
    CreateTable();

    // collect inherited records
    CollectInheritedRecords();
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
}

void DataPersistenceInterface::CreateTable()
{
    if (nullptr == m_currentDatabase)
    {
        return;
    }

    std::string createTableSql = R"(
        CREATE TABLE IF NOT EXISTS t_tasks (
            c_id INTEGER PRIMARY KEY,
            c_title TEXT,
            c_desc TEXT,
            c_create_time TEXT,
            c_start_time TEXT,
            c_finish_time TEXT,
            c_deadline TEXT,
            c_status INTEGER,
            c_type INTEGER,
            c_tag INTEGER
        );
    )";

    auto rc = sqlite3_exec(m_currentDatabase, createTableSql.c_str(), nullptr, nullptr, nullptr);

    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << sqlite3_errmsg(m_currentDatabase) << std::endl;
    }
}

void DataPersistenceInterface::CollectInheritedRecords()
{
    // find the most recent database except self
    std::filesystem::path databaseFolderPath = m_revelationIntf->GetApplicationPath() / "databases";
    if (!std::filesystem::exists(databaseFolderPath))
    {
        return;
    }

    IUtilityInterface* utilityIntf = m_revelationIntf->GetInterfaceById<IUtilityInterface>("Utility");
    if (nullptr == utilityIntf)
    {
        return;
    }

    std::string today           = utilityIntf->GetDateTimeFormatter()->GetCurrentDateTimeString(TimeMask::YMD);
    time_t      currentDataTime = utilityIntf->GetDateTimeFormatter()->ConvertDateTimeFromString(today);

    std::string mostRecentDatabaseFileName = "";
    time_t      mostRecentDate             = -1;
    for (const auto& entry : std::filesystem::directory_iterator(databaseFolderPath))
    {
        if (entry.is_regular_file())
        {
            std::string databaseFileName = entry.path().filename().stem().u8string();
            if (databaseFileName == today)
            {
                continue;
            }

            time_t dataTime = utilityIntf->GetDateTimeFormatter()->ConvertDateTimeFromString(databaseFileName);
            if (dataTime > mostRecentDate)
            {
                mostRecentDate             = dataTime;
                mostRecentDatabaseFileName = databaseFileName;
            }
        }
    }

    std::filesystem::path mostRecentDatabaseFile = databaseFolderPath / mostRecentDatabaseFileName;
    if (!std::filesystem::exists(mostRecentDatabaseFile))
    {
        return;
    }

    // create database connection
    sqlite3* mostRecentDatabase = nullptr;
    sqlite3_open(mostRecentDatabaseFile.u8string().c_str(), &mostRecentDatabase);
    if (nullptr == mostRecentDatabase)
    {
        return;
    }

    // collect records
    std::string   createTableSql = R"(
        SELECT * FROM t_tasks WHERE c_tag = 1 OR c_tag = 2;
    )";
    sqlite3_stmt* stmt;
    auto          rc = sqlite3_prepare_v2(mostRecentDatabase, createTableSql.c_str(), -1, &stmt, nullptr);
    if (rc == SQLITE_OK)
    {
        auto toStr = [&](const unsigned char* c) {
            const char* text = reinterpret_cast<const char*>(c);
            return std::string(text);
        };

        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
        {
            TaskPrototype task;
            task.m_id         = sqlite3_column_int64(stmt, 0);
            task.m_title      = toStr(sqlite3_column_text(stmt, 1));
            task.m_desc       = toStr(sqlite3_column_text(stmt, 2));
            task.m_createTime = toStr(sqlite3_column_text(stmt, 3));
            task.m_startTime  = toStr(sqlite3_column_text(stmt, 4));
            task.m_finishTime = toStr(sqlite3_column_text(stmt, 5));
            task.m_deadline   = toStr(sqlite3_column_text(stmt, 6));
            task.m_taskStatus = TaskStatus(sqlite3_column_int(stmt, 7));
            task.m_taskType   = TaskType(sqlite3_column_int(stmt, 8));
            task.m_taskTag    = TaskTag(sqlite3_column_int(stmt, 9));

            InsertOrReplaceTask(task);
        }
    }

    sqlite3_close(mostRecentDatabase);
}
