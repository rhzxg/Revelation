#include <QApplication>
#include <QTimer>
#include <QScreen>
#include "RevelationInterface.h"
#include "RevelationSplash.h"
#include "Revelation.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    auto intf = std::make_shared<RevelationInterface>();

    RevelationSplash splash(intf.get());
    QTimer::singleShot(3000, [&]() { splash.close(); });
    splash.exec();

    Revelation revelation(intf.get());
    revelation.show();

    return app.exec();
}