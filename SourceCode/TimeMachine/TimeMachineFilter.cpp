#include "TimeMachineFilter.h"

TimeMachineFilter::TimeMachineFilter(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    Initialize();
}

TimeMachineFilter::~TimeMachineFilter()
{
}

void TimeMachineFilter::Initialize()
{
    InitWIdget();
    InitSignalSlots();
}

void TimeMachineFilter::InitWIdget()
{
}

void TimeMachineFilter::InitSignalSlots()
{
}
