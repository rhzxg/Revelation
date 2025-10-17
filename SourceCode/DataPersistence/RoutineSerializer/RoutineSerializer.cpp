#include "RoutineSerializer.h"
#include "IRevelationInterface.h"
#include "DataPersistence/IDataPersistenceInterface.h"
#include "Utility/IUtilityInterface.h"
#include "../Sqlite/SQLiteDatabase.h"

RoutineSerializer::RoutineSerializer(IRevelationInterface* intf)
    : m_interface(intf)
{
    m_dataPersistenceIntf = m_interface->GetInterfaceById<IDataPersistenceInterface>("DataPeristence");
    m_utilityIntf         = m_interface->GetInterfaceById<IUtilityInterface>("Utility");

    Initialize();
}

RoutineSerializer::~RoutineSerializer()
{
}

void RoutineSerializer::RecordRoutine(TaskRoutine routine)
{
    std::lock_guard<std::mutex> lock(m_insertMutex);

    std::string insertSql = R"(
        REPLACE INTO t_task_routines (
        f_id, 
        f_create_time, 
        f_repeat_type, 
        f_task_title, 
        f_task_desc) 
        VALUES (?, ?, ?, ?, ?);
    )";

    auto link = m_database->GetDisposableLink();
    link->PrepareStmt(insertSql);
    link->BindInt64(routine.m_id, 1);
    link->BindString(routine.m_createTime, 2);
    link->BindInt((int)routine.m_repeatType, 3);
    link->BindString(routine.m_taskTitle, 4);
    link->BindString(routine.m_taskDesc, 5);
    link->Evaluate();
}

void RoutineSerializer::RemoveRoutine(TaskRoutine routine)
{
    std::string deleteSql = "DELETE FROM t_task_routines WHERE f_id = ?;";
    auto        link      = m_database->GetDisposableLink();
    link->PrepareStmt(deleteSql);
    link->BindInt64(routine.m_id, 1);
    link->Evaluate();
}

void RoutineSerializer::ReteiveRoutines(std::vector<TaskRoutine>& routines)
{
    std::string reteiveSql = "SELECT * FROM t_task_routines;";

    auto link = m_database->GetDisposableLink();
    link->PrepareStmt(reteiveSql);
    while (link->QueryNext())
    {
        TaskRoutine routine;

        routine.m_id         = link->ColumnInt64(0);
        routine.m_createTime = link->ColumnString(1);
        routine.m_repeatType = TaskRepeatType(link->ColumnInt(2));
        routine.m_taskTitle  = link->ColumnString(3);
        routine.m_taskDesc   = link->ColumnString(4);

        routines.emplace_back(std::move(routine));
    }
}

void RoutineSerializer::Initialize()
{
    // create folder if there is not
    std::filesystem::path databaseFolderPath = m_interface->GetApplicationPath() / "databases";
    if (!std::filesystem::exists(databaseFolderPath))
    {
        std::filesystem::create_directory(databaseFolderPath);
    }

    // open database
    std::string           suffix           = ".rev";
    std::string           fileName         = "revelation" + suffix;
    std::filesystem::path databaseFilePath = databaseFolderPath / fileName;
    bool                  newDatabase      = !std::filesystem::exists(databaseFilePath);

    m_database = new SQLiteDatabase;
    m_database->SetDatabaseRole(DatabaseRole::Daily);
    bool opened = m_database->Open(databaseFilePath.u8string());

    // create table
    if (opened && newDatabase)
    {
        std::string createTableSql = R"(
            CREATE TABLE IF NOT EXISTS t_task_routines (
            f_id INTEGER PRIMARY KEY,
            f_create_time TEXT,
            f_repeat_type INTEGER,
            f_task_title TEXT,
            f_task_desc TEXT
            );
         )";

        auto link = m_database->GetDisposableLink();
        link->Exec(createTableSql);

        m_dataPersistenceIntf->RegisterDatabase(DatabaseRole::Daily, m_database);
    }
}

void RoutineSerializer::Uninitialize()
{
}
