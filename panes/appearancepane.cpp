#include "appearancepane.h"

#include "../mainwindow.h"

void AppearancePane::setPaneContents() {
    mContentsWidget = new QWidget();
    QVBoxLayout* contentsLayout = new QVBoxLayout(mContentsWidget);
    mContentsWidget->setObjectName("innerPane");

    QString comboBoxStyle = QString("QComboBox::item:selected { "
                                    "background-color: %1; color: #ffffff; "
                                    "};").arg(getConfigValue("accent").toString());

    QString fontComboBoxStyle = QString("QListView::item:selected { "
                                        "background-color: %1; color: #ffffff; "
                                        "};").arg(getConfigValue("accent").toString());

    // Adding widgets
    QFormLayout* iconThemeLayout = new QFormLayout();

    QLabel* iconThemeLabel = new QLabel(tr("Icon Theme"));
    iconThemeLabel->setFont(mTitleFont);

    QComboBox* iconThemeComboBox = new QComboBox();
    iconThemeComboBox->setFont(mFont);
    iconThemeComboBox->setStyleSheet(comboBoxStyle);
    QDir iconThemesDir("/usr/share/icons");
    QStringList iconThemesList = iconThemesDir.entryList(QDir::Dirs |
                                                         QDir::NoDotAndDotDot);
    foreach (QString iconTheme, iconThemesList) {
        iconThemeComboBox->addItem(iconTheme);
    }

    iconThemeLayout->addRow(iconThemeLabel, iconThemeComboBox);
    contentsLayout->addLayout(iconThemeLayout);


    QLabel* fontLabel = new QLabel("\n" + tr("Font"));
    fontLabel->setFont(mTitleFont);
    contentsLayout->addWidget(fontLabel);

    QFormLayout* fontLayout = new QFormLayout();
    fontLayout->setContentsMargins(10, 1, 1, 1);

    QLabel* familyLabel = new QLabel(tr("Family"));
    familyLabel->setFont(mTitleFont);
    QFontComboBox* fontComboBox = new QFontComboBox();
    fontComboBox->setFont(mFont);
    fontComboBox->setStyleSheet(fontComboBoxStyle);
    fontLayout->addRow(familyLabel, fontComboBox);

    QLabel* sizeLabel = new QLabel(tr("Size"));
    sizeLabel->setFont(mTitleFont);
    QSpinBox* sizeSpinBox = new QSpinBox();
    sizeSpinBox->setFont(mFont);
    fontLayout->addRow(sizeLabel, sizeSpinBox);

    contentsLayout->addLayout(fontLayout);


    QLabel* themeLabel = new QLabel("\n" + tr("Theme"));
    themeLabel->setFont(mTitleFont);
    contentsLayout->addWidget(themeLabel);

    QComboBox* themeComboBox = new QComboBox();
    themeComboBox->setFont(mFont);
    themeComboBox->setStyleSheet(comboBoxStyle);
    QDir themesDir("/usr/share/plainDE/styles");
    QStringList themesList = themesDir.entryList(QDir::Files |
                                                 QDir::NoDotAndDotDot);
    foreach (QString theme, themesList) {
        themeComboBox->addItem(theme);
    }
    contentsLayout->addWidget(themeComboBox);

    QLabel* themeTipLabel = new QLabel(tr("Themes can be found at "
                                          "/usr/share/plainDE/styles"));
    themeTipLabel->setFont(mFont);
    contentsLayout->addWidget(themeTipLabel);


    QColorDialog* accentColorDialog = new QColorDialog();

    QLabel* accentLabel = new QLabel("\n" + tr("Accent"));
    accentLabel->setFont(mTitleFont);
    contentsLayout->addWidget(accentLabel);

    QHBoxLayout* accentLayout = new QHBoxLayout();

    QLineEdit* accentLineEdit = new QLineEdit();
    accentLineEdit->setFont(mFont);
    accentLayout->addWidget(accentLineEdit);

    QPushButton* chooseAccentButton = new QPushButton("...");
    chooseAccentButton->setFont(mFont);
    accentLayout->addWidget(chooseAccentButton);

    accentLayout->addSpacerItem(new QSpacerItem(0, 0,
                                                QSizePolicy::MinimumExpanding,
                                                QSizePolicy::Ignored));
    contentsLayout->addLayout(accentLayout);


    QColorDialog* ipColorDialog = new QColorDialog();

    QLabel* ipColorLabel = new QLabel("\n" + tr("IP Address Color"));
    ipColorLabel->setFont(mTitleFont);
    contentsLayout->addWidget(ipColorLabel);

    QHBoxLayout* ipColorLayout = new QHBoxLayout();

    QLineEdit* ipColorLineEdit = new QLineEdit();
    ipColorLineEdit->setFont(mFont);
    ipColorLayout->addWidget(ipColorLineEdit);

    QPushButton* chooseIPColorButton = new QPushButton("...");
    chooseIPColorButton->setFont(mFont);
    ipColorLayout->addWidget(chooseIPColorButton);

    ipColorLayout->addSpacerItem(new QSpacerItem(0, 0,
                                                 QSizePolicy::MinimumExpanding,
                                                 QSizePolicy::Ignored));

    contentsLayout->addLayout(ipColorLayout);


    contentsLayout->addSpacerItem(new QSpacerItem(0, 0,
                                                  QSizePolicy::Ignored,
                                                  QSizePolicy::MinimumExpanding));


    QPushButton* saveButton = new QPushButton(tr("Save settings"));
    saveButton->setFont(mFont);
    contentsLayout->addWidget(saveButton);
    saveButton->setVisible(false);


    // Setting current settings
    QString iconTheme = getConfigValue("iconTheme").toString();
    if (!iconTheme.isEmpty()) {
        int i = iconThemeComboBox->findText(iconTheme);
        if (i != -1) {
            iconThemeComboBox->setCurrentIndex(i);
        }
    }

    fontComboBox->setCurrentFont(mFont);
    sizeSpinBox->setValue(mFont.pointSize());

    QString theme = getConfigValue("theme").toString();
    int i = themeComboBox->findText(theme);
    if (i != -1) {
        themeComboBox->setCurrentIndex(i);
    }

    accentLineEdit->setText(getConfigValue("accent").toString());

    ipColorLineEdit->setText(getConfigValue("ipColor").toString());


    // Making connections
    connect(iconThemeComboBox, &QComboBox::currentTextChanged,
            saveButton, &QPushButton::show);
    connect(fontComboBox, &QFontComboBox::currentFontChanged,
            saveButton, &QPushButton::show);
    connect(sizeSpinBox, static_cast<void(QSpinBox::*)(int)>(
                             &QSpinBox::valueChanged),
            saveButton, &QPushButton::show);
    connect(themeComboBox, &QComboBox::currentTextChanged,
            saveButton, &QPushButton::show);
    connect(accentLineEdit, &QLineEdit::textChanged,
            saveButton, &QPushButton::show);
    connect(ipColorLineEdit, &QLineEdit::textChanged,
            saveButton, &QPushButton::show);
    connect(chooseAccentButton, &QPushButton::clicked, this, [accentColorDialog,
                                                              accentLineEdit]() {
        if (accentColorDialog->exec() == QColorDialog::Accepted) {
            QString color = QVariant(accentColorDialog->currentColor()).toString();
            accentLineEdit->setText(color);
        }
    });
    connect(chooseIPColorButton, &QPushButton::clicked, this, [ipColorDialog,
                                                               ipColorLineEdit]() {
        if (ipColorDialog->exec() == QColorDialog::Accepted) {
            QString color = QVariant(ipColorDialog->currentColor()).toString();
            ipColorLineEdit->setText(color);
        }
    });
    connect(saveButton, &QPushButton::clicked, this, [this, iconThemeComboBox,
                                                      fontComboBox, sizeSpinBox,
                                                      themeComboBox, accentLineEdit,
                                                      ipColorLineEdit, saveButton]() {
        prepareToSave(iconThemeComboBox, fontComboBox, sizeSpinBox, themeComboBox,
                      accentLineEdit, ipColorLineEdit);
        saveConfig();
        saveButton->setVisible(false);

        static_cast<MainWindow*>(mParent)->setAppearance();
    });

    finalizeUI();
}

void AppearancePane::prepareToSave(QComboBox* iconThemeComboBox,
                                   QFontComboBox* fontComboBox,
                                   QSpinBox* fontSizeSpinBox,
                                   QComboBox* themeComboBox,
                                   QLineEdit* accentLineEdit,
                                   QLineEdit* ipColorLineEdit) {
    setEntry("iconTheme", QJsonValue(iconThemeComboBox->currentText()));
    setEntry("fontFamily", QJsonValue(fontComboBox->currentFont().family()));
    setEntry("fontSize", QJsonValue(fontSizeSpinBox->value()));
    setEntry("theme", QJsonValue(themeComboBox->currentText()));
    setEntry("accent", QJsonValue(accentLineEdit->text()));
    setEntry("ipColor", QJsonValue(ipColorLineEdit->text()));
}

AppearancePane::AppearancePane(QJsonObject* cfgObj,
                               QWidget* parent) : Pane(nullptr,
                                                       cfgObj,
                                                       tr("Appearance"),
                                                       "cs-themes") {
    mParent = parent;
}
