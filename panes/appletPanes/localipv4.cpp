#include "localipv4.h"

QJsonObject localIPv4AppletPaneConfig;


void LocalIPv4AppletPane::readConfig() {
    QString homeDirectory = getenv("HOME");
    QFile file;
    QString data;

    file.setFileName(homeDirectory + "/.config/plainDE/config.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();
    localIPv4AppletPaneConfig = QJsonDocument::fromJson(data.toUtf8()).object();
}

void LocalIPv4AppletPane::prepareUI(QListWidget* ifaceList) {
    QList<QNetworkInterface> availableIfaces = QNetworkInterface::allInterfaces();
    ifaceList->clear();
    foreach(QNetworkInterface currentIface, availableIfaces) {
        ifaceList->addItem(currentIface.name());
    }
}

void LocalIPv4AppletPane::setCurrentSettings(QListWidget* ifaceList) {
    if (!localIPv4AppletPaneConfig["ipIfname"].toString().isEmpty()) {
        for (qint8 ifnameIndex = 0; ifnameIndex < ifaceList->count(); ++ifnameIndex) {
            if (ifaceList->item(ifnameIndex)->text() == localIPv4AppletPaneConfig["ipIfname"].toString()) {
                ifaceList->setCurrentRow(ifnameIndex);
                break;
            }
        }
    }
}

void LocalIPv4AppletPane::saveSettings(QListWidget* ifaceList) {
    if (!ifaceList->selectedItems().isEmpty()) {
        localIPv4AppletPaneConfig["ipIfname"] = QJsonValue(ifaceList->selectedItems()[0]->text());
    }

    Pane::saveConfig(localIPv4AppletPaneConfig);
}

QWidget* LocalIPv4AppletPane::createUI() {
    readConfig();

    // UI
    QWidget* localIPv4AppletPane = new QWidget;
    localIPv4AppletPane->setObjectName("localIPv4AppletPane");
    localIPv4AppletPane->setWindowTitle("plainControlCenter");
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(4, 4, 4, 4);
    localIPv4AppletPane->setLayout(layout);

    // Theme
    QFile stylesheetReader("/usr/share/plainDE/styles/" + localIPv4AppletPaneConfig["theme"].toString());
    stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream styleSheet(&stylesheetReader);
    localIPv4AppletPane->setStyleSheet(styleSheet.readAll());
    stylesheetReader.close();

    QFont paneFont;
    paneFont.setFamily(localIPv4AppletPaneConfig["fontFamily"].toString());
    paneFont.setPointSize(localIPv4AppletPaneConfig["fontSize"].toInt());
    localIPv4AppletPane->setFont(paneFont);

    short width = 400, height = 500;
    localIPv4AppletPane->setGeometry(650, 250, width, height);

    QLabel* selectLabel = new QLabel("Select desired network interface:");
    layout->addWidget(selectLabel);

    QListWidget* ifaceList = new QListWidget;
    ifaceList->setStyleSheet("QListView::item:selected { background-color: " + \
                             localIPv4AppletPaneConfig["accent"].toString() + \
                             "; color: #ffffff };");
    layout->addWidget(ifaceList);

    QPushButton* revertPushButton = new QPushButton("Revert");
    layout->addWidget(revertPushButton);

    QPushButton* savePushButton = new QPushButton("Save");
    layout->addWidget(savePushButton);

    prepareUI(ifaceList);

    // Make connections
    localIPv4AppletPane->connect(revertPushButton, &QPushButton::clicked, localIPv4AppletPane,
                                 [this, ifaceList]() {
        setCurrentSettings(ifaceList);
    });

    localIPv4AppletPane->connect(savePushButton, &QPushButton::clicked, localIPv4AppletPane,
                                 [this, ifaceList]() {
        saveSettings(ifaceList);
    });

    return localIPv4AppletPane;
}
