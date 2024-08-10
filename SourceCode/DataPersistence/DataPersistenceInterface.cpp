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

void DataPersistenceInterface::InsertOrReplaceTaskInDatabase(TaskPrototype task)
{
    if (nullptr == m_currentDatabase)
    {
        return;
    }

    std::lock_guard<std::mutex> lock(m_insertMutex);

    sqlite3_stmt* stmt;
    std::string   insertSql = R"(
        REPLACE INTO t_tasks (
            f_id, 
            f_title,
            f_desc, 
            f_create_time, 
            f_start_time, 
            f_finish_time, 
            f_deadline, 
            f_status, 
            f_type, 
            f_tag) 
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
    }
    sqlite3_finalize(stmt);
}

void DataPersistenceInterface::RemoveTaskFromDatabase(TaskPrototype task)
{
    if (nullptr == m_currentDatabase)
    {
        return;
    }

    sqlite3_stmt* stmt;
    std::string   deleteSql = R"(
        DELETE FROM t_tasks WHERE f_id = ?;
    )";

    auto rc = sqlite3_prepare_v2(m_currentDatabase, deleteSql.c_str(), -1, &stmt, nullptr);
    if (rc == SQLITE_OK)
    {
        sqlite3_bind_int64(stmt, 1, task.m_id);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE)
        {
            std::cerr << "Failed to delete data: " << sqlite3_errmsg(m_currentDatabase) << std::endl;
        }
    }
    sqlite3_finalize(stmt);
}

void DataPersistenceInterface::RetriveTasksFromDatabase(std::vector<TaskPrototype>& tasks)
{
    if (nullptr == m_currentDatabase)
    {
        return;
    }

    RetriveTasksHelper(tasks, m_currentDatabase);
}

void DataPersistenceInterface::RetriveTasksFromDatabase(std::vector<TaskPrototype>& tasks, const std::string& date)
{
    std::filesystem::path databasePath = m_revelationIntf->GetApplicationPath() / "databases" / (date + ".rev");
    if (!std::filesystem::exists(databasePath))
    {
        return;
    }

    sqlite3* database = nullptr;
    sqlite3_open(databasePath.u8string().c_str(), &database);
    if (nullptr == database)
    {
        return;
    }

    RetriveTasksHelper(tasks, database);

    sqlite3_close(database);
}

void DataPersistenceInterface::RetriveTasksFromDatabase(std::vector<TaskPrototype>& tasks, const std::string& from, const std::string& to)
{
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

    time_t fromDate = utilityIntf->GetDateTimeFormatter()->ConvertDateTimeFromString(from);
    time_t toDate   = utilityIntf->GetDateTimeFormatter()->ConvertDateTimeFromString(to);
    for (const auto& entry : std::filesystem::directory_iterator(databaseFolderPath))
    {
        if (entry.is_regular_file())
        {
            std::filesystem::path filePath         = entry.path();
            std::string           databaseFileStem = filePath.filename().stem().u8string();
            time_t                date             = utilityIntf->GetDateTimeFormatter()->ConvertDateTimeFromString(databaseFileStem);
            if (fromDate <= date || date <= toDate)
            {
                sqlite3* database = nullptr;
                sqlite3_open(filePath.u8string().c_str(), &database);
                if (nullptr == database)
                {
                    continue;
                }

                RetriveTasksHelper(tasks, database);

                sqlite3_close(database);
            }
        }
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
    if (CreateTable())
    {
        // collect inherited records
        CollectInheritedRecords();
    }
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

    std::string           today            = utilityIntf->GetDateTimeFormatter()->GetCurrentDateTimeString(TimeMask::YMD);
    std::string           suffix           = ".rev";
    std::string           fileName         = today + suffix;
    std::filesystem::path databaseFilePath = m_revelationIntf->GetApplicationPath() / "databases" / fileName;

    sqlite3_open(databaseFilePath.u8string().c_str(), &m_currentDatabase);
}

bool DataPersistenceInterface::CreateTable()
{
    if (nullptr == m_currentDatabase)
    {
        return false;
    }

    std::string   tableQuerySql = "SELECT name FROM sqlite_master WHERE type='table' AND name='t_tasks';";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(m_currentDatabase, tableQuerySql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            sqlite3_finalize(stmt);
            return false;
        }
        sqlite3_finalize(stmt);
    }

    std::string createTableSql = R"(
        CREATE TABLE IF NOT EXISTS t_tasks (
            f_id INTEGER PRIMARY KEY,
            f_title TEXT,
            f_desc TEXT,
            f_create_time TEXT,
            f_start_time TEXT,
            f_finish_time TEXT,
            f_deadline TEXT,
            f_status INTEGER,
            f_type INTEGER,
            f_tag INTEGER
        );
    )";

    auto rc = sqlite3_exec(m_currentDatabase, createTableSql.c_str(), nullptr, nullptr, nullptr);

    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << sqlite3_errmsg(m_currentDatabase) << std::endl;
        return false;
    }

    return true;
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

    std::string mostRecentDatabaseFileStem = "";
    time_t      mostRecentDate             = -1;
    for (const auto& entry : std::filesystem::directory_iterator(databaseFolderPath))
    {
        if (entry.is_regular_file())
        {
            std::string databaseFileStem = entry.path().filename().stem().u8string();
            if (databaseFileStem == today)
            {
                continue;
            }

            time_t dateTime = utilityIntf->GetDateTimeFormatter()->ConvertDateTimeFromString(databaseFileStem);
            if (dateTime > mostRecentDate)
            {
                mostRecentDate             = dateTime;
                mostRecentDatabaseFileStem = databaseFileStem;
            }
        }
    }

    std::string           suffix                     = ".rev";
    std::string           mostRecentDatabaseFileName = mostRecentDatabaseFileStem + suffix;
    std::filesystem::path mostRecentDatabaseFile     = databaseFolderPath / mostRecentDatabaseFileName;
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

    // collect records of: ([not done] or [routine])
    std::string   collectSql = R"(
        SELECT * FROM t_tasks WHERE (f_status <> 4 OR f_tag == 1);
    )";
    sqlite3_stmt* stmt;
    auto          rc = sqlite3_prepare_v2(mostRecentDatabase, collectSql.c_str(), -1, &stmt, nullptr);
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

            // update task tag
            if (task.m_taskStatus != TaskStatus::Done)
            {
                task.m_taskTag = TaskTag::Inherited;
            }

            if (task.m_taskTag == TaskTag::Routine)
            {
                task.m_taskStatus = TaskStatus::Todo;
            }

            InsertOrReplaceTaskInDatabase(task);
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(mostRecentDatabase);
}

void DataPersistenceInterface::RetriveTasksHelper(std::vector<TaskPrototype>& tasks, sqlite3* database)
{
    if (nullptr == database)
    {
        return;
    }

    // always read
    std::string reteiveSql = R"(
        SELECT * FROM t_tasks;
    )";

    sqlite3_stmt* stmt;
    auto          rc = sqlite3_prepare_v2(database, reteiveSql.c_str(), -1, &stmt, nullptr);
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

            tasks.emplace_back(task);
        }
    }

    sqlite3_finalize(stmt);
}
