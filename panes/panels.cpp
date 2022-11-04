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

    // Expand panel
    ui->expandCheckBox->setChecked(false);
    if (panelsPaneConfig[panelName].toObject()["expand"].toBool()) {
        ui->expandCheckBox->setChecked(true);
    }

    // X Offset
    ui->xOffsetSpinBox->setValue(panelsPaneConfig[panelName].toObject()["xOffset"].toInt());

    // Panel height
    ui->heightSpinBox->setValue(panelsPaneConfig[panelName].toObject()["height"].toInt());

    // Panel location
    if (panelsPaneConfig[panelName].toObject()["location"].toString() == "top") {
        ui->topRadioButton->setChecked(true);
    }
    else {
        ui->bottomRadioButton->setChecked(true);
    }

    // Panel opacity
    ui->opacitySpinBox->setValue(panelsPaneConfig[panelName].toObject()["opacity"].toDouble());
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

    // Expand panel
    panelObject["expand"] = QJsonValue(ui->expandCheckBox->isChecked());

    // X Offset
    panelObject["xOffset"] = QJsonValue(ui->xOffsetSpinBox->value());

    // Panel height
    panelObject["height"] = QJsonValue(ui->heightSpinBox->value());

    // Panel location
    if (ui->topRadioButton->isChecked()) {
        panelObject["location"] = QJsonValue("top");
    }
    else {
        panelObject["location"] = QJsonValue("bottom");
    }

    // Panel opacity
    panelObject["opacity"] = QJsonValue(ui->opacitySpinBox->value());

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

    for (qint8 i = 0; i < ui->availableAppletsListWidget->count(); ++i) {
        QListWidgetItem* item = ui->availableAppletsListWidget->item(i);
        item->setIcon(QIcon::fromTheme(iconByApplet[item->text()]));
    }

    // List panels
    ui->panelsListWidget->clear();
    if (panelsPaneConfig.contains("panel1")) {
        ui->panelsListWidget->addItem("Panel 1");
    }
    if (panelsPaneConfig.contains("panel2")) {
        ui->panelsListWidget->addItem("Panel 2");
    }

    // Set max X offset
    ui->xOffsetSpinBox->setMaximum(QGuiApplication::primaryScreen()->geometry().width());

    // Make connections
    this->connect(ui->panelsListWidget, &QListWidget::itemClicked, this,
                  [this]() {
        QString panelName = ui->panelsListWidget->selectedItems()[0]->text();
        qint8 panelNumber = panelName.midRef(6, 6).toInt();
        setCurrentSettings(panelNumber);
    });

    this->connect(ui->backPushButton, &QPushButton::clicked, this,
                  [this, controlCenter]() {
        this->hide();
        controlCenter->controlCenterWidget->show();
    });

    this->connect(ui->addPanelPushButton, &QPushButton::clicked, this,
                  [this]() {
        if (ui->panelsListWidget->count() < 2) {
            QJsonObject panelObject;
            QString panelName, visibleName;
            qint8 newPanelNumber;
            panelObject["expand"] = QJsonValue(true);
            panelObject["xOffset"] = QJsonValue(0);
            panelObject["height"] = QJsonValue(28);
            panelObject["opacity"] = QJsonValue(0.85);

            if (ui->panelsListWidget->count() == 0) {
                newPanelNumber = 1;
                panelName = "panel1";
                visibleName = "Panel 1";
                panelObject["location"] = QJsonValue("top");
            }
            else {
                QString existingVisibleName = ui->panelsListWidget->item(0)->text();
                qint8 existingPanelNumber = existingVisibleName.midRef(6, 6).toInt();
                newPanelNumber = existingPanelNumber == 1 ? 2 : 1;
                panelName = "panel" + QString::number(newPanelNumber);
                visibleName = "Panel " + QString::number(newPanelNumber);

                if (panelsPaneConfig["panel" + QString::number(existingPanelNumber)].toObject()["location"] == "top") {
                    panelObject["location"] = QJsonValue("bottom");
                }
                else {
                    panelObject["location"] = QJsonValue("top");
                }
            }

            panelsPaneConfig[panelName] = panelObject;
            ui->panelsListWidget->addItem(visibleName);
            setCurrentSettings(newPanelNumber);
            ui->panelsListWidget->setCurrentRow(1);
        }
        else {
            QMessageBox msg;
            msg.setWindowTitle("Error");
            msg.setText("2 panels maximum allowed.");
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

                setCurrentSettings(delPanelNumber == 2 ? 1 : 2);
                ui->panelsListWidget->setCurrentRow(0);
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
                                                             "/usr/share/applications/ (i.e., app.desktop)",
                                                             QLineEdit::Normal,
                                                             "",
                                                             &ok);
                    if (ok && !filename.isEmpty()) {
                        QString desktopEntryPath = "/usr/share/applications/" + filename;
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
                  [this]() {
        if (!ui->enabledAppletsListWidget->selectedItems().isEmpty()) {
            QString applet = ui->enabledAppletsListWidget->selectedItems().at(0)->text();
            if (applet == "appmenu") {
                AppMenuAppletPane* appMenuAppletPane = new AppMenuAppletPane;
                QWidget* appMenuAppletWidget = appMenuAppletPane->createUI();
                appMenuAppletWidget->show();
            }
            else if (applet == "datetime") {
                DatetimeAppletPane* datetimeAppletPane = new DatetimeAppletPane;
                QWidget* datetimeAppletWidget = datetimeAppletPane->createUI();
                datetimeAppletWidget->show();
            }
            else if (applet == "localipv4") {
                LocalIPv4AppletPane* localIPv4AppletPane = new LocalIPv4AppletPane;
                QWidget* localIPv4AppletWidget = localIPv4AppletPane->createUI();
                localIPv4AppletWidget->show();
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
            ui->xOffsetSpinBox->setEnabled(false);
            ui->xOffsetSpinBox->setValue(0);
        }
        else {
            ui->xOffsetSpinBox->setEnabled(true);
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
