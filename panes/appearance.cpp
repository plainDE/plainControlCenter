#include "appearance.h"
#include "../settings.h"
#include "../pane.h"

QJsonObject appearancePaneConfig;


void AppearancePane::updateStyle(QWidget* pane) {
    QFile stylesheetReader("/usr/share/plainDE/styles/" + appearancePaneConfig["theme"].toString());
    stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream styleSheet(&stylesheetReader);
    pane->setStyleSheet(styleSheet.readAll());
    stylesheetReader.close();

    QFont paneFont;
    paneFont.setFamily(appearancePaneConfig["fontFamily"].toString());
    paneFont.setPointSize(appearancePaneConfig["fontSize"].toInt());
    pane->setFont(paneFont);
}

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

void AppearancePane::prepareUI(QListWidget* iconThemeListWidget,
                               QListWidget* themesListWidget) {
    // Icon themes

    // TODO: ~/.locale/share/icons
    QDir iconThemesDir("/usr/share/icons");
    QStringList iconThemesList = iconThemesDir.entryList();
    iconThemesList.removeFirst();
    iconThemesList.removeFirst();

    QDir panelThemesDir("/usr/share/plainDE/styles");
    QStringList panelThemesList = panelThemesDir.entryList();
    panelThemesList.removeFirst();
    panelThemesList.removeFirst();

    for (short i = 0; i < iconThemesList.length(); ++i) {
        iconThemeListWidget->addItem(iconThemesList.at(i));
    }

    for (short i = 0; i < panelThemesList.length(); ++i) {
        themesListWidget->addItem(panelThemesList.at(i));
    }
}

void AppearancePane::setCurrentSettings(QListWidget* iconThemeListWidget,
                                        QFontComboBox* fontFamilyComboBox,
                                        QSpinBox* fontSizeSpinBox,
                                        QListWidget* themesListWidget,
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
    currentRowData = "";
    i = 0;
    while (currentRowData != appearancePaneConfig["theme"].toString()) {
        currentRowData = themesListWidget->item(i)->text();
        ++i;
    }
    themesListWidget->setCurrentRow(i-1);

    for (short i = 0; i < themesListWidget->selectedItems().length(); ++i) {
        if (themesListWidget->item(i)->text() ==
                appearancePaneConfig["theme"].toString()) {
            themesListWidget->setCurrentRow(i);
        }
    }


    // Accent
    accentLineEdit->setText(appearancePaneConfig["accent"].toString());
}

void AppearancePane::saveSettings(QListWidget* iconThemeListWidget,
                                  QFontComboBox* fontFamilyComboBox,
                                  QSpinBox* fontSizeSpinBox,
                                  QListWidget* themesListWidget,
                                  QLineEdit* accentLineEdit) {
    // Icon theme
    if (!iconThemeListWidget->selectedItems().isEmpty()) {
        appearancePaneConfig["iconTheme"] = QJsonValue(
                    iconThemeListWidget->selectedItems()[0]->text());
    }

    // Font
    appearancePaneConfig["fontFamily"] = QJsonValue(fontFamilyComboBox->currentFont().family());
    appearancePaneConfig["fontSize"] = QJsonValue(fontSizeSpinBox->value());

    // Theme
    if (!themesListWidget->selectedItems().isEmpty()) {
        appearancePaneConfig["theme"] = QJsonValue(
                    themesListWidget->selectedItems()[0]->text());
    }

    // Accent
    appearancePaneConfig["accent"] = QJsonValue(accentLineEdit->text());

    Pane::saveConfig(appearancePaneConfig);
}

QWidget* AppearancePane::createUI(Settings* controlCenter) {
    readConfig();

    // UI
    QWidget* appearancePane = new QWidget;
    appearancePane->setObjectName("appearancePane");
    appearancePane->setWindowTitle("plainControlCenter");
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(4, 4, 4, 4);
    appearancePane->setLayout(layout);

    // Appearance
    updateStyle(appearancePane);

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
    appearancePane->layout()->addWidget(themeLabel);
    QListWidget* themesListWidget = new QListWidget;
    appearancePane->layout()->addWidget(themesListWidget);
    themesListWidget->setStyleSheet("QListView::item:selected { background-color: " + \
                                    appearancePaneConfig["accent"].toString() + \
                                    "; color: #ffffff };");

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

    prepareUI(iconThemeListWidget, themesListWidget);
    setCurrentSettings(iconThemeListWidget,
                       fontFamilyComboBox,
                       fontSizeSpinBox,
                       themesListWidget,
                       accentLineEdit);


    // Make connections
    appearancePane->connect(savePushButton, &QPushButton::clicked, appearancePane,
                  [this, iconThemeListWidget, fontFamilyComboBox, fontSizeSpinBox,
                   themesListWidget, accentLineEdit, appearancePane]() {
        saveSettings(iconThemeListWidget,
                     fontFamilyComboBox,
                     fontSizeSpinBox,
                     themesListWidget,
                     accentLineEdit);

        updateStyle(appearancePane);
    });

    appearancePane->connect(revertPushButton, &QPushButton::clicked, appearancePane,
                  [this, iconThemeListWidget, fontFamilyComboBox, fontSizeSpinBox,
                   themesListWidget, accentLineEdit]() {
        setCurrentSettings(iconThemeListWidget,
                           fontFamilyComboBox,
                           fontSizeSpinBox,
                           themesListWidget,
                           accentLineEdit);
    });

    appearancePane->connect(backPushButton, &QPushButton::clicked, appearancePane,
                  [appearancePane, controlCenter]() {
        controlCenter->updateStyle();
        controlCenter->controlCenterWidget->show();
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
