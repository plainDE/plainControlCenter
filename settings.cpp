#include "settings.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
#include <QString>
#include <QIcon>
#include <QFont>
#include <QProcess>
#include <QDebug>

#include "panes/appearance.h"

QJsonObject currentConfig;
QHash<QString,QJsonValue> newConfig;
QFont controlCenterFont;

struct Entry {
    QString name;
    QIcon icon;
};


void readConfig() {
    // set globally readable variable for reading settings

    QString homeDirectory = getenv("HOME");
    QFile file;
    QString data;

    file.setFileName(homeDirectory + "/.config/plainDE/config.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();
    currentConfig = QJsonDocument::fromJson(data.toUtf8()).object();
}

void createUI() {
    QWidget* controlCenter = new QWidget;

    // Geometry
    short width = 400, height = 500;
    controlCenter->setGeometry(250, 250, width, height);

    // Set font
    controlCenterFont.setFamily(currentConfig["fontFamily"].toString());
    controlCenterFont.setPointSize(currentConfig["fontSize"].toInt());
    controlCenter->setFont(controlCenterFont);


    // UI
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(4, 4, 4, 4);
    controlCenter->setLayout(layout);

    QListWidget* entriesListWidget = new QListWidget;
    // Appearance - icons, font, theme, panel height
    // Sound - play startup sound, alsa, pulseaudio

    Entry entries[12] = {
                            {"Network", QIcon::fromTheme("preferences-system-network")},
                            {"Appearance", QIcon::fromTheme("preferences-desktop-theme")},
                            {"Sound", QIcon::fromTheme("audio-volume-high")},
                            {"Display", QIcon::fromTheme("video-display")},
                            {"Keyboard", QIcon::fromTheme("preferences-desktop-keyboard")},
                            {"Date & Time", QIcon::fromTheme("x-office-calendar")},
                            {"Language", QIcon::fromTheme("preferences-desktop-locale")},
                            {"Autostart", QIcon::fromTheme("system-run")},
                            {"Applets", QIcon::fromTheme("applications-utilities")},
                            {"Developer settings", QIcon::fromTheme("utilities-terminal")},
                            {"Default applications", QIcon::fromTheme("emblem-default")},
                            {"About", QIcon("/usr/share/plainDE/icon.png")}
                        };

    for (short i = 0; i < 12; ++i) {
        entriesListWidget->addItem(entries[i].name);
        entriesListWidget->item(i)->setIcon(entries[i].icon);
    }

    // Widgets
    AppearancePane appearancePane;
    QWidget* appearanceWidget = appearancePane.createUI(controlCenter);

    QProcess* process = new QProcess(controlCenter);

    // Make connections
    controlCenter->connect(entriesListWidget, &QListWidget::itemDoubleClicked, controlCenter,
               [entriesListWidget, process, controlCenter, appearanceWidget]() {
        if (entriesListWidget->selectedItems()[0]->text() == "Appearance") {
             appearanceWidget->show();
             controlCenter->hide();
        }
        else if (entriesListWidget->selectedItems()[0]->text() == "About") {
             process->start("plainAbout");
        }
    });

    controlCenter->layout()->addWidget(entriesListWidget);
    controlCenter->show();
}


settings::settings(QWidget *parent): QWidget(parent) {
    readConfig();
    QIcon::setThemeName(currentConfig["iconTheme"].toString());
    createUI();
}

settings::~settings() {

}

