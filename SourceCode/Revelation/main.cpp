#include <QApplication>
#include <QTimer>
#include <QScreen>
#include "RevelationInterface.h"
#include "RevelationMainWindow.h"
#include "RevelationSplash.h"
#include "Revelation.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    RevelationMainWindow mainWindow;

    auto intf = std::make_shared<RevelationInterface>(&mainWindow);

    RevelationSplash splash(intf.get());
    QTimer::singleShot(3000, [&]() { splash.close(); });
    splash.exec();

    Revelation revelation(intf.get());
    mainWindow.setCentralWidget(&revelation);
    mainWindow.setMinimumSize(600, 400);
    mainWindow.resize(revelation.size());
    mainWindow.show();
    revelation.SetBottomBarVisible(true);

    return app.exec();
}