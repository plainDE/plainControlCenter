#include "autostart.h"
#include "../pane.h"

QJsonObject autostartPaneConfig;
QStringList autostartEntries;
QHash<QListWidgetItem*,QString> entryByItem;


void AutostartPane::readConfig() {
    QString homeDirectory = getenv("HOME");
    QFile file;
    QString data;

    file.setFileName(homeDirectory + "/.config/plainDE/config.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();
    autostartPaneConfig = QJsonDocument::fromJson(data.toUtf8()).object();
}

App readDesktopFile(QString pathToCurrentDesktopFile) {
    App myApp;
    QString iconPath;

    QSettings desktopFileReader(pathToCurrentDesktopFile, QSettings::IniFormat);
    desktopFileReader.sync();
    desktopFileReader.beginGroup("Desktop Entry");
            myApp.displayedName = desktopFileReader.value("Name").toString();
            iconPath = desktopFileReader.value("Icon").toString();
            if (QIcon::hasThemeIcon(iconPath)) {
                myApp.icon = QIcon::fromTheme(iconPath);
            }
            else {
                if (QFile::exists(iconPath)) {
                    myApp.icon = QIcon(iconPath);
                }
                else {
                    // ICON: unknown app
                    myApp.icon = QIcon(iconPath);
                }
            }
    return myApp;
}

void AutostartPane::prepareUI(QListWidget* autostartEntriesListWidget,
                              QListWidget* allAppsListWidget) {
    // TODO: ~/.local/share/applications
    QDir appDir("/usr/share/applications");
    QStringList desktopFilesList = appDir.entryList();

    autostartEntries = autostartPaneConfig["autostart"].toVariant().toStringList();

    QString pathToCurrentDesktopFile;
    App currentApp;

    desktopFilesList.removeFirst();
    desktopFilesList.removeFirst();

    allAppsListWidget->clear();
    autostartEntriesListWidget->clear();
    entryByItem.clear();

    for (int i = 0; i < desktopFilesList.length(); ++i) {
        pathToCurrentDesktopFile = appDir.absoluteFilePath(desktopFilesList[i]);
        currentApp = readDesktopFile(pathToCurrentDesktopFile);
        if (currentApp.displayedName != "") {
            QListWidgetItem* item = new QListWidgetItem(currentApp.displayedName);
            item->setIcon(currentApp.icon);
            allAppsListWidget->addItem(item);
            entryByItem[item] = desktopFilesList[i];

            if (autostartEntries.contains(desktopFilesList[i])) {
                QListWidgetItem* favItem = new QListWidgetItem(currentApp.displayedName);
                favItem->setIcon(currentApp.icon);
                autostartEntriesListWidget->addItem(favItem);
                entryByItem[favItem] = desktopFilesList[i];
            }
        }
    }

    allAppsListWidget->setCurrentRow(0);
}

void AutostartPane::addEntry(QListWidget* autostartEntriesListWidget,
                             QListWidget* allAppsListWidget) {
    if (!allAppsListWidget->selectedItems().isEmpty()) {
        QListWidgetItem* selectedItem = allAppsListWidget->selectedItems()[0];

        QString selectedItemName = selectedItem->text();
        QIcon selectedItemIcon = selectedItem->icon();

        QListWidgetItem* favItem = new QListWidgetItem(selectedItemName);
        favItem->setIcon(selectedItemIcon);
        autostartEntriesListWidget->addItem(favItem);
        entryByItem[favItem] = entryByItem[selectedItem];
    }
}

void AutostartPane::removeEntry(QListWidget* autostartEntriesListWidget) {
    if (!autostartEntriesListWidget->selectedItems().isEmpty()) {
        QListWidgetItem* item = autostartEntriesListWidget->selectedItems()[0];
        autostartEntriesListWidget->removeItemWidget(item);
        delete item;
    }
}

void AutostartPane::saveSettings(QListWidget* autostartEntriesListWidget) {
    QStringList finalAutostartEntries;
    for (short i = 0; i < autostartEntriesListWidget->count(); ++i) {
        QString currentItemEntry = entryByItem[autostartEntriesListWidget->item(i)];
        finalAutostartEntries << currentItemEntry;
    }
    autostartPaneConfig["autostart"] = QJsonValue(QJsonArray::fromStringList(finalAutostartEntries));
    Pane::saveConfig(autostartPaneConfig);
}

QWidget* AutostartPane::createUI(QWidget* controlCenter) {
    readConfig();

    // UI
    QWidget* autostartPane = new QWidget;
    autostartPane->setObjectName("autostartPane");
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(4, 4, 4, 4);
    autostartPane->setLayout(layout);

    // Theme
    QFile stylesheetReader("/usr/share/plainDE/styles/" + autostartPaneConfig["theme"].toString());
    stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream styleSheet(&stylesheetReader);
    autostartPane->setStyleSheet(styleSheet.readAll());

    QFont paneFont;
    paneFont.setFamily(autostartPaneConfig["fontFamily"].toString());
    paneFont.setPointSize(autostartPaneConfig["fontSize"].toInt());
    autostartPane->setFont(paneFont);

    short width = 400, height = 500;
    autostartPane->setGeometry(250, 250, width, height);

    QPushButton* backPushButton = new QPushButton("Back");
    backPushButton->setIcon(QIcon::fromTheme("go-previous"));
    autostartPane->layout()->addWidget(backPushButton);

    QLabel* autostartEntriesLabel = new QLabel("Autostart entries");
    autostartPane->layout()->addWidget(autostartEntriesLabel);

    QListWidget* autostartEntriesListWidget = new QListWidget;
    autostartPane->layout()->addWidget(autostartEntriesListWidget);
    autostartEntriesListWidget->setStyleSheet("QListView::item:selected { background-color: " + \
                                              autostartPaneConfig["accent"].toString() + \
                                              "; color: #ffffff };");

    QLabel* allAppsLabel = new QLabel("All applications");
    autostartPane->layout()->addWidget(allAppsLabel);

    QListWidget* allAppsListWidget = new QListWidget;
    autostartPane->layout()->addWidget(allAppsListWidget);
    allAppsListWidget->setStyleSheet("QListView::item:selected { background-color: " + \
                                     autostartPaneConfig["accent"].toString() + \
                                     "; color: #ffffff };");

    QPushButton* addSelectedPushButton = new QPushButton("Add");
    autostartPane->layout()->addWidget(addSelectedPushButton);

    QPushButton* removeSelectedPushButton = new QPushButton("Remove");
    autostartPane->layout()->addWidget(removeSelectedPushButton);

    QPushButton* revertPushButton = new QPushButton("Revert");
    autostartPane->layout()->addWidget(revertPushButton);

    QPushButton* savePushButton = new QPushButton("Save");
    autostartPane->layout()->addWidget(savePushButton);

    prepareUI(autostartEntriesListWidget, allAppsListWidget);

    // Make connections
    autostartPane->connect(backPushButton, &QPushButton::clicked, autostartPane,
                           [autostartPane, controlCenter]() {
        controlCenter->show();
        autostartPane->hide();
        delete autostartPane;
    });

    autostartPane->connect(addSelectedPushButton, &QPushButton::clicked, autostartPane,
                           [this, autostartEntriesListWidget, allAppsListWidget]() {
        addEntry(autostartEntriesListWidget, allAppsListWidget);
    });

    autostartPane->connect(removeSelectedPushButton, &QPushButton::clicked, autostartPane,
                           [this, autostartEntriesListWidget]() {
        removeEntry(autostartEntriesListWidget);
    });

    autostartPane->connect(savePushButton, &QPushButton::clicked, autostartPane,
                           [this, autostartEntriesListWidget]() {
        saveSettings(autostartEntriesListWidget);
    });

    autostartPane->connect(revertPushButton, &QPushButton::clicked, autostartPane,
                           [this, autostartEntriesListWidget, allAppsListWidget]() {
        prepareUI(autostartEntriesListWidget, allAppsListWidget);
    });


    return autostartPane;
}
