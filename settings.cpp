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
    this->setStyleSheet(styleSheet.readAll());
    stylesheetReader.close();

    // Font
    controlCenterFont.setFamily(config["fontFamily"].toString());
    controlCenterFont.setPointSize(config["fontSize"].toInt());
    this->setFont(controlCenterFont);
}

void Settings::createUI() {
    this->setObjectName("controlCenter");
    this->setWindowTitle("plainControlCenter");

    // Geometry
    short width = 600, height = 500;
    this->setGeometry(250, 250, width, height);

    // Appearance
    updateStyle();

    // UI
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(4, 4, 4, 4);
    mainLayout->setSpacing(3);

    QListWidget* entriesListWidget = new QListWidget;
    entriesListWidget->setStyleSheet("QListView::item:selected { background-color: " + \
                                     config["accent"].toString() + \
                                     "; color: #ffffff };");

    Entry entries[12] = {
                            {"Appearance", QIcon::fromTheme("preferences-desktop-theme")},
                            {"Keyboard", QIcon::fromTheme("preferences-desktop-keyboard")},
                            {"Autostart", QIcon::fromTheme("applications-utilities")},
                            {"Panels", QIcon::fromTheme("panel")},
                            {"About", QIcon("/usr/share/plainDE/menuIcon.png")}
                        };

    for (short i = 0; i < 5; ++i) {
        entriesListWidget->addItem(entries[i].name);
        entriesListWidget->item(i)->setIcon(entries[i].icon);
    }

    QProcess* process = new QProcess(this);

    mainLayout->addWidget(entriesListWidget);
    this->show();

    // Make connections
    this->connect(entriesListWidget, &QListWidget::itemClicked, this,
               [this, mainLayout, entriesListWidget, process]() {
        if (entriesListWidget->selectedItems()[0]->text() == "Appearance") {
            if (!mAppearanceWidgetVisible) {
                mAppearanceWidgetVisible = true;
                AppearancePane* appearancePane = new AppearancePane;
                QWidget* appearanceWidget = appearancePane->createUI(this);
                mainLayout->addWidget(appearanceWidget);
            }
        }
        else if (entriesListWidget->selectedItems()[0]->text() == "Autostart") {
            if (!mAutostartWidgetVisible) {
                mAutostartWidgetVisible = true;
                AutostartPane* autostartPane = new AutostartPane;
                QWidget* autostartWidget = autostartPane->createUI(this);
                mainLayout->addWidget(autostartWidget);
            }
        }
        else if (entriesListWidget->selectedItems()[0]->text() == "Panels") {
            if (!mPanelsWidgetVisible) {
                mPanelsWidgetVisible = true;
                PanelsPane* panelsPane = new PanelsPane(nullptr, this);
                mainLayout->addWidget(panelsPane);
            }
        }
        else if (entriesListWidget->selectedItems()[0]->text() == "Keyboard") {
            if (!mKeyboardWidgetVisible) {
                mKeyboardWidgetVisible = true;
                KeyboardPane* keyboardPane = new KeyboardPane;
                QWidget* keyboardWidget = keyboardPane->createUI(this);
                mainLayout->addWidget(keyboardWidget);
            }
        }
        else if (entriesListWidget->selectedItems()[0]->text() == "About") {
            process->start("plainAbout --plainControlCenter");
        }
    });
}

Settings::Settings(QWidget *parent) : QWidget(parent) {
    readConfig();
    QIcon::setThemeName(config["iconTheme"].toString());
    createUI();
}

Settings::~Settings() {

}
