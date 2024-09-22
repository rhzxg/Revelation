#include "RevelationConfig.h"
#include "Utility/IUtilityInterface.h"
#include "FluSettingsSelectBox.h"
#include "FluSettingsVersionBox.h"
#include "FluLabel.h"
#include "FluHyperLinkButton.h"
#include "FluTextToggleSwitchEx.h"
#include <QSettings>
#include <QDir>

#ifdef WIN32
#include <windows.h>
#include <ShlObj.h>
#endif

RevelationConfig::RevelationConfig(IRevelationInterface* intf, QWidget* parent)
    : QWidget(parent), m_interface(intf)
{
    ui.setupUi(this);

    Initialize();
}

RevelationConfig::~RevelationConfig()
{
}

void RevelationConfig::AddSettingItem(QWidget* item)
{
    m_scrollArea->getMainLayout()->addWidget(item);
}

void RevelationConfig::RemoveSettingsItems()
{
    while (m_scrollArea->getMainLayout()->count() > 0)
    {
        QWidget* widget = m_scrollArea->getMainLayout()->itemAt(0)->widget();
        m_scrollArea->getMainLayout()->removeWidget(widget);
        delete widget;
    }
}

void RevelationConfig::Initialize()
{
    InitWidget();
    InitSignalSlots();
}

void RevelationConfig::InitWidget()
{
    m_scrollArea = new FluVScrollView;
    m_scrollArea->getMainLayout()->setAlignment(Qt::AlignTop);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_scrollArea);
    ui.eScrollAreaWidget->setLayout(layout);

    SetupApplicationInfoItem();
}

void RevelationConfig::InitSignalSlots()
{
}

void RevelationConfig::SetupApplicationInfoItem()
{
    auto settingsToolkit = m_interface->GetInterfaceById<IUtilityInterface>("Utility")->GetSettingsToolkit();

    auto revelationItem = new FluSettingsVersionBox;

    auto        utilityIntf = m_interface->GetInterfaceById<IUtilityInterface>("Utility");
    std::string version     = utilityIntf->GetSettingsToolkit()->GetString("Version", "Revelation", "2.0.0");
    revelationItem->getTitleLabel()->setText("Revelation");
    revelationItem->getInfoLabel()->setText("Open-source license: GPLv3");
    revelationItem->getVersionLabel()->setText(QString::fromStdString(version));

    QIcon icon = QIcon("./resources/images/icon.ico");
    revelationItem->getIconLabel()->setPixmap(icon.pixmap(40, 40));
    revelationItem->getIconLabel()->setFixedSize(40, 40);

    //////////////////////////////////////////////////////////////////////////
    auto themeChangeLabel = new FluLabel;
    themeChangeLabel->setLabelStyle(FluLabelStyle::BodyTextBlockStyle);
    themeChangeLabel->setText("Application theme: ");

    FluComboBoxEx* changeThemeCombobox = new FluComboBoxEx;
    changeThemeCombobox->setFixedWidth(115);
    changeThemeCombobox->addItem(tr("Light"));
    changeThemeCombobox->addItem(tr("Dark"));
    connect(changeThemeCombobox, &FluComboBoxEx::currentIndexChanged, [=](int index) {
        FluTheme theme = index == 0 ? FluTheme::Light : FluTheme::Dark;
        // FluThemeUtils::getUtils()->setTheme(FluTheme::Light);
    });

    QHBoxLayout* themeItemLayout = new QHBoxLayout;
    themeItemLayout->setContentsMargins(0, 0, 0, 0);
    themeItemLayout->addWidget(themeChangeLabel);
    themeItemLayout->addStretch();
    themeItemLayout->addWidget(changeThemeCombobox);
    QWidget* themeItemWidget = new QWidget;
    themeItemWidget->setLayout(themeItemLayout);

    revelationItem->addWidget(themeItemWidget);
    revelationItem->addVSplitLine();

    //////////////////////////////////////////////////////////////////////////
#ifdef WIN32
    bool launchOnBoot = settingsToolkit->GetBoolean("LaunchOnBoot", "Revelation", false);

    auto launchOnBootLabel = new FluLabel;
    launchOnBootLabel->setLabelStyle(FluLabelStyle::BodyTextBlockStyle);
    launchOnBootLabel->setText("Launch on boot: ");

    FluTextToggleSwitchEx* launchOnBootSwitch = new FluTextToggleSwitchEx;
    launchOnBootSwitch->setText("ON", "OFF");
    launchOnBootSwitch->blockSignals(true);
    launchOnBootSwitch->setChecked(launchOnBoot);
    launchOnBootSwitch->blockSignals(false);
    connect(launchOnBootSwitch, &FluTextToggleSwitchEx::stateChanged, [=](bool checked) {
        settingsToolkit->SetBoolean("LaunchOnBoot", "Revelation", checked);

        QSettings bootUpSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
        QString   appName = QCoreApplication::applicationName();
        QString   appPath = QCoreApplication::applicationFilePath();

        PWSTR appDataPath;
        if (SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &appDataPath) == S_OK)
        {
            QString appDataDir = QString::fromUtf16(reinterpret_cast<const char16_t*>(appDataPath));
            CoTaskMemFree(appDataPath);

            QString startupPath = appDataDir + "/Microsoft/Windows/Start Menu/Programs/Startup/" + appName + ".exe.lnk";
            if (checked)
            {
                QFile::link(appPath, startupPath);
            }
            else
            {
                QFile file(startupPath);
                file.remove();
            }
        }
    });

    QHBoxLayout* launchOnBootLayout = new QHBoxLayout;
    launchOnBootLayout->setContentsMargins(0, 0, 0, 0);
    launchOnBootLayout->addWidget(launchOnBootLabel);
    launchOnBootLayout->addStretch();
    launchOnBootLayout->addWidget(launchOnBootSwitch);
    QWidget* launchOnBootWidget = new QWidget;
    launchOnBootWidget->setLayout(launchOnBootLayout);

    revelationItem->addWidget(launchOnBootWidget);
    revelationItem->addVSplitLine();
