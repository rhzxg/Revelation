#include "RevelationConfig.h"
#include "Utility/IUtilityInterface.h"
#include "FluSettingsSelectBox.h"
#include "FluSettingsVersionBox.h"
#include "FluLabel.h"
#include "FluHyperLinkButton.h"

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
    SetupChangeThemeItem();
}

void RevelationConfig::InitSignalSlots()
{
}

void RevelationConfig::SetupApplicationInfoItem()
{
    auto applicationInfoItem = new FluSettingsVersionBox;

    auto        utilityIntf = m_interface->GetInterfaceById<IUtilityInterface>("Utility");
    std::string version     = utilityIntf->GetSettingsToolkit()->GetString("Version", "Revelation", "2.0.0");
    applicationInfoItem->getTitleLabel()->setText("Revelation");
    applicationInfoItem->getInfoLabel()->setText("Open-source license: GPLv3");
    applicationInfoItem->getVersionLabel()->setText(QString::fromStdString(version));

    QIcon icon = QIcon("./resources/images/icon.ico");
    applicationInfoItem->getIconLabel()->setPixmap(icon.pixmap(40, 40));
    applicationInfoItem->getIconLabel()->setFixedSize(40, 40);

    auto repoLabel = new FluLabel;
    repoLabel->setLabelStyle(FluLabelStyle::BodyTextBlockStyle);
    repoLabel->setText("Github");

    auto cloneRepoBtn = new FluHyperLinkButton("https://github.com/rhzxg/Revelation");
    cloneRepoBtn->setText("Github page");

    applicationInfoItem->addWidget(repoLabel);
    applicationInfoItem->addWidget(cloneRepoBtn);
    applicationInfoItem->addVSplitLine();

    auto dependAndRef = new FluLabel;
    dependAndRef->setLabelStyle(FluLabelStyle::BodyTextBlockStyle);
    dependAndRef->setText("References");

    auto fluentUIReference = new FluHyperLinkButton("https://github.com/mowangshuying/CppQtFluentUi888");
    fluentUIReference->setText("Qt FluentUI");

    auto kdchartReference = new FluHyperLinkButton("https://github.com/KDAB/KDChart");
    kdchartReference->setText("KDChart");

    applicationInfoItem->addWidget(dependAndRef);
    applicationInfoItem->addWidget(fluentUIReference);
    applicationInfoItem->addWidget(kdchartReference);

    m_scrollArea->getMainLayout()->addWidget(applicationInfoItem);
}

void RevelationConfig::SetupChangeThemeItem()
{
    auto changeThemeItem = new FluSettingsSelectBox;
    changeThemeItem->setTitleInfo(tr("App theme"), tr("Change application theme color"));
    changeThemeItem->setIcon(FluAwesomeType::Color);

    changeThemeItem->getComboBox()->addItem(tr("Light"));
    changeThemeItem->getComboBox()->addItem(tr("Dark"));

    connect(changeThemeItem->getComboBox(), &FluComboBoxEx::currentIndexChanged, [=](int index) {
        FluTheme theme = index == 0 ? FluTheme::Light : FluTheme::Dark;
        // FluThemeUtils::getUtils()->setTheme(FluTheme::Light);
    });

    m_scrollArea->getMainLayout()->addWidget(changeThemeItem);
}