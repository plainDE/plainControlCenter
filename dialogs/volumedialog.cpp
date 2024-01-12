#include "volumedialog.h"

void VolumeDialog::setPaneContents() {
    mContentsWidget = new QWidget();
    QVBoxLayout* contentsLayout = new QVBoxLayout(mContentsWidget);
    mContentsWidget->setObjectName("innerPane");

    // Adding widgets
    QLabel* adjustMethodLabel = new QLabel(tr("Volume Adjust Method"));
    adjustMethodLabel->setFont(mTitleFont);
    contentsLayout->addWidget(adjustMethodLabel);

    QVBoxLayout* adjustMethodLayout = new QVBoxLayout();

    QRadioButton* alsaRadioButton = new QRadioButton("ALSA");
    alsaRadioButton->setFont(mFont);
    adjustMethodLayout->addWidget(alsaRadioButton);

    QRadioButton* pulseaudioRadioButton = new QRadioButton("PulseAudio");
    pulseaudioRadioButton->setFont(mFont);
    adjustMethodLayout->addWidget(pulseaudioRadioButton);

    contentsLayout->addLayout(adjustMethodLayout);

    QCheckBox* overamplificationCheckBox = new QCheckBox(tr("Enable overamplification (PulseAudio only)"));
    overamplificationCheckBox->setFont(mTitleFont);
    overamplificationCheckBox->setEnabled(false);
    contentsLayout->addWidget(overamplificationCheckBox);

    QLabel* warningLabel = new QLabel(tr("Be careful! High volume may damage\n"
                                         "your hardware and hearing."));
    warningLabel->setFont(mFont);
    warningLabel->setStyleSheet("color: #DD0000");
    warningLabel->setVisible(false);
    contentsLayout->addWidget(warningLabel);

    QLabel* defaultVolumeLabel = new QLabel(tr("Default Volume"));
    defaultVolumeLabel->setFont(mTitleFont);
    contentsLayout->addWidget(defaultVolumeLabel);

    bool enableOveramplification = getConfigValue("enableOveramplification").toBool();

    QSpinBox* defaultVolumeSpinBox = new QSpinBox();
    defaultVolumeSpinBox->setFont(mFont);
    defaultVolumeSpinBox->setMinimum(0);
    if (enableOveramplification) {
        defaultVolumeSpinBox->setMaximum(150);
    }
    else {
        defaultVolumeSpinBox->setMaximum(100);
    }
    contentsLayout->addWidget(defaultVolumeSpinBox);

    contentsLayout->addSpacerItem(new QSpacerItem(0, 0,
                                                  QSizePolicy::Ignored,
                                                  QSizePolicy::MinimumExpanding));

    QHBoxLayout* buttonsLayout = new QHBoxLayout();

    buttonsLayout->addSpacerItem(new QSpacerItem(0, 0,
                                                 QSizePolicy::MinimumExpanding,
                                                 QSizePolicy::Ignored));

    QPushButton* cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setFont(mFont);
    buttonsLayout->addWidget(cancelButton);

    QPushButton* okButton = new QPushButton(tr("OK"));
    okButton->setFont(mFont);
    buttonsLayout->addWidget(okButton);

    contentsLayout->addLayout(buttonsLayout);


    // Setting current settings
    if (!getConfigValue("volumeAdjustMethod").toString().compare("ALSA")) {
        alsaRadioButton->setChecked(true);
        overamplificationCheckBox->setEnabled(false);
    }
    else {  // PulseAudio
        pulseaudioRadioButton->setChecked(true);
        overamplificationCheckBox->setEnabled(true);
    }

    overamplificationCheckBox->setChecked(enableOveramplification);
    if (enableOveramplification) {
        warningLabel->setVisible(true);
    }

    defaultVolumeSpinBox->setValue(getConfigValue("defaultVolume").toInt());


    // Making connections
    connect(pulseaudioRadioButton, &QRadioButton::clicked, this, [this, overamplificationCheckBox] {
        overamplificationCheckBox->setEnabled(true);
        overamplificationCheckBox->setChecked(getConfigValue("enableOveramplification").toBool());
    });

    connect(alsaRadioButton, &QRadioButton::clicked, this, [overamplificationCheckBox]() {
        overamplificationCheckBox->setEnabled(false);
        overamplificationCheckBox->setChecked(false);
    });

    connect(overamplificationCheckBox, &QCheckBox::stateChanged, this, [overamplificationCheckBox,
                                                                        warningLabel,
                                                                        defaultVolumeSpinBox]() {
        if (overamplificationCheckBox->isChecked()) {
            warningLabel->setVisible(true);
            defaultVolumeSpinBox->setMaximum(150);
        }
        else {
            warningLabel->setVisible(false);
            defaultVolumeSpinBox->setMaximum(100);
        }
    });

    connect(cancelButton, &QPushButton::clicked, this, [this]() {
        this->hide();
        delete this;
    });

    connect(okButton, &QPushButton::clicked, this, [this, alsaRadioButton,
                                                    overamplificationCheckBox,
                                                    defaultVolumeSpinBox]() {
        prepareToSave(alsaRadioButton,
                      overamplificationCheckBox,
                      defaultVolumeSpinBox);
        saveConfig();
        this->hide();
        delete this;
    });

    finalizeUI();
    setWindowGeometry();
    setTransparency(this);
}

void VolumeDialog::prepareToSave(QRadioButton* alsaRadioButton,
                                 QCheckBox* overamplificationCheckBox,
                                 QSpinBox* defaultVolumeSpinBox) {
    if (alsaRadioButton->isChecked()) {
        setEntry("volumeAdjustMethod", "ALSA");
    }
    else {  // PulseAudio
        setEntry("volumeAdjustMethod", "PulseAudio");
    }
    setEntry("enableOveramplification", overamplificationCheckBox->isChecked());
    setEntry("defaultVolume", defaultVolumeSpinBox->value());
}

VolumeDialog::VolumeDialog(QJsonObject* cfgObj) : Dialog(cfgObj,
                                                         tr("Volume applet settings"),
                                                         "cs-sound") {

}
