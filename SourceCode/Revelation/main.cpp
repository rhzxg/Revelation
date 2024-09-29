#include <QApplication>
#include <QTimer>
#include <QScreen>
#include "RevelationInterface.h"
#include "RevelationSplash.h"
#include "Revelation.h"
#include "RevelationConfig.h"
#include "RevelationMainWindow.h"
#include "CommonWidgetInterface.h"
#include <thread>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    RevelationMainWindow  mainWindow;
    CommonWidgetInterface commonWidgetIntf(&mainWindow);
    RevelationInterface   intf(&mainWindow, &commonWidgetIntf);

    RevelationSplash splash(&intf);
    Revelation       revelation(&intf);

    std::thread initializeThread([&]() {
        using namespace std::chrono;
        auto start = steady_clock::now();

        intf.InitExtensions();
        revelation.ReteiveDataFromDatabase();

        auto end     = steady_clock::now();
        auto elapsed = duration_cast<seconds>(end - start).count();
        if (elapsed < 3)
        {
            std::this_thread::sleep_for(seconds(3 - elapsed));
        }

        splash.close();
    });
    initializeThread.detach();

    splash.exec();

    mainWindow.AddStackedWidget(&revelation, "Revelation", FluAwesomeType::Home, Qt::AlignCenter);
    intf.Broadcast(BroadcastType::CollectNavigationView);
    intf.Broadcast(BroadcastType::CollectSettingsItem);
    
    mainWindow.AlignToScreenCenter();
    mainWindow.show();

    return app.exec();
}