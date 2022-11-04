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
#include "panes/autostart.h"
#include "panes/panels.h"
#include "panes/keyboard/keyboard.h"

QJsonObject config;
QFont controlCenterFont;

struct Entry {
    QString name;
    QIcon icon;
};

void Settings::readConfig() {
    // set globally readable variable for reading settings

    QString homeDirectory = getenv("HOME");
    QFile file;
    QString data;

    file.setFileName(homeDirectory + "/.config/plainDE/config.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();
    config = QJsonDocument::fromJson(data.toUtf8()).object();
}

void Settings::updateStyle() {
    readConfig();

    // Theme
    QFile stylesheetReader("/usr/share/plainDE/styles/" + config["theme"].toString());
    stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream styleSheet(&stylesheetReader);
    controlCenterWidget->setStyleSheet(styleSheet.readAll());
    stylesheetReader.close();

    // Font
    controlCenterFont.setFamily(config["fontFamily"].toString());
    controlCenterFont.setPointSize(config["fontSize"].toInt());
    controlCenterWidget->setFont(controlCenterFont);
}

void Settings::createUI() {
    controlCenterWidget = new QWidget;
    controlCenterWidget->setObjectName("controlCenter");
    controlCenterWidget->setWindowTitle("plainControlCenter");

    // Geometry
    short width = 400, height = 500;
    controlCenterWidget->setGeometry(250, 250, width, height);

    // Appearance
    updateStyle();

    // UI
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(4, 4, 4, 4);
    controlCenterWidget->setLayout(layout);

    QListWidget* entriesListWidget = new QListWidget;
    entriesListWidget->setStyleSheet("QListView::item:selected { background-color: " + \
                                     config["accent"].toString() + \
                                     "; color: #ffffff };");

    // Sound - play startup sound, alsa, pulseaudio

    Entry entries[12] = {
                            //{"Network", QIcon::fromTheme("preferences-system-network")},
                            {"Appearance", QIcon::fromTheme("preferences-desktop-theme")},
                            //{"Sound", QIcon::fromTheme("audio-volume-high")},
                            //{"Display", QIcon::fromTheme("video-display")},
                            {"Keyboard", QIcon::fromTheme("preferences-desktop-keyboard")},
                            //{"Date & Time", QIcon::fromTheme("x-office-calendar")},
                            //{"Language", QIcon::fromTheme("preferences-desktop-locale")},
                            {"Autostart", QIcon::fromTheme("applications-utilities")},
                            {"Panels", QIcon::fromTheme("panel")},
                            //{"Developer settings", QIcon::fromTheme("utilities-terminal")},
                            //{"Default applications", QIcon::fromTheme("emblem-default")},
                            {"About", QIcon("/usr/share/plainDE/menuIcon.png")}
                        };

    for (short i = 0; i < 6; ++i) {
        entriesListWidget->addItem(entries[i].name);
        entriesListWidget->item(i)->setIcon(entries[i].icon);
    }

    QProcess* process = new QProcess(controlCenterWidget);

    // Make connections
    controlCenterWidget->connect(entriesListWidget, &QListWidget::itemDoubleClicked, controlCenterWidget,
               [this, entriesListWidget, process]() {
        if (entriesListWidget->selectedItems()[0]->text() == "Appearance") {
            AppearancePane* appearancePane = new AppearancePane;
            QWidget* appearanceWidget = appearancePane->createUI(this);
            appearanceWidget->show();
            controlCenterWidget->hide();
        }
        else if (entriesListWidget->selectedItems()[0]->text() == "Autostart") {
            AutostartPane* autostartPane = new AutostartPane;
            QWidget* autostartWidget = autostartPane->createUI(this);
            autostartWidget->show();
            controlCenterWidget->hide();
        }
        else if (entriesListWidget->selectedItems()[0]->text() == "Panels") {
            PanelsPane* panelsPane = new PanelsPane(nullptr, this);
            panelsPane->show();
            controlCenterWidget->hide();
        }
        else if (entriesListWidget->selectedItems()[0]->text() == "Keyboard") {
            KeyboardPane* keyboardPane = new KeyboardPane;
            QWidget* keyboardWidget = keyboardPane->createUI(this);
            keyboardWidget->show();
            controlCenterWidget->hide();
        }
        else if (entriesListWidget->selectedItems()[0]->text() == "About") {
            process->start("plainAbout --plainControlCenter");
        }
    });

    controlCenterWidget->layout()->addWidget(entriesListWidget);
    controlCenterWidget->show();
}


Settings::Settings() {
    readConfig();
    QIcon::setThemeName(config["iconTheme"].toString());
    createUI();
}

Settings::~Settings() {

}

