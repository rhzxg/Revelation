#include <QApplication>
#include <QTimer>
#include <QScreen>
#include "RevelationInterface.h"
#include "RevelationMainWindow.h"
#include "RevelationSplash.h"
#include "Revelation.h"
#include <thread>

int main(int argc, char* argv[])
{
    QApplication         app(argc, argv);
    RevelationMainWindow mainWindow;

    auto intf = std::make_shared<RevelationInterface>(&mainWindow);

    RevelationSplash splash(intf.get());
    Revelation       revelation(intf.get());

    std::thread initializeThread([&]() {
        using namespace std::chrono;
        auto start = steady_clock::now();

        intf->InitExtensions();
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

    mainWindow.setCentralWidget(&revelation);
    mainWindow.setMinimumSize(600, 400);
    mainWindow.resize(revelation.size());
    mainWindow.show();

    return app.exec();
}