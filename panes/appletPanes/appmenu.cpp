#include "appmenu.h"

QJsonObject appMenuAppletConfig;
QHash<QListWidgetItem*,QString> _entryByItem;
QStringList favEntries;


void AppMenuAppletPane::readConfig() {
    QString homeDirectory = getenv("HOME");
    QFile file;
    QString data;

    file.setFileName(homeDirectory + "/.config/plainDE/config.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();
    appMenuAppletConfig = QJsonDocument::fromJson(data.toUtf8()).object();
}

void AppMenuAppletPane::makePreview(QPushButton* previewButton,
                                    QLineEdit* lineEdit,
                                    LineEditType lineEditType) {
    if (lineEditType == Text) {
        previewButton->setText(lineEdit->text());
    }
    else {
        if (QIcon::hasThemeIcon(lineEdit->text())) {
            previewButton->setIcon(QIcon::fromTheme(lineEdit->text()));
        }
        else {
            previewButton->setIcon(QIcon(lineEdit->text()));
        }
    }
}

App readDesktopEntry(QString pathToCurrentDesktopFile) {
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
    desktopFileReader.endGroup();

    return myApp;
}

void AppMenuAppletPane::prepareUI(QListWidget* allAppsListWidget,
                                  QListWidget* favAppsListWidget) {
    QDir appDir("/usr/share/applications");
    QStringList desktopFilesList = appDir.entryList();

    favEntries = appMenuAppletConfig["favApps"].toVariant().toStringList();

    QString pathToCurrentDesktopFile;
    App currentApp;

    desktopFilesList.removeFirst();
    desktopFilesList.removeFirst();

    allAppsListWidget->clear();
    favAppsListWidget->clear();
    _entryByItem.clear();

    for (int i = 0; i < desktopFilesList.length(); ++i) {
        pathToCurrentDesktopFile = appDir.absoluteFilePath(desktopFilesList[i]);
        currentApp = readDesktopEntry(pathToCurrentDesktopFile);
        if (currentApp.displayedName != "") {
            QListWidgetItem* item = new QListWidgetItem(currentApp.displayedName);
            item->setIcon(currentApp.icon);
            allAppsListWidget->addItem(item);
            _entryByItem[item] = desktopFilesList[i];

            if (favEntries.contains(desktopFilesList[i])) {
                QListWidgetItem* favItem = new QListWidgetItem(currentApp.displayedName);
                favItem->setIcon(currentApp.icon);
                favAppsListWidget->addItem(favItem);
                _entryByItem[favItem] = desktopFilesList[i];
            }
        }
    }

    allAppsListWidget->setCurrentRow(0);
}

void AppMenuAppletPane::setCurrentSettings(QLineEdit* buttonTextLineEdit,
                                           QLineEdit* buttonIconLineEdit,
                                           QPushButton* previewButton,
                                           QCheckBox* useTriangularTabsCheckBox) {
    buttonTextLineEdit->setText(appMenuAppletConfig["menuText"].toString());
    buttonIconLineEdit->setText(appMenuAppletConfig["menuIcon"].toString());

    makePreview(previewButton, buttonTextLineEdit, LineEditType::Text);
    makePreview(previewButton, buttonIconLineEdit, LineEditType::Icon);

    if (appMenuAppletConfig["appMenuTriangularTabs"].toBool()) {
        if (!useTriangularTabsCheckBox->isChecked()) {
            useTriangularTabsCheckBox->toggle();
        }
    }
    else {
        if (useTriangularTabsCheckBox->isChecked()) {
            useTriangularTabsCheckBox->toggle();
        }
    }
}

void AppMenuAppletPane::addEntry(QListWidget* allAppsListWidget,
                                 QListWidget* favAppsListWidget) {
    if (!allAppsListWidget->selectedItems().isEmpty()) {
        QListWidgetItem* item = allAppsListWidget->selectedItems()[0];

        QString selectedItemText = item->text();
        QIcon selectedItemIcon = item->icon();

        QListWidgetItem* newItem = new QListWidgetItem(selectedItemText);
        newItem->setIcon(selectedItemIcon);
        favAppsListWidget->addItem(newItem);
        _entryByItem[newItem] = _entryByItem[item];
    }
}

void AppMenuAppletPane::removeEntry(QListWidget* favAppsListWidget) {
    if (!favAppsListWidget->selectedItems().isEmpty()) {
        QListWidgetItem* item = favAppsListWidget->selectedItems()[0];
        favAppsListWidget->removeItemWidget(item);
        delete item;
    }
}

void AppMenuAppletPane::saveSettings(QLineEdit* buttonTextLineEdit,
                                     QLineEdit* buttonIconLineEdit,
                                     QListWidget* favAppsListWidget,
                                     QCheckBox* useTriangularTabsCheckBox) {
    QStringList favApps;

    appMenuAppletConfig["menuText"] = buttonTextLineEdit->text();
    appMenuAppletConfig["menuIcon"] = buttonIconLineEdit->text();

    for (short i = 0; i < favAppsListWidget->count(); ++i) {
        favApps << _entryByItem[favAppsListWidget->item(i)];
    }

    appMenuAppletConfig["favApps"] = QJsonValue::fromVariant(QVariant(favApps));

    appMenuAppletConfig["appMenuTriangularTabs"] = QJsonValue(
                                        useTriangularTabsCheckBox->isChecked());

    Pane::saveConfig(appMenuAppletConfig);
}

QWidget* AppMenuAppletPane::createUI() {
    readConfig();

    // UI
    QWidget* appMenuAppletPane = new QWidget;
    appMenuAppletPane->setObjectName("appMenuAppletPane");
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(4, 4, 4, 4);
    appMenuAppletPane->setLayout(layout);

    // Style
    if (appMenuAppletConfig["theme"] == "light") {
        QFile stylesheetReader(":/styles/general-light.qss");
        stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream styleSheet(&stylesheetReader);
        appMenuAppletPane->setStyleSheet(styleSheet.readAll());
    }
    else {
        QFile stylesheetReader(":/styles/general-dark.qss");
        stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream styleSheet(&stylesheetReader);
        appMenuAppletPane->setStyleSheet(styleSheet.readAll());
    }

    QFont paneFont;
    paneFont.setFamily(appMenuAppletConfig["fontFamily"].toString());
    paneFont.setPointSize(appMenuAppletConfig["fontSize"].toInt());
    appMenuAppletPane->setFont(paneFont);

    short width = 400, height = 500;
    appMenuAppletPane->setGeometry(650, 250, width, height);

    QLabel* buttonTextLabel = new QLabel("Button text");
    QLineEdit* buttonTextLineEdit = new QLineEdit;
    appMenuAppletPane->layout()->addWidget(buttonTextLabel);
    appMenuAppletPane->layout()->addWidget(buttonTextLineEdit);

    QLabel* buttonIconLabel = new QLabel("Button icon (path or name)");
    QLineEdit* buttonIconLineEdit = new QLineEdit;
    appMenuAppletPane->layout()->addWidget(buttonIconLabel);
    appMenuAppletPane->layout()->addWidget(buttonIconLineEdit);

    QLabel* previewLabel = new QLabel("Button preview");
    QPushButton* previewButton = new QPushButton;
    appMenuAppletPane->layout()->addWidget(previewLabel);
    appMenuAppletPane->layout()->addWidget(previewButton);

    QLabel* favAppsLabel = new QLabel("Favorite apps");
    appMenuAppletPane->layout()->addWidget(favAppsLabel);

    QListWidget* allAppsListWidget = new QListWidget;
    appMenuAppletPane->layout()->addWidget(allAppsListWidget);
    allAppsListWidget->setStyleSheet("QListView::item:selected { background-color: " + \
                                     appMenuAppletConfig["accent"].toString() + \
                                     "; color: #ffffff };");

    QListWidget* favAppsListWidget = new QListWidget;
    appMenuAppletPane->layout()->addWidget(favAppsListWidget);
    favAppsListWidget->setStyleSheet("QListView::item:selected { background-color: " + \
                                     appMenuAppletConfig["accent"].toString() + \
                                     "; color: #ffffff };");

    QPushButton* addEntryPushButton = new QPushButton("Add");
    appMenuAppletPane->layout()->addWidget(addEntryPushButton);

    QPushButton* removeEntryPushButton = new QPushButton("Remove");
    appMenuAppletPane->layout()->addWidget(removeEntryPushButton);

    QCheckBox* useTriangularTabsCheckBox = new QCheckBox("Use triangular tabs (uncheck only on light theme)");
    appMenuAppletPane->layout()->addWidget(useTriangularTabsCheckBox);

    QPushButton* revertPushButton = new QPushButton("Revert");
    appMenuAppletPane->layout()->addWidget(revertPushButton);

    QPushButton* savePushButton = new QPushButton("Save");
    appMenuAppletPane->layout()->addWidget(savePushButton);

    setCurrentSettings(buttonTextLineEdit, buttonIconLineEdit,
                       previewButton, useTriangularTabsCheckBox);
    prepareUI(allAppsListWidget, favAppsListWidget);


    // Make connections
    appMenuAppletPane->connect(buttonTextLineEdit, &QLineEdit::textEdited, appMenuAppletPane,
                               [this, previewButton, buttonTextLineEdit]() {
        makePreview(previewButton, buttonTextLineEdit, LineEditType::Text);
    });

    appMenuAppletPane->connect(buttonIconLineEdit, &QLineEdit::textEdited, appMenuAppletPane,
                               [this, previewButton, buttonIconLineEdit]() {
        makePreview(previewButton, buttonIconLineEdit, LineEditType::Icon);
    });

    appMenuAppletPane->connect(addEntryPushButton, &QPushButton::clicked, appMenuAppletPane,
                               [this, allAppsListWidget, favAppsListWidget]() {
        addEntry(allAppsListWidget, favAppsListWidget);
    });

    appMenuAppletPane->connect(removeEntryPushButton, &QPushButton::clicked, appMenuAppletPane,
                               [this, favAppsListWidget]() {
        removeEntry(favAppsListWidget);
    });

    appMenuAppletPane->connect(revertPushButton, &QPushButton::clicked, appMenuAppletPane,
                               [this, allAppsListWidget, favAppsListWidget, previewButton,
                                buttonTextLineEdit, buttonIconLineEdit,
                                useTriangularTabsCheckBox]() {
        setCurrentSettings(buttonTextLineEdit, buttonIconLineEdit, previewButton,
                           useTriangularTabsCheckBox);
        prepareUI(allAppsListWidget, favAppsListWidget);
    });

    appMenuAppletPane->connect(savePushButton, &QPushButton::clicked, appMenuAppletPane,
                               [this, buttonTextLineEdit, buttonIconLineEdit,
                               favAppsListWidget, useTriangularTabsCheckBox]() {
        saveSettings(buttonTextLineEdit, buttonIconLineEdit,
                     favAppsListWidget, useTriangularTabsCheckBox);
    });

    return appMenuAppletPane;
}
