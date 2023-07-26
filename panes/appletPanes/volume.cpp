#include "volume.h"

QJsonObject volumeAppletPaneConfig;

void VolumeAppletPane::readConfig() {
    QString homeDirectory = getenv("HOME");
    QFile file;
    QString data;

    file.setFileName(homeDirectory + "/.config/plainDE/config.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();
    volumeAppletPaneConfig = QJsonDocument::fromJson(data.toUtf8()).object();
}

void VolumeAppletPane::setCurrentSettings(QRadioButton* alsaRadioButton,
                                          QRadioButton* pulseaudioRadioButton) {
    QString method = volumeAppletPaneConfig["volumeAdjustMethod"].toString();
    if (!method.compare("ALSA")) {
        alsaRadioButton->setChecked(true);
    }
    else if (!method.compare("PulseAudio")) {
        pulseaudioRadioButton->setChecked(true);
    }
}

void VolumeAppletPane::saveSettings(QRadioButton* alsaRadioButton,
                                    QRadioButton* pulseaudioRadioButton) {
    if (alsaRadioButton->isChecked()) {
        volumeAppletPaneConfig["volumeAdjustMethod"] = QJsonValue("ALSA");
        Pane::saveConfig(volumeAppletPaneConfig);
    }
    else if (pulseaudioRadioButton->isChecked()) {
        volumeAppletPaneConfig["volumeAdjustMethod"] = QJsonValue("PulseAudio");
        Pane::saveConfig(volumeAppletPaneConfig);
    }
    else {
        QMessageBox msg;
        msg.setWindowTitle("Error");
        msg.setText("Please select either ALSA or PulseAudio volume adjust method.");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setIcon(QMessageBox::Critical);
        msg.exec();
    }
}

QWidget* VolumeAppletPane::createUI(Settings* controlCenter) {
    readConfig();

    // UI
    QWidget* volumeAppletPane = new QWidget();
    volumeAppletPane->setObjectName("volumeAppletPane");
    volumeAppletPane->setWindowTitle("plainControlCenter");
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(4, 4, 4, 4);
    volumeAppletPane->setLayout(layout);

    // Theme
    QFile stylesheetReader("/usr/share/plainDE/styles/" + volumeAppletPaneConfig["theme"].toString());
    stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream styleSheet(&stylesheetReader);
    volumeAppletPane->setStyleSheet(styleSheet.readAll());
    stylesheetReader.close();

    QFont paneFont;
    paneFont.setFamily(volumeAppletPaneConfig["fontFamily"].toString());
    paneFont.setPointSize(volumeAppletPaneConfig["fontSize"].toInt());
    volumeAppletPane->setFont(paneFont);

    short width = 400, height = 500;
    volumeAppletPane->setGeometry(650, 250, width, height);

    QPushButton* backPushButton = new QPushButton("Close");
    backPushButton->setIcon(QIcon::fromTheme("go-previous"));
    layout->addWidget(backPushButton);

    QLabel* adjustMethodLabel = new QLabel("Volume adjust method");
    layout->addWidget(adjustMethodLabel);

    QRadioButton* alsaRadioButton = new QRadioButton("ALSA");
    layout->addWidget(alsaRadioButton);

    QRadioButton* pulseaudioRadioButton = new QRadioButton("PulseAudio");
    layout->addWidget(pulseaudioRadioButton);

    layout->addItem(new QSpacerItem(0,
                                    0,
                                    QSizePolicy::Ignored,
                                    QSizePolicy::Expanding));

    QPushButton* revertPushButton = new QPushButton("Revert");
    layout->addWidget(revertPushButton);

    QPushButton* savePushButton = new QPushButton("Save");
    layout->addWidget(savePushButton);

    setCurrentSettings(alsaRadioButton, pulseaudioRadioButton);

    // Make connections
    volumeAppletPane->connect(revertPushButton, &QPushButton::clicked, volumeAppletPane,
                               [this, alsaRadioButton, pulseaudioRadioButton]() {
                                   setCurrentSettings(alsaRadioButton,
                                                      pulseaudioRadioButton);
                               });

    volumeAppletPane->connect(savePushButton, &QPushButton::clicked, volumeAppletPane,
                               [this, alsaRadioButton, pulseaudioRadioButton]() {
                                   saveSettings(alsaRadioButton,
                                                pulseaudioRadioButton);
                               });
    volumeAppletPane->connect(backPushButton, &QPushButton::clicked, volumeAppletPane,
                               [this, volumeAppletPane, controlCenter]() {
                                   controlCenter->mVolumeWidgetVisible = false;
                                   delete volumeAppletPane;
                                   delete this;
                               });

    return volumeAppletPane;
}
