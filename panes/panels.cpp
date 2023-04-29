#include "panels.h"
#include "ui_panels.h"

#include "pane.h"
#include "settings.h"
#include "appletPanes/appmenu.h"
#include "appletPanes/datetime.h"
#include "appletPanes/localipv4.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QMessageBox>
#include <QInputDialog>

QJsonObject panelsPaneConfig;
QHash<QString,QString> iconByApplet;
QHash<QString,QString> nameByApplet;

void PanelsPane::readConfig() {
    QString homeDirectory = getenv("HOME");
    QFile file;
    QString data;

    file.setFileName(homeDirectory + "/.config/plainDE/config.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();
    panelsPaneConfig = QJsonDocument::fromJson(data.toUtf8()).object();
}

void PanelsPane::setCurrentSettings(qint8 panelNumber) {
    QString panelName = "panel" + QString::number(panelNumber);

    // Show enabled applets
    ui->enabledAppletsListWidget->clear();
    foreach (QJsonValue applet, panelsPaneConfig[panelName].toObject()["applets"].toArray()) {
        QListWidgetItem* item = new QListWidgetItem;
        item->setText(applet.toString());
        if (!applet.toString().startsWith("launcher")) {
            item->setIcon(QIcon::fromTheme(iconByApplet[applet.toString()]));
        }
        else {
            QString desktopEntryPath = "/usr/share/applications/" + applet.toString().split(':')[1];
            QString iconPath;

            QSettings desktopFileReader(desktopEntryPath, QSettings::IniFormat);
            desktopFileReader.sync();
            desktopFileReader.beginGroup("Desktop Entry");
                iconPath = desktopFileReader.value("Icon").toString();
            desktopFileReader.endGroup();

            if (QIcon::hasThemeIcon(iconPath)) {
                item->setIcon(QIcon::fromTheme(iconPath));
            }
            else {
                if (QFile::exists(iconPath)) {
                    item->setIcon(QIcon(iconPath));
                }
                else {
                    item->setIcon(QIcon::fromTheme("terminal"));
                }
            }
        }
        ui->enabledAppletsListWidget->addItem(item);
    }

    // Launcher icon size
    ui->launcherIconSizeSpinBox->setValue(panelsPaneConfig[panelName].toObject()["launcherIconSize"].toInt());

    // Expand panel
    ui->expandCheckBox->setChecked(false);
    if (panelsPaneConfig[panelName].toObject()["expand"].toBool()) {
        ui->expandCheckBox->setChecked(true);
    }

    // X Offset
    ui->shiftSpinBox->setValue(panelsPaneConfig[panelName].toObject()["shift"].toInt());

    // Panel height
    ui->thicknessSpinBox->setValue(panelsPaneConfig[panelName].toObject()["thickness"].toInt());

    // Panel location
    if (panelsPaneConfig[panelName].toObject()["location"].toString() == "top") {
        ui->topRadioButton->setChecked(true);
    }
    else if (panelsPaneConfig[panelName].toObject()["location"].toString() == "bottom") {
        ui->bottomRadioButton->setChecked(true);
    }
    else if (panelsPaneConfig[panelName].toObject()["location"].toString() == "left") {
        ui->leftRadioButton->setChecked(true);
    }
    else { // right
        ui->rightRadioButton->setChecked(true);
    }

    // Panel opacity
    ui->opacitySpinBox->setValue(panelsPaneConfig[panelName].toObject()["opacity"].toDouble());

    // Spacing between applets
    ui->spacingSpinBox->setValue(panelsPaneConfig[panelName].toObject()["spacing"].toInt());
}

void PanelsPane::saveSettings(qint8 panelNumber) {
    QString panelName = "panel" + QString::number(panelNumber);

    // Enabled applets
    QJsonArray enabledApplets;
    for (int i = 0; i < ui->enabledAppletsListWidget->count(); ++i) {
        enabledApplets << QJsonValue(ui->enabledAppletsListWidget->item(i)->text());
    }

    QJsonObject panelObject = panelsPaneConfig[panelName].toObject();
    panelObject["applets"] = QJsonValue(enabledApplets);

    // Launcher icon size
    panelObject["launcherIconSize"] = ui->launcherIconSizeSpinBox->value();

    // Expand panel
    panelObject["expand"] = QJsonValue(ui->expandCheckBox->isChecked());

    // X Offset
    panelObject["shift"] = QJsonValue(ui->shiftSpinBox->value());

    // Panel height
    panelObject["thickness"] = QJsonValue(ui->thicknessSpinBox->value());

    // Panel location
    if (ui->topRadioButton->isChecked()) {
        panelObject["location"] = QJsonValue("top");
    }
    else if (ui->bottomRadioButton->isChecked()) {
        panelObject["location"] = QJsonValue("bottom");
    }
    else if (ui->leftRadioButton->isChecked()) {
        panelObject["location"] = QJsonValue("left");
    }
    else { // right
        panelObject["location"] = QJsonValue("right");
    }

    // Panel opacity
    panelObject["opacity"] = QJsonValue(ui->opacitySpinBox->value());

    // Spacing between applets
    panelObject["spacing"] = QJsonValue(ui->spacingSpinBox->value());


    panelsPaneConfig[panelName] = panelObject;
    Pane::saveConfig(panelsPaneConfig);
}

PanelsPane::PanelsPane(QWidget *parent, Settings* controlCenter) :
    QWidget(parent),
    ui(new Ui::PanelsPane) {
    ui->setupUi(this);

    this->setObjectName("panelsPane");

    readConfig();

    // Theme & Font
    QFile stylesheetReader("/usr/share/plainDE/styles/" + panelsPaneConfig["theme"].toString());
    stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream styleSheet(&stylesheetReader);
    this->setStyleSheet(styleSheet.readAll());
    stylesheetReader.close();

    QFont paneFont;
    paneFont.setFamily(panelsPaneConfig["fontFamily"].toString());
    paneFont.setPointSize(panelsPaneConfig["fontSize"].toInt());
    this->setFont(paneFont);

    // QListWidget's accent
    ui->panelsListWidget->setStyleSheet("QListView::item:selected { background-color: " + \
                                        panelsPaneConfig["accent"].toString() + \
                                        "; color: #ffffff };");
    ui->availableAppletsListWidget->setStyleSheet("QListView::item:selected { background-color: " + \
                                                   panelsPaneConfig["accent"].toString() + \
                                                   "; color: #ffffff };");
    ui->enabledAppletsListWidget->setStyleSheet("QListView::item:selected { background-color: " + \
                                                panelsPaneConfig["accent"].toString() + \
                                                "; color: #ffffff };");

    // Applet icons
    iconByApplet["appmenu"] = "app-launcher";
    iconByApplet["windowlist"] = "kwin";
    iconByApplet["spacer"] = "extensions";
    iconByApplet["workspaces"] = "cs-workspaces";
    iconByApplet["volume"] = "sound";
    iconByApplet["kblayout"] = "keyboard";
    iconByApplet["datetime"] = "calendar";
    iconByApplet["splitter"] = "extensions";
    iconByApplet["usermenu"] = "user_icon";
    iconByApplet["localipv4"] = "cs-network";
    iconByApplet["mpris"] = "sound";
    iconByApplet["launcher"] = "terminal";
    iconByApplet["battery"] = "extensions";
    iconByApplet["sni"] = "extensions";

    // Applet human-readable names
    nameByApplet["appmenu"] = "App Menu";
    nameByApplet["windowlis"] = "Window List";
    nameByApplet["spacer"] = "Spacer";
    nameByApplet["workspaces"] = "Workspaces Indicator";
    nameByApplet["volume"] = "Volume Dial";
    nameByApplet["kblayout"] = "Keyboard Layout Indicator";
    nameByApplet["datetime"] = "Date & Time";
    nameByApplet["splitter"] = "Splitter (|)";
    nameByApplet["usermenu"] = "User Menu";
    nameByApplet["localipv4"] = "Local IPv4 Indicator";
    nameByApplet["mpris"] = "Playback Control";
    nameByApplet["launcher"] = "Launcher";
    nameByApplet["battery"] = "Battery Indicator";
    nameByApplet["sni"] = "SNI tray";


    for (qint8 i = 0; i < ui->availableAppletsListWidget->count(); ++i) {
        QListWidgetItem* item = ui->availableAppletsListWidget->item(i);
        item->setIcon(QIcon::fromTheme(iconByApplet[item->text()]));
    }

    // List panels
    ui->panelsListWidget->clear();
    for (int i = 1; i <= panelsPaneConfig["countPanels"].toInt(); ++i) {
        if (!panelsPaneConfig["panel" + QString::number(i)].isNull()) {
            ui->panelsListWidget->addItem("Panel " + QString::number(i));
        }
    }

    // Set max shift
    ui->shiftSpinBox->setMaximum(QGuiApplication::primaryScreen()->geometry().width());

    // Make connections
    this->connect(ui->panelsListWidget, &QListWidget::itemClicked, this,
                  [this]() {
        QString panelName = ui->panelsListWidget->selectedItems()[0]->text();
        qint8 panelNumber = panelName.midRef(6, 6).toInt();
        setCurrentSettings(panelNumber);
    });

    this->connect(ui->backPushButton, &QPushButton::clicked, this,
                  [this, controlCenter]() {
        controlCenter->mPanelsWidgetVisible = false;
        /*if (mAppMenuAppletPane != NULL) {
            delete mAppMenuAppletPane;
            controlCenter->mAppMenuWidgetVisible = false;
        }
        if (mDatetimeAppletPane != NULL) {
            delete mDatetimeAppletPane;
            controlCenter->mDateTimeWidgetVisible = false;
        }
        if (mLocalIPv4AppletPane != NULL) {
            delete mLocalIPv4AppletPane;
            controlCenter->mLocalIPv4WidgetVisible = false;
        }*/
        this->hide();
        delete this;
    });

    this->connect(ui->addPanelPushButton, &QPushButton::clicked, this,
                  [this]() {
        if (ui->panelsListWidget->count() < 4) {
            QJsonObject panelObject;
            QString panelName, visibleName;
            qint8 newPanelNumber;
            panelObject["expand"] = QJsonValue(true);
            panelObject["shift"] = QJsonValue(0);
            panelObject["thickness"] = QJsonValue(28);
            panelObject["opacity"] = QJsonValue(0.85);
            panelObject["launcherIconSize"] = QJsonValue(22);
            panelObject["spacing"] = QJsonValue(5);

            if (ui->panelsListWidget->count() == 0) {
                newPanelNumber = 1;
                panelName = "panel1";
                visibleName = "Panel 1";
                panelObject["location"] = QJsonValue("top");
            }
            else {
                QString existingVisibleName = ui->panelsListWidget->item(ui->panelsListWidget->count() - 1)->text();
                qint8 existingPanelNumber = existingVisibleName.midRef(6, 6).toInt();
                newPanelNumber = existingPanelNumber + 1;
                panelName = "panel" + QString::number(newPanelNumber);
                visibleName = "Panel " + QString::number(newPanelNumber);

                QStringList busyLocations;
                for (int i = 0; i < ui->panelsListWidget->count(); ++i) {
                    qint8 currentPanelNumber = ui->panelsListWidget->item(i)->text().midRef(6, 6).toInt();
                    busyLocations.append(panelsPaneConfig["panel" + QString::number(currentPanelNumber)].toObject()["location"].toString());
                }

                if (!busyLocations.contains("top")) {
                    panelObject["location"] = QJsonValue("top");
                }
                else if (!busyLocations.contains("bottom")) {
                    panelObject["location"] = QJsonValue("bottom");
                }
                else if (!busyLocations.contains("left")) {
                    panelObject["location"] = QJsonValue("left");
                }
                else { // right
                    panelObject["location"] = QJsonValue("right");
                }
            }

            panelsPaneConfig[panelName] = panelObject;
            ui->panelsListWidget->addItem(visibleName);
            setCurrentSettings(newPanelNumber);
            ui->panelsListWidget->setCurrentRow(ui->panelsListWidget->count() - 1);
        }
        else {
            QMessageBox msg;
            msg.setWindowTitle("Error");
            msg.setText("4 panels maximum allowed.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setIcon(QMessageBox::Critical);
            msg.exec();
        }
    });

    this->connect(ui->removePanelPushButton, &QPushButton::clicked, this,
                  [this]() {
        if (ui->panelsListWidget->count() > 1) {
            if (!ui->panelsListWidget->selectedItems().isEmpty()) {
                QListWidgetItem* delPanelItem = ui->panelsListWidget->selectedItems()[0];
                qint8 delPanelNumber = delPanelItem->text().midRef(6, 6).toInt();
                panelsPaneConfig.remove("panel" + QString::number(delPanelNumber));
                delete delPanelItem;

                setCurrentSettings(ui->panelsListWidget->count());
                ui->panelsListWidget->setCurrentRow(ui->panelsListWidget->count() - 1);
            }
        }
        else {
            QMessageBox msg;
            msg.setWindowTitle("Error");
            msg.setText("At least 1 panel must exist.");
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setIcon(QMessageBox::Critical);
            msg.exec();
        }
    });

    this->connect(ui->addAppletPushButton, &QPushButton::clicked, this,
                  [this]() {
        foreach (QListWidgetItem* item, ui->availableAppletsListWidget->selectedItems()) {
            if (ui->enabledAppletsListWidget->findItems(item->text(), Qt::MatchExactly).isEmpty()) {
                if (item->text().startsWith("launcher")) {
                    bool ok;
                    QString filename = QInputDialog::getText(this,
                                                             "Select app",
                                                             "Type one of filenames from "
                                                             "/usr/share/applications/ or "
                                                             "~/.local/share/applications "
                                                             "(i.e., app.desktop)",
                                                             QLineEdit::Normal,
                                                             "",
                                                             &ok);


                    if (ok && !filename.isEmpty()) {
                        QString desktopEntryPath;
                        if (QFile::exists("/usr/share/applications" + filename)) {
                            desktopEntryPath = "/usr/share/applications/" + filename;
                        }
                        else {
                            QString homeDir = getenv("HOME");
                            desktopEntryPath = homeDir + "/.local/share/applications/" + filename;
                        }

                        QString iconPath;

                        QListWidgetItem* addedItem = new QListWidgetItem;
                        addedItem->setText("launcher:" + filename);

                        QSettings desktopFileReader(desktopEntryPath, QSettings::IniFormat);
                        desktopFileReader.sync();
                        desktopFileReader.beginGroup("Desktop Entry");
                            iconPath = desktopFileReader.value("Icon").toString();
                        desktopFileReader.endGroup();

                        if (QIcon::hasThemeIcon(iconPath)) {
                            addedItem->setIcon(QIcon::fromTheme(iconPath));
                        }
                        else {
                            if (QFile::exists(iconPath)) {
                                addedItem->setIcon(QIcon(iconPath));
                            }
                            else {
                                addedItem->setIcon(QIcon::fromTheme("terminal"));
                            }
                        }

                        ui->enabledAppletsListWidget->addItem(addedItem);
                    }
                }
                else {
                    QListWidgetItem* addedItem = new QListWidgetItem;
                    addedItem->setText(item->text());
                    addedItem->setIcon(QIcon::fromTheme(iconByApplet[item->text()]));
                    ui->enabledAppletsListWidget->addItem(addedItem);
                }
            }
            else {
                if (item->text() == "splitter" ||
                     item->text() == "spacer") {
                    QListWidgetItem* addedItem = new QListWidgetItem;
                    addedItem->setText(item->text());
                    addedItem->setIcon(QIcon::fromTheme(iconByApplet[item->text()]));
                    ui->enabledAppletsListWidget->addItem(addedItem);
                }
                else {
                    QMessageBox msg;
                    msg.setWindowTitle("Error");
                    msg.setText("This applet is not allowed to be added multiple times.");
                    msg.setStandardButtons(QMessageBox::Ok);
                    msg.setIcon(QMessageBox::Critical);
                    msg.exec();
                }
            }
        }
    });

    this->connect(ui->removeAppletPushButton, &QPushButton::clicked, this,
                  [this]() {
        if (!ui->enabledAppletsListWidget->selectedItems().isEmpty()) {
            foreach (QListWidgetItem* item, ui->enabledAppletsListWidget->selectedItems()) {
                delete item;
            }
        }
    });

    this->connect(ui->moveUpPushButton, &QPushButton::clicked, this,
                  [this]() {
        if (!ui->enabledAppletsListWidget->selectedItems().isEmpty()) {
            int selectedIndex = ui->enabledAppletsListWidget->currentRow();
            if (selectedIndex > 0) {
                QListWidgetItem* item = ui->enabledAppletsListWidget->selectedItems()[0];
                QString text = item->text();
                QIcon icon = item->icon();
                delete item;
                ui->enabledAppletsListWidget->insertItem(selectedIndex-1, new QListWidgetItem(icon, text));
                ui->enabledAppletsListWidget->setCurrentRow(selectedIndex-1);
            }
        }
    });

    this->connect(ui->moveDownPushButton, &QPushButton::clicked, this,
                  [this]() {
        if (!ui->enabledAppletsListWidget->selectedItems().isEmpty()) {
            int selectedIndex = ui->enabledAppletsListWidget->currentRow();
            if (selectedIndex < (ui->enabledAppletsListWidget->count() - 1)) {
                QListWidgetItem* item = ui->enabledAppletsListWidget->selectedItems().at(0);
                QString text = item->text();
                QIcon icon = item->icon();
                delete item;
                ui->enabledAppletsListWidget->insertItem(selectedIndex+1, new QListWidgetItem(icon, text));
                ui->enabledAppletsListWidget->setCurrentRow(selectedIndex+1);
            }
        }
    });

    this->connect(ui->configureAppletPushButton, &QPushButton::clicked, this,
                  [this, controlCenter]() {
        if (!ui->enabledAppletsListWidget->selectedItems().isEmpty()) {
            QString applet = ui->enabledAppletsListWidget->selectedItems().at(0)->text();
            if (applet == "appmenu") {
                if (!controlCenter->mAppMenuWidgetVisible) {
                    controlCenter->mAppMenuWidgetVisible = true;
                    mAppMenuAppletPane = new AppMenuAppletPane;
                    QWidget* appMenuAppletWidget = mAppMenuAppletPane->createUI(controlCenter);
                    controlCenter->layout()->addWidget(appMenuAppletWidget);
                }
            }
            else if (applet == "datetime") {
                if (!controlCenter->mDateTimeWidgetVisible) {
                    mDatetimeAppletPane = new DatetimeAppletPane;
                    QWidget* datetimeAppletWidget = mDatetimeAppletPane->createUI(controlCenter);
                    controlCenter->layout()->addWidget(datetimeAppletWidget);
                }
            }
            else if (applet == "localipv4") {
                if (!controlCenter->mLocalIPv4WidgetVisible) {
                    mLocalIPv4AppletPane = new LocalIPv4AppletPane;
                    QWidget* localIPv4AppletWidget = mLocalIPv4AppletPane->createUI(controlCenter);
                    controlCenter->layout()->addWidget(localIPv4AppletWidget);
                }
            }
            else {
                QMessageBox msg;
                msg.setWindowTitle("Information");
                msg.setText("Nothing to configure in this applet.");
                msg.setStandardButtons(QMessageBox::Ok);
                msg.setIcon(QMessageBox::Information);
                msg.exec();
            }
        }
    });

    this->connect(ui->expandCheckBox, &QCheckBox::toggled, this,
                  [this]() {
        if (ui->expandCheckBox->isChecked()) {
            ui->shiftSpinBox->setEnabled(false);
            ui->shiftSpinBox->setValue(0);
        }
        else {
            ui->shiftSpinBox->setEnabled(true);
        }
    });

    this->connect(ui->revertPushButton, &QPushButton::clicked, this,
                  [this]() {
        if (!ui->panelsListWidget->selectedItems().isEmpty()) {
            QString panelName = ui->panelsListWidget->selectedItems()[0]->text();
            qint8 panelNumber = panelName.midRef(6, 6).toInt();
            setCurrentSettings(panelNumber);
        }
    });

    this->connect(ui->savePushButton, &QPushButton::clicked, this,
                  [this]() {
        if (!ui->panelsListWidget->selectedItems().isEmpty()) {
            QString panelName = ui->panelsListWidget->selectedItems()[0]->text();
            qint8 panelNumber = panelName.midRef(6, 6).toInt();
            saveSettings(panelNumber);
        }
    });

}

PanelsPane::~PanelsPane() {
    delete ui;
}
