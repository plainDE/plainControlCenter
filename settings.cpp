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
#include "panes/panel.h"
#include "panes/applets.h"

QJsonObject config;
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
    config = QJsonDocument::fromJson(data.toUtf8()).object();
}

void createUI() {
    QWidget* controlCenter = new QWidget;
    controlCenter->setObjectName("controlCenter");

    // Geometry
    short width = 400, height = 500;
    controlCenter->setGeometry(250, 250, width, height);

    // Style
    QFile stylesheetReader("/usr/share/plainDE/styles/" + config["theme"].toString());
    stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream styleSheet(&stylesheetReader);
    controlCenter->setStyleSheet(styleSheet.readAll());

    // Set font
    controlCenterFont.setFamily(config["fontFamily"].toString());
    controlCenterFont.setPointSize(config["fontSize"].toInt());
    controlCenter->setFont(controlCenterFont);


    // UI
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(4, 4, 4, 4);
    controlCenter->setLayout(layout);

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
                            //{"Keyboard", QIcon::fromTheme("preferences-desktop-keyboard")},
                            //{"Date & Time", QIcon::fromTheme("x-office-calendar")},
                            //{"Language", QIcon::fromTheme("preferences-desktop-locale")},
                            {"Autostart", QIcon::fromTheme("applications-utilities")},
                            {"Panel", QIcon::fromTheme("panel")},
                            {"Applets", QIcon::fromTheme("panel-applets")},  // cs-extensions
                            //{"Developer settings", QIcon::fromTheme("utilities-terminal")},
                            //{"Default applications", QIcon::fromTheme("emblem-default")},
                            {"About", QIcon("/usr/share/plainDE/menuIcon.png")}
                        };

    for (short i = 0; i < 5; ++i) {
        entriesListWidget->addItem(entries[i].name);
        entriesListWidget->item(i)->setIcon(entries[i].icon);
    }

    // Widgets
    AppearancePane* appearancePane = new AppearancePane;
    AutostartPane* autostartPane = new AutostartPane;
    PanelPane* panelPane = new PanelPane;
    AppletsPane* appletsPane = new AppletsPane;

    QWidget* appearanceWidget = appearancePane->createUI(controlCenter);
    QWidget* autostartWidget = autostartPane->createUI(controlCenter);
    QWidget* panelWidget = panelPane->createUI(controlCenter);
    QWidget* appletsWidget = appletsPane->createUI(controlCenter);


    QProcess* process = new QProcess(controlCenter);

    // Make connections
    controlCenter->connect(entriesListWidget, &QListWidget::itemDoubleClicked, controlCenter,
               [entriesListWidget, process, controlCenter,
                appearanceWidget, appearancePane,
                autostartWidget, autostartPane,
                panelWidget, panelPane,
                appletsWidget, appletsPane]()mutable {
        if (entriesListWidget->selectedItems()[0]->text() == "Appearance") {
            appearanceWidget = appearancePane->createUI(controlCenter);
            appearanceWidget->show();
            controlCenter->hide();
        }
        else if (entriesListWidget->selectedItems()[0]->text() == "Autostart") {
            autostartWidget = autostartPane->createUI(controlCenter);
            autostartWidget->show();
            controlCenter->hide();
        }
        else if (entriesListWidget->selectedItems()[0]->text() == "Panel") {
            panelWidget = panelPane->createUI(controlCenter);
            panelWidget->show();
            controlCenter->hide();
        }
        else if (entriesListWidget->selectedItems()[0]->text() == "Applets") {
            appletsWidget = appletsPane->createUI(controlCenter);
            appletsWidget->show();
            controlCenter->hide();
        }
        else if (entriesListWidget->selectedItems()[0]->text() == "About") {
            process->start("plainAbout --plainControlCenter");
        }
    });

    controlCenter->layout()->addWidget(entriesListWidget);
    controlCenter->show();
}


settings::settings(QWidget *parent): QWidget(parent) {
    readConfig();
    QIcon::setThemeName(config["iconTheme"].toString());
    createUI();
}

settings::~settings() {

}

