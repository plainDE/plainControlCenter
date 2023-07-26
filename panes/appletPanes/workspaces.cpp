#include "workspaces.h"

QJsonObject workspacesAppletPaneConfig;

void WorkspacesAppletPane::readConfig() {
    QString homeDirectory = getenv("HOME");
    QFile file;
    QString data;

    file.setFileName(homeDirectory + "/.config/plainDE/config.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();
    workspacesAppletPaneConfig = QJsonDocument::fromJson(data.toUtf8()).object();
}

void WorkspacesAppletPane::setCurrentSettings(QCheckBox* showDesktopNamesCheckBox) {
    if (workspacesAppletPaneConfig["showDesktopNames"].toBool()) {
        showDesktopNamesCheckBox->setChecked(true);
    }
    else {
        showDesktopNamesCheckBox->setChecked(false);
    }
}

void WorkspacesAppletPane::saveSettings(QCheckBox* showDesktopNamesCheckBox) {
    if (showDesktopNamesCheckBox->isChecked()) {
        workspacesAppletPaneConfig["showDesktopNames"] = QJsonValue(true);
    }
    else {
        workspacesAppletPaneConfig["showDesktopNames"] = QJsonValue(false);
    }
    Pane::saveConfig(workspacesAppletPaneConfig);
}

QWidget* WorkspacesAppletPane::createUI(Settings* controlCenter) {
    readConfig();

    // UI
    QWidget* workspacesAppletPane = new QWidget();
    workspacesAppletPane->setObjectName("workspacesAppletPane");
    workspacesAppletPane->setWindowTitle("plainControlCenter");
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(4, 4, 4, 4);
    workspacesAppletPane->setLayout(layout);

    // Theme
    QFile stylesheetReader("/usr/share/plainDE/styles/" + workspacesAppletPaneConfig["theme"].toString());
    stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream styleSheet(&stylesheetReader);
    workspacesAppletPane->setStyleSheet(styleSheet.readAll());
    stylesheetReader.close();

    QFont paneFont;
    paneFont.setFamily(workspacesAppletPaneConfig["fontFamily"].toString());
    paneFont.setPointSize(workspacesAppletPaneConfig["fontSize"].toInt());
    workspacesAppletPane->setFont(paneFont);

    short width = 400, height = 500;
    workspacesAppletPane->setGeometry(650, 250, width, height);

    QPushButton* backPushButton = new QPushButton("Close");
    backPushButton->setIcon(QIcon::fromTheme("go-previous"));
    layout->addWidget(backPushButton);

    QCheckBox* showDesktopNamesCheckBox = new QCheckBox("Show desktop names");
    layout->addWidget(showDesktopNamesCheckBox);

    layout->addItem(new QSpacerItem(0,
                                    0,
                                    QSizePolicy::Ignored,
                                    QSizePolicy::Expanding));

    QPushButton* revertPushButton = new QPushButton("Revert");
    layout->addWidget(revertPushButton);

    QPushButton* savePushButton = new QPushButton("Save");
    layout->addWidget(savePushButton);

    setCurrentSettings(showDesktopNamesCheckBox);

    // Make connections
    workspacesAppletPane->connect(revertPushButton, &QPushButton::clicked, workspacesAppletPane,
                              [this, showDesktopNamesCheckBox]() {
                                  setCurrentSettings(showDesktopNamesCheckBox);
                              });

    workspacesAppletPane->connect(savePushButton, &QPushButton::clicked, workspacesAppletPane,
                              [this, showDesktopNamesCheckBox]() {
                                  saveSettings(showDesktopNamesCheckBox);
                              });
    workspacesAppletPane->connect(backPushButton, &QPushButton::clicked, workspacesAppletPane,
                              [this, workspacesAppletPane, controlCenter]() {
                                  controlCenter->mWorkspacesWidgetVisible = false;
                                  delete workspacesAppletPane;
                                  delete this;
                              });

    return workspacesAppletPane;
}
