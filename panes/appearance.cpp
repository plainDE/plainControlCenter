#include "appearance.h"
#include "../settings.h"
#include "../pane.h"

QJsonObject appearancePaneConfig;


void AppearancePane::readConfig() {
    QString homeDirectory = getenv("HOME");
    QFile file;
    QString data;

    file.setFileName(homeDirectory + "/.config/plainDE/config.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();
    appearancePaneConfig = QJsonDocument::fromJson(data.toUtf8()).object();
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
                                        QLineEdit* accentLineEdit) {
    // Icon theme
    QString currentRowData = "";
    short i = 0;

    if (!appearancePaneConfig["iconTheme"].toString().isEmpty()) {
        while (currentRowData != appearancePaneConfig["iconTheme"].toString()) {
            currentRowData = iconThemeListWidget->item(i)->text();
            ++i;
        }
        iconThemeListWidget->setCurrentRow(i-1);
    }

    // Font
    QFont currentFont;
    currentFont.setFamily(appearancePaneConfig["fontFamily"].toString());
    fontFamilyComboBox->setCurrentFont(currentFont);
    fontSizeSpinBox->setValue(appearancePaneConfig["fontSize"].toInt());

    // Theme
    if (appearancePaneConfig["theme"] == "light") {
        lightRadioButton->toggle();
    }
    else {
        darkRadioButton->toggle();
    }

    // Accent
    accentLineEdit->setText(appearancePaneConfig["accent"].toString());
}

void AppearancePane::saveSettings(QListWidget* iconThemeListWidget,
                                  QFontComboBox* fontFamilyComboBox,
                                  QSpinBox* fontSizeSpinBox,
                                  QRadioButton* lightRadioButton,
                                  QLineEdit* accentLineEdit) {
    // Icon theme
    appearancePaneConfig["iconTheme"] = QJsonValue(iconThemeListWidget->currentItem()->text());

    // Font
    appearancePaneConfig["fontFamily"] = QJsonValue(fontFamilyComboBox->currentFont().family());
    appearancePaneConfig["fontSize"] = QJsonValue(fontSizeSpinBox->value());

    // Theme
    if (lightRadioButton->isChecked()) {
        appearancePaneConfig["theme"] = "light";
    }
    else {
        appearancePaneConfig["theme"] = "dark";
    }

    // Accent
    appearancePaneConfig["accent"] = QJsonValue(accentLineEdit->text());

    Pane::saveConfig(appearancePaneConfig);
}

QWidget* AppearancePane::createUI(QWidget* controlCenter) {
    readConfig();

    // UI
    QWidget* appearancePane = new QWidget;
    appearancePane->setObjectName("appearancePane");
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(4, 4, 4, 4);
    appearancePane->setLayout(layout);

    // Style
    if (appearancePaneConfig["theme"] == "light") {
        QFile stylesheetReader(":/styles/general-light.qss");
        stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream styleSheet(&stylesheetReader);
        appearancePane->setStyleSheet(styleSheet.readAll());
    }
    else {
        QFile stylesheetReader(":/styles/general-dark.qss");
        stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream styleSheet(&stylesheetReader);
        appearancePane->setStyleSheet(styleSheet.readAll());
    }

    QFont paneFont;
    paneFont.setFamily(appearancePaneConfig["fontFamily"].toString());
    paneFont.setPointSize(appearancePaneConfig["fontSize"].toInt());
    appearancePane->setFont(paneFont);

    short width = 400, height = 500;
    appearancePane->setGeometry(250, 250, width, height);

    QPushButton* backPushButton = new QPushButton;
    backPushButton->setIcon(QIcon::fromTheme("go-previous"));
    backPushButton->setText(" Back");
    appearancePane->layout()->addWidget(backPushButton);

    QLabel* iconThemeLabel = new QLabel("Icon theme");
    QListWidget* iconThemeListWidget = new QListWidget;
    appearancePane->layout()->addWidget(iconThemeLabel);
    appearancePane->layout()->addWidget(iconThemeListWidget);
    iconThemeListWidget->setStyleSheet("QListView::item:selected { background-color: " + \
                                       appearancePaneConfig["accent"].toString() + \
                                       "; color: #ffffff };");

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

    QColorDialog* colorDialog = new QColorDialog;
    QLabel* accentLabel = new QLabel("Accent");
    QLineEdit* accentLineEdit = new QLineEdit;
    QPushButton* setAccentPushButton = new QPushButton("Choose accent...");
    appearancePane->layout()->addWidget(accentLabel);
    appearancePane->layout()->addWidget(accentLineEdit);
    appearancePane->layout()->addWidget(setAccentPushButton);

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
                       accentLineEdit);


    // Make connections
    appearancePane->connect(savePushButton, &QPushButton::clicked, appearancePane,
                  [this, iconThemeListWidget, fontFamilyComboBox, fontSizeSpinBox,
                   lightRadioButton, accentLineEdit]() {
        saveSettings(iconThemeListWidget,
                     fontFamilyComboBox,
                     fontSizeSpinBox,
                     lightRadioButton,
                     accentLineEdit);
    });

    appearancePane->connect(revertPushButton, &QPushButton::clicked, appearancePane,
                  [this, iconThemeListWidget, fontFamilyComboBox, fontSizeSpinBox,
                   lightRadioButton, darkRadioButton, accentLineEdit]() {
        setCurrentSettings(iconThemeListWidget,
                           fontFamilyComboBox,
                           fontSizeSpinBox,
                           lightRadioButton,
                           darkRadioButton,
                           accentLineEdit);
    });

    appearancePane->connect(backPushButton, &QPushButton::clicked, appearancePane,
                  [appearancePane, controlCenter]() {
        controlCenter->show();
        appearancePane->hide();
        delete appearancePane;
    });

    appearancePane->connect(setAccentPushButton, &QPushButton::clicked, appearancePane,
                            [colorDialog, accentLineEdit]() {
        if (colorDialog->exec() == QColorDialog::Accepted) {
            accentLineEdit->setText(QVariant(colorDialog->currentColor()).toString());
        }
    });


    return appearancePane;
}
