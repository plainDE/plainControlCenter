#include "winlist.h"

QJsonObject winListAppletPaneConfig;

void WinListAppletPane::readConfig() {
    QString homeDirectory = getenv("HOME");
    QFile file;
    QString data;

    file.setFileName(homeDirectory + "/.config/plainDE/config.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();
    winListAppletPaneConfig = QJsonDocument::fromJson(data.toUtf8()).object();
}

void WinListAppletPane::setCurrentSettings(QSpinBox* sizeSpinBox) {
    int size = winListAppletPaneConfig["winListIconSize"].toInt();
    sizeSpinBox->setValue(size);
}

void WinListAppletPane::saveSettings(QSpinBox* sizeSpinBox) {
    winListAppletPaneConfig["winListIconSize"] = QJsonValue(sizeSpinBox->value());
    Pane::saveConfig(winListAppletPaneConfig);
}

QWidget* WinListAppletPane::createUI(Settings* controlCenter) {
    readConfig();

    // UI
    QWidget* winListAppletPane = new QWidget();
    winListAppletPane->setObjectName("winListAppletPane");
    winListAppletPane->setWindowTitle("plainControlCenter");
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(4, 4, 4, 4);
    winListAppletPane->setLayout(layout);

    // Theme
    QFile stylesheetReader("/usr/share/plainDE/styles/" + winListAppletPaneConfig["theme"].toString());
    stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream styleSheet(&stylesheetReader);
    winListAppletPane->setStyleSheet(styleSheet.readAll());
    stylesheetReader.close();

    QFont paneFont;
    paneFont.setFamily(winListAppletPaneConfig["fontFamily"].toString());
    paneFont.setPointSize(winListAppletPaneConfig["fontSize"].toInt());
    winListAppletPane->setFont(paneFont);

    short width = 400, height = 500;
    winListAppletPane->setGeometry(650, 250, width, height);

    QPushButton* backPushButton = new QPushButton("Close");
    backPushButton->setIcon(QIcon::fromTheme("go-previous"));
    layout->addWidget(backPushButton);

    QLabel* sizeLabel = new QLabel("Choose icon size");
    layout->addWidget(sizeLabel);

    QSpinBox* sizeSpinBox = new QSpinBox();
    layout->addWidget(sizeSpinBox);

    layout->addItem(new QSpacerItem(0,
                                    0,
                                    QSizePolicy::Ignored,
                                    QSizePolicy::Expanding));

    QPushButton* revertPushButton = new QPushButton("Revert");
    layout->addWidget(revertPushButton);

    QPushButton* savePushButton = new QPushButton("Save");
    layout->addWidget(savePushButton);

    setCurrentSettings(sizeSpinBox);

    // Make connections
    winListAppletPane->connect(revertPushButton, &QPushButton::clicked, winListAppletPane,
                                 [this, sizeSpinBox]() {
                                     setCurrentSettings(sizeSpinBox);
                                 });

    winListAppletPane->connect(savePushButton, &QPushButton::clicked, winListAppletPane,
                                 [this, sizeSpinBox]() {
                                     saveSettings(sizeSpinBox);
                                 });
    winListAppletPane->connect(backPushButton, &QPushButton::clicked, winListAppletPane,
                                 [this, winListAppletPane, controlCenter]() {
                                     controlCenter->mWinListWidgetVisible = false;
                                     delete winListAppletPane;
                                     delete this;
                                 });

    return winListAppletPane;
}
