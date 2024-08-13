#include "TimeMachineFilter.h"
#include "IRevelationInterface.h"
#include "ICommonWidgetInterface.h"
#include "DataPersistence/IDataPersistenceInterface.h"
#include "Utility/IUtilityInterface.h"
#include <QMainWindow>

TimeMachineFilter::TimeMachineFilter(IRevelationInterface* intf, QWidget* parent)
    : m_interface(intf), QWidget(parent)
{
    ui.setupUi(this);

    Initialize();
}

TimeMachineFilter::~TimeMachineFilter()
{
}

void TimeMachineFilter::Initialize()
{
    m_commonWidgetIntf    = m_interface->GetCommonWidgetInterface();
    m_dataPersistenceIntf = m_interface->GetInterfaceById<IDataPersistenceInterface>("DataPersistence");
    m_timeFormatter       = m_interface->GetInterfaceById<IUtilityInterface>("Utility")->GetDateTimeFormatter();

    InitWidget();
    InitSignalSlots();
}

void TimeMachineFilter::InitWidget()
{
    QDate today = QDate::currentDate();
    ui.btnSelectFromDate->setCurDate(today);
    ui.btnSelectToDate->setCurDate(today);

    ui.cbStatue->addItems({tr("All"), tr("ToDo"), tr("Doing"), tr("Verifying"), tr("Done")});
    ui.cbType->addItems({tr("All"), tr("None"), tr("Bug"), tr("Feature"), tr("Test"), tr("UI")});
    ui.cbTag->addItems({tr("All"), tr("None"), tr("Routine"), tr("Inherited")});
}

void TimeMachineFilter::InitSignalSlots()
{
    connect(ui.btnFilter, &QPushButton::clicked, this, &TimeMachineFilter::OnBtnFilterClicked);
}

void TimeMachineFilter::ReteiveTasksFromDatabase(std::map<std::string, std::vector<TaskPrototype>>& dateToTasks)
{
    QDate       fromDate    = ui.btnSelectFromDate->getCurDate();
    QDate       toDate      = ui.btnSelectToDate->getCurDate();
    std::string fromDateStr = fromDate.toString("yyyy-MM-dd").toStdString();
    std::string toDateStr   = toDate.toString("yyyy-MM-dd").toStdString();

    std::vector<std::string> dates;
    std::filesystem::path    databaseFolder = m_interface->GetApplicationPath() / "databases";
    for (const auto& entry : std::filesystem::directory_iterator(databaseFolder))
    {
        if (entry.is_regular_file())
        {
            std::filesystem::path filePath         = entry.path();
            std::string           databaseFileStem = filePath.filename().stem().u8string();
            QDate                 databaseDate     = QDate::fromString(QString::fromStdString(databaseFileStem), "yyyy-MM-dd");
            if (fromDate <= databaseDate && databaseDate <= toDate)
            {
                dates.push_back(databaseFileStem);
            }
        }
    }

    for (int i = 0; i < dates.size(); ++i)
    {
        int perc = (float(i + 1) / dates.size() / 2) * 100;
        m_commonWidgetIntf->SetProgressBarVisibility(true, perc);

        std::string                date = dates[i];
        std::vector<TaskPrototype> tasks;
        m_dataPersistenceIntf->RetrieveTasksFromDatabase(tasks, date);

        if (!tasks.empty())
        {
            dateToTasks.emplace(date, tasks);
        }
    }
}

void TimeMachineFilter::FilterTasksByCondition(std::map<std::string, std::vector<TaskPrototype>>& dateToTasks)
{
    // TODO
}

void TimeMachineFilter::SortTasksByTaskParameter(std::map<std::string, std::vector<TaskPrototype>>& dateToTasks)
{
    auto mySort = [&](const TaskPrototype& t1, const TaskPrototype& t2) {
        if (t1.m_taskStatus == TaskStatus::Todo && t2.m_taskStatus != TaskStatus::Todo)
        {
            return false;
        }
        else if (t1.m_taskStatus != TaskStatus::Todo && t2.m_taskStatus == TaskStatus::Todo)
        {
            return true;
        }

        auto t1StartTime = m_timeFormatter->ConvertDateTimeFromString(t1.m_startTime);
        auto t2StartTime = m_timeFormatter->ConvertDateTimeFromString(t2.m_startTime);
        if (t1StartTime < t2StartTime)
        {
            return true;
        }

        if (t1.m_startTime == t2.m_startTime)
        {
            auto t1FinishTime = m_timeFormatter->ConvertDateTimeFromString(t1.m_finishTime);
            auto t2FinishTime = m_timeFormatter->ConvertDateTimeFromString(t2.m_finishTime);

            return t1FinishTime < t2FinishTime;
        }
        return false;
    };

    for (auto& pair : dateToTasks)
    {
        std::vector<TaskPrototype>& vec = pair.second;
        std::sort(vec.begin(), vec.end(), mySort);
    }
}

void TimeMachineFilter::OnBtnFilterClicked()
{
    QDate fromDate = ui.btnSelectFromDate->getCurDate();
    QDate toDate   = ui.btnSelectToDate->getCurDate();
    if (toDate > fromDate)
    {
        // TODO: alert
        // return;
    }
    else if (fromDate.daysTo(toDate) >= 365)
    {
        // TODO: alert
        // return;
    }

    std::thread filterThread([=]() {
        m_commonWidgetIntf->SetProgressBarVisibility(true, 0);

        std::map<std::string, std::vector<TaskPrototype>> dateToTasks;

        // retrieve
        ReteiveTasksFromDatabase(dateToTasks);

        // filter
        FilterTasksByCondition(dateToTasks);

        // sort
        SortTasksByTaskParameter(dateToTasks);

        emit TaskFiltered(dateToTasks);

        m_commonWidgetIntf->SetProgressBarVisibility(false);
    });
    filterThread.detach();
}
