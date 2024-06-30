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
    this->setWindowFlag(Qt::SplashScreen);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setWindowFlag(Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    auto iconPath   = m_interface->GetResourcePath() / "images" / "icon.ico";
    if (std::filesystem::exists(iconPath))
    {
        QPixmap pixmap(QString::fromStdString(iconPath.u8string()));
        this->setWindowIcon(pixmap);
    }

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
        this->close();
    }
}
