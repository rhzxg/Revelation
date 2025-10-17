#pragma once
#include "IRevelationDataDefine.h"

class IRoutineSerialzer
{
  public:
    virtual void RecordRoutine(TaskRoutine routine) = 0;
    virtual void RemoveRoutine(TaskRoutine routine) = 0;

    virtual void ReteiveRoutines(std::vector<TaskRoutine>& routines) = 0;
};