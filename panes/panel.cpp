#include "panel.h"

QJsonObject panelPaneConfig;


void PanelPane::readConfig() {
    QString homeDirectory = getenv("HOME");
    QFile file;
    QString data;

    file.setFileName(homeDirectory + "/.config/plainDE/config.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();
    panelPaneConfig = QJsonDocument::fromJson(data.toUtf8()).object();
}

void PanelPane::setCurrentSettings(QSpinBox* panelHeightSpinBox,
                                   QRadioButton* panelTopLocationRadioButton,
                                   QRadioButton* panelBottomLocationRadioButton,
                                   QCheckBox* panelAnimationCheckBox) {
    panelHeightSpinBox->setValue(panelPaneConfig["panelHeight"].toInt());
    if (panelPaneConfig["panelLocation"].toString() == "top") {
        panelTopLocationRadioButton->toggle();
    }
    else {
        panelBottomLocationRadioButton->toggle();
    }

    if (panelAnimationCheckBox->isChecked()) {
        panelAnimationCheckBox->toggle();
    }
    if (panelPaneConfig["enableAnimation"].toBool()) {
        panelAnimationCheckBox->toggle();
    }
}

void PanelPane::saveSettings(QSpinBox* panelHeightSpinBox,
                             QRadioButton* panelTopLocationRadioButton,
                             QCheckBox* panelAnimationCheckBox) {
    panelPaneConfig["panelHeight"] = QJsonValue(panelHeightSpinBox->value());
    if (panelTopLocationRadioButton->isChecked()) {
        panelPaneConfig["panelLocation"] = QJsonValue("top");
    }
    else {
        panelPaneConfig["panelLocation"] = QJsonValue("bottom");
    }

    if (panelAnimationCheckBox->isChecked()) {
        panelPaneConfig["enableAnimation"] = QJsonValue(true);
    }
    else {
        panelPaneConfig["enableAnimation"] = QJsonValue(false);
    }

    Pane::saveConfig(panelPaneConfig);
}

QWidget* PanelPane::createUI(QWidget* controlCenter) {
    readConfig();

    // UI
    QWidget* panelPane = new QWidget;
    panelPane->setObjectName("panelPane");
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(4, 4, 4, 4);
    panelPane->setLayout(layout);

    // Style
    if (panelPaneConfig["theme"] == "light") {
        QFile stylesheetReader(":/styles/general-light.qss");
        stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream styleSheet(&stylesheetReader);
        panelPane->setStyleSheet(styleSheet.readAll());
    }
    else {
        QFile stylesheetReader(":/styles/general-dark.qss");
        stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream styleSheet(&stylesheetReader);
        panelPane->setStyleSheet(styleSheet.readAll());
    }

    QFont paneFont;
    paneFont.setFamily(panelPaneConfig["fontFamily"].toString());
    paneFont.setPointSize(panelPaneConfig["fontSize"].toInt());
    panelPane->setFont(paneFont);

    short width = 400, height = 500;
    panelPane->setGeometry(250, 250, width, height);

    QPushButton* backPushButton = new QPushButton("Back");
    backPushButton->setIcon(QIcon::fromTheme("go-previous"));
    panelPane->layout()->addWidget(backPushButton);

    QLabel* panelHeightLabel = new QLabel("Panel height");
    panelPane->layout()->addWidget(panelHeightLabel);

    QSpinBox* panelHeightSpinBox = new QSpinBox;
    panelPane->layout()->addWidget(panelHeightSpinBox);

    QLabel* panelLocationLabel = new QLabel("Panel location");
    panelPane->layout()->addWidget(panelLocationLabel);

    QRadioButton* panelTopLocationRadioButton = new QRadioButton("Top");
    panelPane->layout()->addWidget(panelTopLocationRadioButton);

    QRadioButton* panelBottomLocationRadioButton = new QRadioButton("Bottom");
    panelPane->layout()->addWidget(panelBottomLocationRadioButton);

    QLabel* panelAnimationLabel = new QLabel("Panel animation");
    QCheckBox* panelAnimationCheckBox = new QCheckBox("Enable");
    panelPane->layout()->addWidget(panelAnimationLabel);
    panelPane->layout()->addWidget(panelAnimationCheckBox);

    panelPane->layout()->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));

    QPushButton* revertPushButton = new QPushButton("Revert");
    panelPane->layout()->addWidget(revertPushButton);

    QPushButton* savePushButton = new QPushButton("Save");
    panelPane->layout()->addWidget(savePushButton);

    setCurrentSettings(panelHeightSpinBox,
                       panelTopLocationRadioButton,
                       panelBottomLocationRadioButton,
                       panelAnimationCheckBox);


    // Make connections
    panelPane->connect(backPushButton, &QPushButton::clicked, panelPane,
                       [panelPane, controlCenter]() {
        controlCenter->show();
        panelPane->hide();
    });

    panelPane->connect(revertPushButton, &QPushButton::clicked, panelPane,
                       [this, panelHeightSpinBox, panelTopLocationRadioButton,
                       panelBottomLocationRadioButton, panelAnimationCheckBox]() {
        setCurrentSettings(panelHeightSpinBox,
                           panelTopLocationRadioButton,
                           panelBottomLocationRadioButton,
                           panelAnimationCheckBox);
    });

    panelPane->connect(savePushButton, &QPushButton::clicked, panelPane,
                       [this, panelHeightSpinBox, panelTopLocationRadioButton,
                        panelAnimationCheckBox]() {
        saveSettings(panelHeightSpinBox,
                     panelTopLocationRadioButton,
                     panelAnimationCheckBox);
    });

    return panelPane;
}
