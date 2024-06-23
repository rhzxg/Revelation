#include "RevelationSplash.h"
#include <QLabel>

RevelationSplash::RevelationSplash(RevelationInterface* intf, QWidget* parent)
    : m_interface(intf), QDialog(parent)
{
    ui.setupUi(this);

    Initialize();
}

RevelationSplash::~RevelationSplash()
{
}

void RevelationSplash::Initialize()
{
    InitWidget();
}

void RevelationSplash::InitWidget()
{
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    auto    imagePath = m_interface->GetResourcePath() / "images" / "splash.png";
    if (std::filesystem::exists(imagePath))
    {
        QLabel* label = new QLabel(ui.eSplashWidget);
        QPixmap pixmap(QString::fromStdString(imagePath.u8string()));
        label->setStyleSheet("background-color: transparent;");
        label->setPixmap(pixmap);
        label->resize(pixmap.size());
    }
    else
    {
        close();
    }
}
