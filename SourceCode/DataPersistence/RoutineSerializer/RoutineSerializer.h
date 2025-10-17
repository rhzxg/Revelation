#pragma once
#include "DataPersistence/IRoutineSerialzer.h"
#include <mutex>

class IRevelationInterface;
class IDataPersistenceInterface;
class IUtilityInterface;
class SQLiteDatabase;

class RoutineSerializer : public IRoutineSerialzer
{
  public:
    RoutineSerializer(IRevelationInterface* intf);
    ~RoutineSerializer();

    virtual void RecordRoutine(TaskRoutine routine) override;
    virtual void RemoveRoutine(TaskRoutine routine) override;

    virtual void ReteiveRoutines(std::vector<TaskRoutine>& routines) override;

  private:
    void Initialize();
    void Uninitialize();

  private:
    IRevelationInterface*      m_interface           = nullptr;
    IDataPersistenceInterface* m_dataPersistenceIntf = nullptr;
    IUtilityInterface*         m_utilityIntf         = nullptr;

    SQLiteDatabase* m_database = nullptr;

    std::mutex m_insertMutex;
};