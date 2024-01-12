#include "appmenudialog.h"

QIcon AppMenuDialog::resolveIconNameOrPath(QString iconNameOrPath, bool showUnknown) {
    if (QIcon::hasThemeIcon(iconNameOrPath)) {
        return QIcon::fromTheme(iconNameOrPath);
    }
    else if (QFile::exists(iconNameOrPath)) {
        return QIcon(iconNameOrPath);
    }
    else if (showUnknown) {
        return QIcon::fromTheme("dialog-question");
    }
    else {
        return QIcon();
    }
}

void AppMenuDialog::setPaneContents() {
    mContentsWidget = new QWidget();
    QVBoxLayout* contentsLayout = new QVBoxLayout(mContentsWidget);
    mContentsWidget->setObjectName("innerPane");

    // Adding widgets
    QLabel* menuTitleLabel = new QLabel(tr("Menu Title"));
    menuTitleLabel->setFont(mTitleFont);
    contentsLayout->addWidget(menuTitleLabel);

    QLineEdit* menuTitleLineEdit = new QLineEdit();
    menuTitleLineEdit->setPlaceholderText(tr("Type app menu title here..."));
    menuTitleLineEdit->setFont(mFont);
    contentsLayout->addWidget(menuTitleLineEdit);

    QLabel* menuIconLabel = new QLabel(tr("Menu Icon"));
    menuIconLabel->setFont(mTitleFont);
    contentsLayout->addWidget(menuIconLabel);

    QHBoxLayout* menuIconLayout = new QHBoxLayout();

    QPushButton* sampleButton = new QPushButton();
    sampleButton->setFixedSize(16, 16);
    sampleButton->setFlat(true);
    menuIconLayout->addWidget(sampleButton);

    QLineEdit* menuIconLineEdit = new QLineEdit();
    menuIconLineEdit->setPlaceholderText(tr("Type app menu icon name or path here..."));
    menuIconLineEdit->setFont(mFont);
    menuIconLayout->addWidget(menuIconLineEdit);

    contentsLayout->addLayout(menuIconLayout);

    QHBoxLayout* menuIconSizeLayout = new QHBoxLayout();

    QLabel* menuIconSizeLabel = new QLabel(tr("Menu Icon Size"));
    menuIconSizeLabel->setFont(mTitleFont);
    menuIconSizeLayout->addWidget(menuIconSizeLabel);

    QSpinBox* menuIconSizeSpinBox = new QSpinBox();
    menuIconSizeSpinBox->setMinimum(0);
    menuIconSizeSpinBox->setMaximum(256);
    menuIconSizeSpinBox->setFont(mFont);
    menuIconSizeLayout->addWidget(menuIconSizeSpinBox);

    contentsLayout->addLayout(menuIconSizeLayout);

    QLabel* favAppsLabel = new QLabel(tr("Favorite apps"));
    favAppsLabel->setFont(mTitleFont);
    contentsLayout->addWidget(favAppsLabel);

    QHBoxLayout* favAppsLayout = new QHBoxLayout();

    QListWidget* appsListWidget = new QListWidget();
    appsListWidget->setFont(mFont);
    QString style = QString("QListView::item:selected { "
                            "background-color: %1; color: #ffffff; "
                            "};").arg(getConfigValue("accent").toString());
    appsListWidget->setStyleSheet(style);
    favAppsLayout->addWidget(appsListWidget);

    QVBoxLayout* favButtonsLayout = new QVBoxLayout();

    QPushButton* addAppButton = new QPushButton("+");
    addAppButton->setFont(mFont);
    favButtonsLayout->addWidget(addAppButton);

    QPushButton* removeAppButton = new QPushButton("-");
    removeAppButton->setFont(mFont);
    favButtonsLayout->addWidget(removeAppButton);

    QPushButton* moveUpButton = new QPushButton("↑");  // U+2191 - Up arrow
    moveUpButton->setFont(mFont);
    favButtonsLayout->addWidget(moveUpButton);

    QPushButton* moveDownButton = new QPushButton("↓");  // U+2193 - Down arrow
    moveDownButton->setFont(mFont);
    favButtonsLayout->addWidget(moveDownButton);

    favAppsLayout->addLayout(favButtonsLayout);

    QListWidget* favAppsListWidget = new QListWidget();
    favAppsListWidget->setFont(mFont);
    favAppsListWidget->setStyleSheet(style);
    favAppsLayout->addWidget(favAppsListWidget);

    contentsLayout->addLayout(favAppsLayout);

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

    // Misc
    QDir appDir("/usr/share/applications");
    QStringList desktopEntriesList = appDir.entryList(QDir::Files | QDir::NoDotAndDotDot);

    for (int i = 0; i < desktopEntriesList.length(); ++i) {
        QString name, iconNameOrPath;
        bool nodisplay;
        QString desktopEntryPath = appDir.absoluteFilePath(desktopEntriesList[i]);
        if (desktopEntryPath.endsWith(".desktop")) {
            QSettings desktopFileReader(desktopEntryPath, QSettings::IniFormat);
            desktopFileReader.sync();
            desktopFileReader.beginGroup("Desktop Entry");
            name = desktopFileReader.value("Name").toString();
            iconNameOrPath = desktopFileReader.value("Icon").toString();
            nodisplay = desktopFileReader.value("NoDisplay").toBool();
            desktopFileReader.endGroup();

            if (!nodisplay) {
                QListWidgetItem* item = new QListWidgetItem(name);
                item->setIcon(resolveIconNameOrPath(iconNameOrPath, true));
                appsListWidget->addItem(item);

                mEntryByItem[item] = desktopEntryPath;
            }
        }
    }

    QString homeDir = getenv("HOME");
    if (QDir(homeDir + "/.local/share/applications").exists()) {
        appDir.cd(homeDir + "/.local/share/applications");
        desktopEntriesList = appDir.entryList(QDir::Files | QDir::NoDotAndDotDot);

        for (int i = 0; i < desktopEntriesList.length(); ++i) {
            QString name, iconName;
            bool nodisplay;
            QString desktopEntryPath = appDir.absoluteFilePath(desktopEntriesList[i]);
            if (desktopEntryPath.endsWith(".desktop")) {
                QSettings desktopFileReader(desktopEntryPath, QSettings::IniFormat);
                desktopFileReader.sync();
                desktopFileReader.beginGroup("Desktop Entry");
                name = desktopFileReader.value("Name").toString();
                iconName = desktopFileReader.value("Icon").toString();
                nodisplay = desktopFileReader.value("NoDisplay").toBool();
                desktopFileReader.endGroup();

                if (!nodisplay) {
                    QListWidgetItem* item = new QListWidgetItem(name);
                    if (QIcon::hasThemeIcon(iconName)) {
                        item->setIcon(QIcon::fromTheme(iconName));
                    }
                    else if (QFile::exists(iconName)) {
                        item->setIcon(QIcon(iconName));
                    }
                    else {
                        item->setIcon(QIcon::fromTheme("dialog-question"));
                    }
                    appsListWidget->addItem(item);

                    mEntryByItem[item] = desktopEntryPath;
                }
            }
        }
    }

    appsListWidget->sortItems();

    // Setting current settings
    menuTitleLineEdit->setText(getConfigValue("menuText").toString());
    menuIconLineEdit->setText(getConfigValue("menuIcon").toString());
    sampleButton->setIcon(resolveIconNameOrPath(getConfigValue("menuIcon").toString(), false));
    menuIconSizeSpinBox->setValue(getConfigValue("menuIconSize").toInt());
    QJsonArray favApps = getConfigValue("favApps").toArray();
    foreach (QVariant favApp, favApps) {
        QString finalPath = QString("/usr/share/applications/%1").arg(favApp.toString());
        if (!QFile(finalPath).exists()) {
            QString homePath = QDir::homePath();
            finalPath = QString("%1/.local/share/applications/%2").arg(homePath, favApp.toString());
        }
        QListWidgetItem* item = mEntryByItem.key(finalPath);
        QListWidgetItem* favItem = new QListWidgetItem(item->icon(), item->text());
        favAppsListWidget->addItem(favItem);
        mEntryByItem[favItem] = mEntryByItem[item];
    }


    // Making connections
    connect(menuIconLineEdit, &QLineEdit::textChanged, this, [this,
                                                              menuIconLineEdit,
                                                              sampleButton]() {
        QString iconNameOrPath = menuIconLineEdit->text();
        sampleButton->setIcon(resolveIconNameOrPath(iconNameOrPath, false));
    });

    connect(addAppButton, &QPushButton::clicked, this, [this, appsListWidget, favAppsListWidget]() {
        if (!appsListWidget->selectedItems().isEmpty()) {
            QListWidgetItem* currentItem = appsListWidget->currentItem();

            QListWidgetItem* item = new QListWidgetItem(currentItem->icon(),
                                                        currentItem->text());
            favAppsListWidget->addItem(item);
            mEntryByItem[item] = mEntryByItem[currentItem];
        }
    });

    connect(removeAppButton, &QPushButton::clicked, this, [this, favAppsListWidget, appsListWidget]() {
        if (!favAppsListWidget->selectedItems().isEmpty()) {
            favAppsListWidget->takeItem(favAppsListWidget->currentRow());
        }
    });

    connect(moveUpButton, &QPushButton::clicked, this, [favAppsListWidget]() {
        if (!favAppsListWidget->selectedItems().isEmpty()) {
            int currentPos = favAppsListWidget->currentRow();
            if (currentPos > 0) {
                favAppsListWidget->insertItem(currentPos - 1,
                                              favAppsListWidget->takeItem(currentPos));
                favAppsListWidget->setCurrentRow(currentPos - 1);
            }
        }
    });

    connect(moveDownButton, &QPushButton::clicked, this, [favAppsListWidget]() {
        if (!favAppsListWidget->selectedItems().isEmpty()) {
            int currentPos = favAppsListWidget->currentRow();
            if (currentPos < favAppsListWidget->count() - 1) {
                favAppsListWidget->insertItem(currentPos + 1,
                                              favAppsListWidget->takeItem(currentPos));
                favAppsListWidget->setCurrentRow(currentPos + 1);
            }
        }
    });

    connect(cancelButton, &QPushButton::clicked, this, [this]() {
        this->hide();
        delete this;
    });

    connect(okButton, &QPushButton::clicked, this, [this, menuTitleLineEdit, menuIconLineEdit,
                                                    menuIconSizeSpinBox, favAppsListWidget]() {
        prepareToSave(menuTitleLineEdit,
                      menuIconLineEdit,
                      menuIconSizeSpinBox,
                      favAppsListWidget);
        saveConfig();
        this->hide();
        delete this;
    });

    finalizeUI();
    setWindowGeometry();
    setTransparency(this);
}

void AppMenuDialog::prepareToSave(QLineEdit* menuTitleLineEdit,
                                  QLineEdit* menuIconLineEdit,
                                  QSpinBox* menuIconSizeSpinBox,
                                  QListWidget* favAppsListWidget) {
    setEntry("menuText", menuTitleLineEdit->text());
    setEntry("menuIcon", menuIconLineEdit->text());
    setEntry("menuIconSize", menuIconSizeSpinBox->value());
    QJsonArray favApps;
    for (int i = 0; i < favAppsListWidget->count(); ++i) {
        QString desktopEntryPath = mEntryByItem[favAppsListWidget->item(i)];
        QString filename = desktopEntryPath.split('/').last();
        favApps.append(filename);
    }
    setEntry("favApps", favApps);
}

AppMenuDialog::AppMenuDialog(QJsonObject* cfgObj) : Dialog(cfgObj,
                                                           tr("App Menu applet settings"),
                                                           "app-launcher") {

}
