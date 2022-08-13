#include "applets.h"

#include "appletPanes/appmenu.h"
#include "appletPanes/datetime.h"
#include "appletPanes/localipv4.h"

QJsonObject appletsPaneConfig;
QHash<QListWidgetItem*,QString> nameByItem;
QHash<QString,QString> iconByApplet;


void AppletsPane::readConfig() {
    QString homeDirectory = getenv("HOME");
    QFile file;
    QString data;

    file.setFileName(homeDirectory + "/.config/plainDE/config.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();
    appletsPaneConfig = QJsonDocument::fromJson(data.toUtf8()).object();
}

void AppletsPane::prepareUI(QListWidget* allAppletsListWidget,
                            QListWidget* enabledAppletsListWidget) {
    allAppletsListWidget->clear();
    enabledAppletsListWidget->clear();
    nameByItem.clear();

    iconByApplet["appmenu"] = "app-launcher";
    iconByApplet["windowlist"] = "kwin";
    iconByApplet["spacer"] = "extensions";
    iconByApplet["workspaces"] = "cs-workspaces";
    iconByApplet["volume"] = "sound";
    iconByApplet["kblayout"] = "keyboard";
    iconByApplet["datetime"] = "calendar";
    iconByApplet["splitter"] = "extensions";
    iconByApplet["usermenu"] = "user_icon";
    iconByApplet["localipv4"] = "extensions";

    QStringList enabledApplets = appletsPaneConfig["applets"].toVariant().toStringList();

    foreach (QString currentApplet, iconByApplet.keys()) {
        QListWidgetItem* item = new QListWidgetItem(currentApplet);
        item->setIcon(QIcon::fromTheme(iconByApplet[currentApplet]));
        allAppletsListWidget->addItem(item);
        nameByItem[item] = currentApplet;
    }

    foreach (QString currentApplet, enabledApplets) {
        QListWidgetItem* item = new QListWidgetItem(currentApplet);
        item->setIcon(QIcon::fromTheme(iconByApplet[currentApplet]));
        enabledAppletsListWidget->addItem(item);
        nameByItem[item] = currentApplet;
    }
}

void AppletsPane::addEntry(QListWidget* allAppletsListWidget,
                           QListWidget* enabledAppletsListWidget) {
    if (!allAppletsListWidget->selectedItems().isEmpty()) {
        QListWidgetItem* selectedItem = allAppletsListWidget->selectedItems()[0];
        QListWidgetItem* newItem = new QListWidgetItem(selectedItem->text());
        newItem->setIcon(QIcon::fromTheme(iconByApplet[selectedItem->text()]));

        enabledAppletsListWidget->addItem(newItem);
        nameByItem[newItem] = nameByItem[selectedItem];
    }
}

void AppletsPane::removeEntry(QListWidget* enabledAppletsListWidget) {
    if (!enabledAppletsListWidget->selectedItems().isEmpty()) {
        QListWidgetItem* selectedItem = enabledAppletsListWidget->selectedItems()[0];
        enabledAppletsListWidget->removeItemWidget(selectedItem);
        delete selectedItem;
    }
}

void AppletsPane::saveSettings(QListWidget* enabledAppletsListWidget) {
    QStringList enabledApplets;
    for (short i = 0; i < enabledAppletsListWidget->count(); ++i) {
        enabledApplets << enabledAppletsListWidget->item(i)->text();
    }
    appletsPaneConfig["applets"] = QJsonValue::fromVariant(QVariant(enabledApplets));
    Pane::saveConfig(appletsPaneConfig);
}

QWidget* AppletsPane::createUI(QWidget* controlCenter) {
    readConfig();

    // UI
    QWidget* appletsPane = new QWidget;
    appletsPane->setObjectName("appletsPane");
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(4, 4, 4, 4);
    appletsPane->setLayout(layout);

    // Theme
    QFile stylesheetReader("/usr/share/plainDE/styles/" + appletsPaneConfig["theme"].toString());
    stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream styleSheet(&stylesheetReader);
    appletsPane->setStyleSheet(styleSheet.readAll());

    QFont paneFont;
    paneFont.setFamily(appletsPaneConfig["fontFamily"].toString());
    paneFont.setPointSize(appletsPaneConfig["fontSize"].toInt());
    appletsPane->setFont(paneFont);

    short width = 400, height = 500;
    appletsPane->setGeometry(250, 250, width, height);

    QPushButton* backPushButton = new QPushButton("Back");
    backPushButton->setIcon(QIcon::fromTheme("go-previous"));
    appletsPane->layout()->addWidget(backPushButton);

    QLabel* allAppletsLabel = new QLabel("All applets");
    appletsPane->layout()->addWidget(allAppletsLabel);

    QListWidget* allAppletsListWidget = new QListWidget;
    appletsPane->layout()->addWidget(allAppletsListWidget);
    allAppletsListWidget->setStyleSheet("QListView::item:selected { background-color: " + \
                                        appletsPaneConfig["accent"].toString() + \
                                        "; color: #ffffff };");

    QLabel* enabledAppletsLabel = new QLabel("Enabled applets");
    appletsPane->layout()->addWidget(enabledAppletsLabel);

    QListWidget* enabledAppletsListWidget = new QListWidget;
    appletsPane->layout()->addWidget(enabledAppletsListWidget);
    enabledAppletsListWidget->setStyleSheet("QListView::item:selected { background-color: " + \
                                            appletsPaneConfig["accent"].toString() + \
                                            "; color: #ffffff };");

    QPushButton* addEntryPushButton = new QPushButton("Add");
    appletsPane->layout()->addWidget(addEntryPushButton);

    QPushButton* removeEntryPushButton = new QPushButton("Remove");
    appletsPane->layout()->addWidget(removeEntryPushButton);

    QPushButton* customizePushButton = new QPushButton("Customize selected applet");
    appletsPane->layout()->addWidget(customizePushButton);

    QPushButton* revertPushButton = new QPushButton("Revert");
    appletsPane->layout()->addWidget(revertPushButton);

    QPushButton* savePushButton = new QPushButton("Save");
    appletsPane->layout()->addWidget(savePushButton);

    prepareUI(allAppletsListWidget, enabledAppletsListWidget);

    // Applets panes
    AppMenuAppletPane* appMenuAppletPane = new AppMenuAppletPane;
    DatetimeAppletPane* datetimeAppletPane = new DatetimeAppletPane;
    LocalIPv4AppletPane* localIPv4AppletPane = new LocalIPv4AppletPane;

    QWidget* appMenuAppletWidget = appMenuAppletPane->createUI();
    QWidget* datetimeAppletWidget = datetimeAppletPane->createUI();
    QWidget* localIPv4AppletWidget = localIPv4AppletPane->createUI();


    // Make connections
    appletsPane->connect(backPushButton, &QPushButton::clicked, appletsPane,
                         [controlCenter, appletsPane]() {
        controlCenter->show();
        appletsPane->hide();
        delete appletsPane;
    });

    appletsPane->connect(addEntryPushButton, &QPushButton::clicked, appletsPane,
                         [this, allAppletsListWidget, enabledAppletsListWidget]() {
        addEntry(allAppletsListWidget, enabledAppletsListWidget);
    });

    appletsPane->connect(removeEntryPushButton, &QPushButton::clicked, appletsPane,
                         [this, enabledAppletsListWidget]() {
        removeEntry(enabledAppletsListWidget);
    });

    appletsPane->connect(customizePushButton, &QPushButton::clicked, appletsPane,
                         [enabledAppletsListWidget,
                          appMenuAppletWidget, appMenuAppletPane,
                          datetimeAppletWidget, datetimeAppletPane,
                          localIPv4AppletWidget, localIPv4AppletPane]()mutable {
        if (!enabledAppletsListWidget->selectedItems().isEmpty()) {
            if (enabledAppletsListWidget->selectedItems()[0]->text() == "appmenu") {
                appMenuAppletWidget = appMenuAppletPane->createUI();
                appMenuAppletWidget->show();
            }
            else if (enabledAppletsListWidget->selectedItems()[0]->text() == "datetime") {
                datetimeAppletWidget = datetimeAppletPane->createUI();
                datetimeAppletWidget->show();
            }
            else if (enabledAppletsListWidget->selectedItems()[0]->text() == "localipv4") {
                localIPv4AppletWidget = localIPv4AppletPane->createUI();
                localIPv4AppletWidget->show();
            }
            else {
                QMessageBox msg;
                msg.setWindowTitle("No such pane");
                msg.setText("Nothing to configure in this applet.");
                msg.setStandardButtons(QMessageBox::Ok);
                msg.setIcon(QMessageBox::Information);
                msg.exec();
            }
        }
    });

    appletsPane->connect(revertPushButton, &QPushButton::clicked, appletsPane,
                         [this, allAppletsListWidget, enabledAppletsListWidget]() {
        prepareUI(allAppletsListWidget, enabledAppletsListWidget);
    });

    appletsPane->connect(savePushButton, &QPushButton::clicked, appletsPane,
                         [this, enabledAppletsListWidget]() {
        saveSettings(enabledAppletsListWidget);
    });

    return appletsPane;
}
