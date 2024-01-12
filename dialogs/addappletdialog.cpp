#include "addappletdialog.h"

#include "launcherdialog.h"
#include "clioutputdialog.h"
#include "../panes/panelspane.h"

void AddAppletDialog::setPaneContents() {
    mContentsWidget = new QWidget();
    QVBoxLayout* contentsLayout = new QVBoxLayout(mContentsWidget);
    mContentsWidget->setObjectName("innerPane");

    // Adding widgets
    QLabel* selectAppletLabel = new QLabel(tr("Select applet from the list:"));
    selectAppletLabel->setFont(mTitleFont);
    contentsLayout->addWidget(selectAppletLabel);

    QListWidget* appletsListWidget = new QListWidget();
    appletsListWidget->setFont(mFont);
    QString style = QString("QListView::item:selected { "
                            "background-color: %1; color: #ffffff; "
                            "};").arg(getConfigValue("accent").toString());
    appletsListWidget->setStyleSheet(style);
    contentsLayout->addWidget(appletsListWidget);

    QHBoxLayout* buttonsLayout = new QHBoxLayout();

    buttonsLayout->addSpacerItem(new QSpacerItem(0, 0,
                                                 QSizePolicy::MinimumExpanding,
                                                 QSizePolicy::Ignored));

    QPushButton* cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setFont(mFont);
    buttonsLayout->addWidget(cancelButton);

    QPushButton* addButton = new QPushButton(tr("Add"));
    addButton->setFont(mFont);
    buttonsLayout->addWidget(addButton);

    contentsLayout->addLayout(buttonsLayout);

    // Misc
    QListWidgetItem* appMenuItem = new QListWidgetItem(tr("App Menu"));
    appMenuItem->setIcon(QIcon::fromTheme("app-launcher"));
    appletsListWidget->addItem(appMenuItem);

    QListWidgetItem* windowListItem = new QListWidgetItem(tr("Window List"));
    windowListItem->setIcon(QIcon::fromTheme("kwin"));
    appletsListWidget->addItem(windowListItem);

    QListWidgetItem* spacerItem = new QListWidgetItem(tr("Spacer"));
    spacerItem->setIcon(QIcon::fromTheme("extensions"));
    appletsListWidget->addItem(spacerItem);

    QListWidgetItem* workspacesItem = new QListWidgetItem(tr("Workspaces"));
    workspacesItem->setIcon(QIcon::fromTheme("cs-workspaces"));
    appletsListWidget->addItem(workspacesItem);

    QListWidgetItem* volumeItem = new QListWidgetItem(tr("Volume"));
    volumeItem->setIcon(QIcon::fromTheme("sound"));
    appletsListWidget->addItem(volumeItem);

    QListWidgetItem* kbLayoutItem = new QListWidgetItem(tr("Keyboard Layout"));
    kbLayoutItem->setIcon(QIcon::fromTheme("keyboard"));
    appletsListWidget->addItem(kbLayoutItem);

    QListWidgetItem* dateTimeItem = new QListWidgetItem(tr("Date & Time"));
    dateTimeItem->setIcon(QIcon::fromTheme("calendar"));
    appletsListWidget->addItem(dateTimeItem);

    QListWidgetItem* splitterItem = new QListWidgetItem(tr("Splitter"));
    splitterItem->setIcon(QIcon::fromTheme("extensions"));
    appletsListWidget->addItem(splitterItem);

    QListWidgetItem* userMenuItem = new QListWidgetItem(tr("User Menu"));
    userMenuItem->setIcon(QIcon::fromTheme("user_icon"));
    appletsListWidget->addItem(userMenuItem);

    QListWidgetItem* ipAddrItem = new QListWidgetItem(tr("Local IPv4 Address"));
    ipAddrItem->setIcon(QIcon::fromTheme("cs-network"));
    appletsListWidget->addItem(ipAddrItem);

    QListWidgetItem* mprisItem = new QListWidgetItem(tr("Playback Control (MPRIS)"));
    mprisItem->setIcon(QIcon::fromTheme("sound"));
    appletsListWidget->addItem(mprisItem);

    QListWidgetItem* launcherItem = new QListWidgetItem(tr("Launcher"));
    launcherItem->setIcon(QIcon::fromTheme("utilities-terminal"));
    appletsListWidget->addItem(launcherItem);

    QListWidgetItem* batteryItem = new QListWidgetItem(tr("Battery"));
    batteryItem->setIcon(QIcon::fromTheme("extensions"));
    appletsListWidget->addItem(batteryItem);

    QListWidgetItem* sniTrayItem = new QListWidgetItem(tr("Tray (StatusNotifierItem)"));
    sniTrayItem->setIcon(QIcon::fromTheme("extensions"));
    appletsListWidget->addItem(sniTrayItem);

    QListWidgetItem* cliOutputItem = new QListWidgetItem("CLI Output");
    cliOutputItem->setIcon(QIcon::fromTheme("utilities-terminal"));
    appletsListWidget->addItem(cliOutputItem);

    // Making connections
    connect(addButton, &QPushButton::clicked, this, [this, appletsListWidget]() {
        if (!appletsListWidget->selectedItems().isEmpty()) {
            if (!appletsListWidget->currentItem()->text().compare("Launcher")) {
                this->hide();
                LauncherDialog* dialog = new LauncherDialog(mCfgObj, mPanelID, true, -1, mAppletsListWidget);
                dialog->setPaneContents();
                dialog->show();
            }
            else if (!appletsListWidget->currentItem()->text().compare("CLI Output")) {
                this->hide();
                CLIOutputDialog* dialog = new CLIOutputDialog(mCfgObj, mPanelID, true, -1, mAppletsListWidget);
                dialog->setPaneContents();
                dialog->show();
            }
            else {
                QListWidgetItem* item = new QListWidgetItem();
                item->setText(appletsListWidget->currentItem()->text());
                item->setIcon(appletsListWidget->currentItem()->icon());
                mAppletsListWidget->addItem(item);
                mAppletsListWidget->setCurrentRow(mAppletsListWidget->count() - 1);
                this->hide();
                delete this;
            }
        }
    });

    connect(cancelButton, &QPushButton::clicked, this, [this]() {
        this->hide();
        delete this;
    });

    finalizeUI();
    setWindowGeometry();
    setTransparency(this);
}

AddAppletDialog::AddAppletDialog(QJsonObject* cfgObj,
                                 int panelID,
                                 QListWidget* appletsListWidget) : Dialog(cfgObj,
                                                                          tr("Adding applet"),
                                                                          "cs-applets") {
    mCfgObj = cfgObj;
    mPanelID = panelID;
    mAppletsListWidget = appletsListWidget;
}
