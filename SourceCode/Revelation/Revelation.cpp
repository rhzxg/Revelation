#include "Revelation.h"
#include "RevelationInterface.h"
#include <QLabel>
#include <QString>
#include <QPainter>
#include <QPushButton>
#include <QGridLayout>

Revelation::Revelation(RevelationInterface* intf, QWidget* parent)
    : m_interface(intf), QWidget(parent)
{
    ui.setupUi(this);

    Initialize();
}

Revelation::~Revelation()
{
}

void Revelation::Initialize()
{
    InitWidget();
    InitSignalSlots();
}

void Revelation::InitWidget()
{
    auto iconPath = m_interface->GetResourcePath() / "images" / "icon.ico";
    if (std::filesystem::exists(iconPath))
    {
        QPixmap pixmap(QString::fromStdString(iconPath.u8string()));
        setWindowIcon(QIcon(pixmap));
    }
}

void Revelation::InitSignalSlots()
{
}