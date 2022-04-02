#include "appearance.h"
#include "../settings.h"
#include "../pane.h"

QJsonObject config;


void AppearancePane::readConfig() {
    QString homeDirectory = getenv("HOME");
    QFile file;
    QString data;

    file.setFileName(homeDirectory + "/.config/plainDE/config.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();
    config = QJsonDocument::fromJson(data.toUtf8()).object();
}

void AppearancePane::prepareUI(QListWidget* iconThemeListWidget) {
    // Icon themes

    // TODO: ~/.locale/share/icons
    QDir themesDir("/usr/share/icons");
    QStringList themesList = themesDir.entryList();
    themesList.removeFirst();
    themesList.removeFirst();

    for (short i = 0; i < themesList.length(); ++i) {
        iconThemeListWidget->addItem(themesList.at(i));
    }
}

void AppearancePane::setCurrentSettings(QListWidget* iconThemeListWidget,
                                        QFontComboBox* fontFamilyComboBox,
                                        QSpinBox* fontSizeSpinBox,
                                        QRadioButton* lightRadioButton,
                                        QRadioButton* darkRadioButton,
                                        QSpinBox* panelHeightSpinBox) {
    // Icon theme
    QString currentRowData = "";
    short i = 0;
    while (currentRowData != config["iconTheme"].toString()) {
        currentRowData = iconThemeListWidget->item(i)->text();
        ++i;
    }
    iconThemeListWidget->setCurrentRow(i-1);

    // Font
    QFont currentFont;
    currentFont.setFamily(config["fontFamily"].toString());
    fontFamilyComboBox->setCurrentFont(currentFont);
    fontSizeSpinBox->setValue(config["fontSize"].toInt());

    // Theme
    if (config["theme"] == "light") {
        lightRadioButton->toggle();
    }
    else {
        darkRadioButton->toggle();
    }

    // Panel height
    panelHeightSpinBox->setValue(config["panelHeight"].toInt());
}

void AppearancePane::saveSettings(QListWidget* iconThemeListWidget,
                                  QFontComboBox* fontFamilyComboBox,
                                  QSpinBox* fontSizeSpinBox,
                                  QRadioButton* lightRadioButton,
                                  QSpinBox* panelHeightSpinBox) {
    // Icon theme
    config["iconTheme"] = QJsonValue(iconThemeListWidget->currentItem()->text());

    // Font
    config["fontFamily"] = QJsonValue(fontFamilyComboBox->currentFont().family());
    config["fontSize"] = QJsonValue(fontSizeSpinBox->value());

    // Theme
    if (lightRadioButton->isChecked()) {
        config["theme"] = "light";
    }
    else {
        config["theme"] = "dark";
    }

    // Panel height
    config["panelHeight"] = QJsonValue(panelHeightSpinBox->value());

    Pane::saveConfig(config);

    QMessageBox msg;
    msg.setWindowTitle("Restart plainDE");
    msg.setText("Changes will take effect after restarting plainDE");
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setIcon(QMessageBox::Information);
    msg.exec();
}

QWidget* AppearancePane::createUI(QWidget* controlCenter) {
    readConfig();

    QWidget* appearancePane = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(4, 4, 4, 4);
    appearancePane->setLayout(layout);

    short width = 400, height = 500;
    appearancePane->setGeometry(250, 250, width, height);

    QPushButton* backPushButton = new QPushButton;
    QIcon::setThemeName(config["iconTheme"].toString());
    backPushButton->setIcon(QIcon::fromTheme("go-previous"));
    backPushButton->setText(" Back");
    appearancePane->layout()->addWidget(backPushButton);

    QLabel* iconThemeLabel = new QLabel("Icon theme");
    QListWidget* iconThemeListWidget = new QListWidget;
    appearancePane->layout()->addWidget(iconThemeLabel);
    appearancePane->layout()->addWidget(iconThemeListWidget);

    QLabel* fontLabel = new QLabel("Font");
    QFontComboBox* fontFamilyComboBox = new QFontComboBox;
    QSpinBox* fontSizeSpinBox = new QSpinBox;
    appearancePane->layout()->addWidget(fontLabel);
    appearancePane->layout()->addWidget(fontFamilyComboBox);
    appearancePane->layout()->addWidget(fontSizeSpinBox);

    QLabel* themeLabel = new QLabel("Theme");
    QRadioButton* lightRadioButton = new QRadioButton("Light");
    QRadioButton* darkRadioButton = new QRadioButton("Dark");
    appearancePane->layout()->addWidget(themeLabel);
    appearancePane->layout()->addWidget(lightRadioButton);
    appearancePane->layout()->addWidget(darkRadioButton);

    QLabel* panelHeightLabel = new QLabel("Panel height");
    QSpinBox* panelHeightSpinBox = new QSpinBox;
    appearancePane->layout()->addWidget(panelHeightLabel);
    appearancePane->layout()->addWidget(panelHeightSpinBox);

    QPushButton* revertPushButton = new QPushButton("Revert");
    QPushButton* savePushButton = new QPushButton("Save");
    appearancePane->layout()->addWidget(revertPushButton);
    appearancePane->layout()->addWidget(savePushButton);

    prepareUI(iconThemeListWidget);
    setCurrentSettings(iconThemeListWidget,
                       fontFamilyComboBox,
                       fontSizeSpinBox,
                       lightRadioButton,
                       darkRadioButton,
                       panelHeightSpinBox);


    // Make connections
    appearancePane->connect(savePushButton, &QPushButton::clicked, appearancePane,
                  [this, iconThemeListWidget, fontFamilyComboBox, fontSizeSpinBox,
                   lightRadioButton, panelHeightSpinBox]() {
        saveSettings(iconThemeListWidget,
                     fontFamilyComboBox,
                     fontSizeSpinBox,
                     lightRadioButton,
                     panelHeightSpinBox);
    });

    appearancePane->connect(revertPushButton, &QPushButton::clicked, appearancePane,
                  [this, iconThemeListWidget, fontFamilyComboBox, fontSizeSpinBox,
                   lightRadioButton, darkRadioButton, panelHeightSpinBox]() {
        setCurrentSettings(iconThemeListWidget,
                           fontFamilyComboBox,
                           fontSizeSpinBox,
                           lightRadioButton,
                           darkRadioButton,
                           panelHeightSpinBox);
    });

    appearancePane->connect(backPushButton, &QPushButton::clicked, appearancePane,
                  [appearancePane, controlCenter]() {
        controlCenter->show();
        appearancePane->hide();
    });


    return appearancePane;
}
