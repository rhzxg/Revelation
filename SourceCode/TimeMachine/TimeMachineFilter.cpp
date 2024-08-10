#include "TimeMachineFilter.h"
#include "IRevelationInterface.h"
#include "ICommonWidgetInterface.h"
#include "DataPersistence/IDataPersistenceInterface.h"
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
        auto commonWidgetIntf    = m_interface->GetCommonWidgetInterface();
        auto dataPersistenceIntf = m_interface->GetInterfaceById<IDataPersistenceInterface>("DataPersistence");

        commonWidgetIntf->SetProgressBarVisibility(true, 0);

        //////////////////////////////////////////////////////////////////////////
        // retrieve
        std::string fromDateStr = fromDate.toString("yyyy-MM-dd").toStdString();
        std::string toDateStr   = toDate.toString("yyyy-MM-dd").toStdString();

        std::vector<std::string>   dates;
        std::vector<TaskPrototype> tasks;
        std::filesystem::path      databaseFolder = m_interface->GetApplicationPath() / "databases";
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
            commonWidgetIntf->SetProgressBarVisibility(true, perc);

            dataPersistenceIntf->RetrieveTasksFromDatabase(tasks, dates[i]);
        }
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        // filter
        
        //////////////////////////////////////////////////////////////////////////
        
        emit TaskFiltered(tasks);

        commonWidgetIntf->SetProgressBarVisibility(false);
    });
    filterThread.detach();
}
