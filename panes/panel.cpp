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
                                   QCheckBox* panelAnimationCheckBox,
                                   QCheckBox* expandPanelCheckBox,
                                   QSpinBox* panelOffsetSpinBox,
                                   QDoubleSpinBox* panelOpacitySpinBox) {
    panelHeightSpinBox->setValue(panelPaneConfig["panelHeight"].toInt());
    if (panelPaneConfig["panelLocation"].toString() == "top") {
        panelTopLocationRadioButton->toggle();
    }
    else {
        panelBottomLocationRadioButton->toggle();
    }

    panelAnimationCheckBox->setChecked(
                panelPaneConfig["enableAnimation"].toBool());

    if (panelPaneConfig["expandPanel"].toBool()) {
        expandPanelCheckBox->setChecked(true);
        panelOffsetSpinBox->setEnabled(false);
    }
    else {expandPanelCheckBox->setChecked(false);
        panelOffsetSpinBox->setEnabled(true);
        panelOffsetSpinBox->setValue(panelPaneConfig["xOffset"].toInt());
    }

    panelOpacitySpinBox->setValue(panelPaneConfig["panelOpacity"].toDouble());
}

void PanelPane::saveSettings(QSpinBox* panelHeightSpinBox,
                             QRadioButton* panelTopLocationRadioButton,
                             QCheckBox* panelAnimationCheckBox,
                             QCheckBox* expandPanelCheckBox,
                             QSpinBox* panelOffsetSpinBox,
                             QDoubleSpinBox* panelOpacitySpinBox) {
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

    if (expandPanelCheckBox->isChecked()) {
        panelPaneConfig["expandPanel"] = QJsonValue(true);
    }
    else {
        panelPaneConfig["expandPanel"] = QJsonValue(false);
    }

    panelPaneConfig["xOffset"] = QJsonValue(panelOffsetSpinBox->value());

    panelPaneConfig["panelOpacity"] = QJsonValue(panelOpacitySpinBox->value());

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

    // Theme
    QFile stylesheetReader("/usr/share/plainDE/styles/" + panelPaneConfig["theme"].toString());
    stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream styleSheet(&stylesheetReader);
    panelPane->setStyleSheet(styleSheet.readAll());

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

    QLabel* panelExpansionLabel = new QLabel("Panel expansion");
    panelPane->layout()->addWidget(panelExpansionLabel);

    QCheckBox* expandPanelCheckBox = new QCheckBox("Expand panel");
    panelPane->layout()->addWidget(expandPanelCheckBox);

    QLabel* panelOffsetLabel = new QLabel("Panel X offset");
    panelPane->layout()->addWidget(panelOffsetLabel);

    QSpinBox* panelOffsetSpinBox = new QSpinBox;
    panelOffsetSpinBox->setMinimum(0);
    panelOffsetSpinBox->setMaximum(
                QGuiApplication::primaryScreen()->size().width());
    panelPane->layout()->addWidget(panelOffsetSpinBox);

    QLabel* panelOpacityLabel = new QLabel("Panel opacity");
    panelPane->layout()->addWidget(panelOpacityLabel);

    QDoubleSpinBox* panelOpacitySpinBox = new QDoubleSpinBox;
    panelOpacitySpinBox->setMinimum(0.1);
    panelOpacitySpinBox->setMaximum(1.0);
    panelPane->layout()->addWidget(panelOpacitySpinBox);

    panelPane->layout()->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));

    QPushButton* revertPushButton = new QPushButton("Revert");
    panelPane->layout()->addWidget(revertPushButton);

    QPushButton* savePushButton = new QPushButton("Save");
    panelPane->layout()->addWidget(savePushButton);

    setCurrentSettings(panelHeightSpinBox,
                       panelTopLocationRadioButton,
                       panelBottomLocationRadioButton,
                       panelAnimationCheckBox,
                       expandPanelCheckBox,
                       panelOffsetSpinBox,
                       panelOpacitySpinBox);


    // Make connections
    panelPane->connect(backPushButton, &QPushButton::clicked, panelPane,
                       [panelPane, controlCenter]() {
        controlCenter->show();
        panelPane->hide();
        delete panelPane;
    });

    panelPane->connect(expandPanelCheckBox, &QCheckBox::toggled, panelPane,
                       [expandPanelCheckBox, panelOffsetSpinBox]() {
        if (expandPanelCheckBox->isChecked()) {
            panelOffsetSpinBox->setEnabled(false);
        }
        else {
            panelOffsetSpinBox->setEnabled(true);
        }
    });

    panelPane->connect(revertPushButton, &QPushButton::clicked, panelPane,
                       [this, panelHeightSpinBox, panelTopLocationRadioButton,
                       panelBottomLocationRadioButton, panelAnimationCheckBox,
                       expandPanelCheckBox, panelOffsetSpinBox, panelOpacitySpinBox]() {
        setCurrentSettings(panelHeightSpinBox,
                           panelTopLocationRadioButton,
                           panelBottomLocationRadioButton,
                           panelAnimationCheckBox,
                           expandPanelCheckBox,
                           panelOffsetSpinBox,
                           panelOpacitySpinBox);
    });

    panelPane->connect(savePushButton, &QPushButton::clicked, panelPane,
                       [this, panelHeightSpinBox, panelTopLocationRadioButton,
                        panelAnimationCheckBox, expandPanelCheckBox,
                        panelOffsetSpinBox, panelOpacitySpinBox]() {
        saveSettings(panelHeightSpinBox,
                     panelTopLocationRadioButton,
                     panelAnimationCheckBox,
                     expandPanelCheckBox,
                     panelOffsetSpinBox,
                     panelOpacitySpinBox);
    });

    return panelPane;
}
