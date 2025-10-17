#include "TaskSerializer.h"
#include "IRevelationInterface.h"
#include "DataPersistence/IDataPersistenceInterface.h"
#include "Utility/IUtilityInterface.h"
#include "../Sqlite/SQLiteDatabase.h"

TaskSerializer::TaskSerializer(IRevelationInterface* intf)
    : m_interface(intf)
{
    m_dataPersistenceIntf = m_interface->GetInterfaceById<IDataPersistenceInterface>("DataPeristence");
    m_utilityIntf         = m_interface->GetInterfaceById<IUtilityInterface>("Utility");

    Initialize();
}

TaskSerializer::~TaskSerializer()
{
    Uninitialize();
}

void TaskSerializer::RecordTask(TaskPrototype task)
{
    std::lock_guard lock(m_insertMutex);

    std::string insertSql = R"(
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
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
    )";

    auto link = m_database->GetDisposableLink();
    link->PrepareStmt(insertSql);
    link->BindInt64(task.m_id, 1);
    link->BindString(task.m_title, 2);
    link->BindString(task.m_desc, 3);
    link->BindString(task.m_createTime, 4);
    link->BindString(task.m_startTime, 5);
    link->BindString(task.m_finishTime, 6);
    link->BindString(task.m_deadline, 7);
    link->BindInt((int)task.m_taskStatus, 8);
    link->BindInt((int)task.m_taskType, 9);
    link->BindInt((int)task.m_taskTag, 10);
    link->Evaluate();
}

void TaskSerializer::RemoveTask(TaskPrototype task)
{
    std::string deleteSql = "DELETE FROM t_tasks WHERE f_id = ?;";
    auto        link      = m_database->GetDisposableLink();
    link->PrepareStmt(deleteSql);
    link->BindInt64(task.m_id, 1);
    link->Evaluate();
}

void TaskSerializer::RetrieveTasks(std::vector<TaskPrototype>& tasks)
{
    std::string today = m_utilityIntf->GetDateTimeFormatter()->GetCurrentDateTimeString(TimeMask::YMD);
    RetrieveTasks(tasks, today);
}

void TaskSerializer::RetrieveTasks(std::vector<TaskPrototype>& tasks, const std::string& date)
{
    RetrieveTasks(tasks, date, date);
}

void TaskSerializer::RetrieveTasks(std::vector<TaskPrototype>& tasks, const std::string& from, const std::string& to)
{
    auto                  timeFormatter      = m_utilityIntf->GetDateTimeFormatter();
    time_t                fromDate           = timeFormatter->ConvertDateTimeFromString(from);
    time_t                toDate             = timeFormatter->ConvertDateTimeFromString(to);
    std::filesystem::path databaseFolderPath = m_interface->GetApplicationPath() / "databases";
    for (const auto& entry : std::filesystem::directory_iterator(databaseFolderPath))
    {
        if (entry.is_regular_file())
        {
            std::filesystem::path filePath         = entry.path();
            std::string           databaseFileStem = filePath.filename().stem().u8string();
            time_t                date             = timeFormatter->ConvertDateTimeFromString(databaseFileStem);
            if (fromDate <= date && date <= toDate)
            {
                std::string reteiveSql = "SELECT * FROM t_tasks;";

                SQLiteDatabase* database = new SQLiteDatabase;
                if (!database->Open(filePath.u8string()))
                {
                    continue;
                }

                auto link = database->GetDisposableLink();
                link->PrepareStmt(reteiveSql);
                while (link->QueryNext())
                {
                    TaskPrototype task;

                    task.m_id         = link->ColumnInt64(0);
                    task.m_title      = link->ColumnString(1);
                    task.m_desc       = link->ColumnString(2);
                    task.m_createTime = link->ColumnString(3);
                    task.m_startTime  = link->ColumnString(4);
                    task.m_finishTime = link->ColumnString(5);
                    task.m_deadline   = link->ColumnString(6);
                    task.m_taskStatus = TaskStatus(link->ColumnInt(7));
                    task.m_taskType   = TaskType(link->ColumnInt(8));
                    task.m_taskTag    = TaskTag(link->ColumnInt(9));

                    tasks.emplace_back(std::move(task));
                }

                delete database;
            }
        }
    }
}

void TaskSerializer::PrepareDatabaseForToday()
{
    // close current database
    Uninitialize();

    // create folder if there is not
    std::filesystem::path databaseFolderPath = m_interface->GetApplicationPath() / "databases";
    if (!std::filesystem::exists(databaseFolderPath))
    {
        std::filesystem::create_directory(databaseFolderPath);
    }

    // open database
    std::string           today            = m_utilityIntf->GetDateTimeFormatter()->GetCurrentDateTimeString(TimeMask::YMD);
    std::string           suffix           = ".rev";
    std::string           fileName         = today + suffix;
    std::filesystem::path databaseFilePath = databaseFolderPath / fileName;
    bool                  newDatabase      = !std::filesystem::exists(databaseFilePath);

    m_database = new SQLiteDatabase;
    m_database->SetDatabaseRole(DatabaseRole::Daily);
    bool opened = m_database->Open(databaseFilePath.u8string());

    // create table
    if (opened && newDatabase)
    {
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

        auto link = m_database->GetDisposableLink();
        link->Exec(createTableSql);

        m_dataPersistenceIntf->RegisterDatabase(DatabaseRole::Daily, m_database);
    }
}

void TaskSerializer::Initialize()
{
    PrepareDatabaseForToday();
}

void TaskSerializer::Uninitialize()
{
    if (nullptr != m_database)
    {
        m_database->Close();
        delete m_database;
    }
}