#endif

    //////////////////////////////////////////////////////////////////////////
    auto githubLabel = new FluLabel;
    githubLabel->setLabelStyle(FluLabelStyle::BodyTextBlockStyle);
    githubLabel->setText("Github: ");

    auto githubRepoBtn = new FluHyperLinkButton("https://github.com/rhzxg/Revelation");
    githubRepoBtn->setText("Github page");

    QHBoxLayout* githubItemLayout = new QHBoxLayout;
    githubItemLayout->setContentsMargins(0, 0, 0, 0);
    githubItemLayout->addWidget(githubLabel);
    githubItemLayout->addStretch();
    githubItemLayout->addWidget(githubRepoBtn);
    QWidget* githubItemWidget = new QWidget;
    githubItemWidget->setLayout(githubItemLayout);

    revelationItem->addWidget(githubItemWidget);
    revelationItem->addVSplitLine();

    //////////////////////////////////////////////////////////////////////////
    auto referenceLabel = new FluLabel;
    referenceLabel->setLabelStyle(FluLabelStyle::BodyTextBlockStyle);
    referenceLabel->setText("References: ");

    auto fluentUIReference = new FluHyperLinkButton("https://github.com/mowangshuying/CppQtFluentUi888");
    fluentUIReference->setText("Qt FluentUI");

    auto kdchartReference = new FluHyperLinkButton("https://github.com/KDAB/KDChart");
    kdchartReference->setText("KDChart");

    QHBoxLayout* referenceItemLayout = new QHBoxLayout;
    referenceItemLayout->setContentsMargins(0, 0, 0, 0);
    referenceItemLayout->addWidget(referenceLabel);
    referenceItemLayout->addStretch();
    referenceItemLayout->addWidget(fluentUIReference);
    referenceItemLayout->addWidget(kdchartReference);
    QWidget* referenceItemWidget = new QWidget;
    referenceItemWidget->setLayout(referenceItemLayout);

    revelationItem->addWidget(referenceItemWidget);

    m_scrollArea->getMainLayout()->addWidget(revelationItem);
}
