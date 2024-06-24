#include <QApplication>
#include <QTimer>
#include <QScreen>
#include "RevelationInterface.h"
#include "FramelessWindow.h"
#include "RevelationSplash.h"
#include "Revelation.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    auto intf = std::make_shared<RevelationInterface>();

    RevelationSplash splash(intf.get());
    QTimer::singleShot(3000, [&]() { splash.close(); });
    splash.exec();

    CFramelessWindow window;
    Revelation       centralWidget(intf.get());
    window.resize(centralWidget.size());
    window.setCentralWidget(&centralWidget);

    QScreen* currentScreen  = QGuiApplication::screenAt(window.pos());
    QRect    screenGeometry = currentScreen->availableGeometry();
    window.move((screenGeometry.width() - window.width()) / 2, (screenGeometry.height() - window.height()) / 2);
    window.show();

    return app.exec();
}