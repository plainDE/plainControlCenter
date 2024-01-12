#include "launcherdialog.h"

QIcon LauncherDialog::resolveIconNameOrPath(QString iconNameOrPath) {
    if (QIcon::hasThemeIcon(iconNameOrPath)) {
        return QIcon::fromTheme(iconNameOrPath);
    }
    else if (QFile::exists(iconNameOrPath)) {
        return QIcon(iconNameOrPath);
    }
    else {
        return QIcon::fromTheme("dialog-question");
    }
}

DesktopEntry LauncherDialog::readDesktopEntry(QString path) {
    QString name, iconNameOrPath;

    QSettings desktopFileReader(path, QSettings::IniFormat);
    desktopFileReader.sync();
    desktopFileReader.beginGroup("Desktop Entry");
    name = desktopFileReader.value("Name").toString();
    iconNameOrPath = desktopFileReader.value("Icon").toString();
    desktopFileReader.endGroup();

    DesktopEntry desktopEntry;
    desktopEntry.name = name;

    desktopEntry.icon = resolveIconNameOrPath(iconNameOrPath);

    return desktopEntry;
}

void LauncherDialog::setPaneContents() {
    mContentsWidget = new QWidget();
    QVBoxLayout* contentsLayout = new QVBoxLayout(mContentsWidget);
    mContentsWidget->setObjectName("innerPane");

    // Adding widgets
    QLabel* selectTypeLabel = new QLabel(tr("Select Launcher Type:"));
    selectTypeLabel->setFont(mTitleFont);
    contentsLayout->addWidget(selectTypeLabel);

    QRadioButton* appRadioButton = new QRadioButton(tr("App"));
    appRadioButton->setFont(mFont);
    contentsLayout->addWidget(appRadioButton);

    QListWidget* entriesListWidget = new QListWidget();
    entriesListWidget->setFont(mFont);
    QString style = QString("QListView::item:selected { "
                            "background-color: %1; color: #ffffff; "
                            "};").arg(getConfigValue("accent").toString());
    entriesListWidget->setStyleSheet(style);
    contentsLayout->addWidget(entriesListWidget);

    QRadioButton* customExecRadioButton = new QRadioButton(tr("Custom executable"));
    customExecRadioButton->setFont(mFont);
    contentsLayout->addWidget(customExecRadioButton);

    QHBoxLayout* pathLayout = new QHBoxLayout();

    QLabel* pathLabel = new QLabel(tr("Path:"));
    pathLabel->setFont(mTitleFont);
    pathLayout->addWidget(pathLabel);

    QLineEdit* pathLineEdit = new QLineEdit();
    pathLineEdit->setFont(mFont);
    pathLineEdit->setPlaceholderText(tr("Type path to the executable here..."));
    pathLayout->addWidget(pathLineEdit);

    contentsLayout->addLayout(pathLayout);

    QHBoxLayout* iconLayout = new QHBoxLayout();

    QLabel* iconLabel = new QLabel(tr("Icon:"));
    iconLabel->setFont(mTitleFont);
    iconLayout->addWidget(iconLabel);

    QLineEdit* iconLineEdit = new QLineEdit();
    iconLineEdit->setFont(mFont);
    iconLineEdit->setPlaceholderText(tr("Type icon name or path here..."));
    iconLayout->addWidget(iconLineEdit);

    contentsLayout->addLayout(iconLayout);

    QHBoxLayout* titleLayout = new QHBoxLayout();

    QLabel* titleLabel = new QLabel(tr("Title:"));
    titleLabel->setFont(mTitleFont);
    titleLayout->addWidget(titleLabel);

    QLineEdit* titleLineEdit = new QLineEdit();
    titleLineEdit->setFont(mFont);
    titleLineEdit->setPlaceholderText(tr("Type title here..."));
    titleLayout->addWidget(titleLineEdit);

    contentsLayout->addLayout(titleLayout);

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

    // Misc
    QDir appDir("/usr/share/applications");
    QStringList desktopEntriesList = appDir.entryList(QDir::Files | QDir::NoDotAndDotDot);

    for (int i = 0; i < desktopEntriesList.length(); ++i) {
        QString name, iconName;
        QString desktopEntryPath = appDir.absoluteFilePath(desktopEntriesList[i]);
        if (desktopEntryPath.endsWith(".desktop")) {
            DesktopEntry desktopEntry = readDesktopEntry(desktopEntryPath);

            QListWidgetItem* item = new QListWidgetItem(desktopEntry.icon, desktopEntry.name);

            entriesListWidget->addItem(item);
            mEntryByItem[item] = desktopEntryPath;
        }
    }

    QString homeDir = getenv("HOME");
    if (QDir(homeDir + "/.local/share/applications").exists()) {
        appDir.cd(homeDir + "/.local/share/applications");
        desktopEntriesList = appDir.entryList(QDir::Files | QDir::NoDotAndDotDot);

        for (int i = 0; i < desktopEntriesList.length(); ++i) {
            QString name, iconName;
            QString desktopEntryPath = appDir.absoluteFilePath(desktopEntriesList[i]);
            if (desktopEntryPath.endsWith(".desktop")) {
                DesktopEntry desktopEntry = readDesktopEntry(desktopEntryPath);

                QListWidgetItem* item = new QListWidgetItem(desktopEntry.icon, desktopEntry.name);
                entriesListWidget->addItem(item);
                mEntryByItem[item] = desktopEntryPath;
            }
        }
    }

    entriesListWidget->sortItems();

    // Setting current settings
    if (!mNewApplet) {
        QString panelName = QString("panel%1").arg(QString::number(mPanelID));
        QString applet = getConfigValue(panelName, "applets").toArray().at(mItem).toString();
        if (applet.endsWith(".desktop")) {
            QString entry = applet.split(':').last();
            appRadioButton->setChecked(true);
            QString globalEntry = QString("/usr/share/applications/%1").arg(entry);
            QListWidgetItem* itemA = mEntryByItem.key(globalEntry);
            if (itemA != NULL) {
                entriesListWidget->setCurrentItem(itemA);
            }
            else {
                QString homePath = QDir::homePath();
                QString localEntry = QString("%1/.local/share/applications/%2").arg(homeDir, entry);
                QListWidgetItem* itemB = mEntryByItem.key(localEntry);
                if (itemB != NULL) {
                    entriesListWidget->setCurrentItem(itemB);
                }
            }

            pathLineEdit->setVisible(false);
            pathLabel->setVisible(false);

            iconLineEdit->setVisible(false);
            iconLabel->setVisible(false);

            titleLineEdit->setVisible(false);
            titleLabel->setVisible(false);
        }
        else {
            QStringList appletList = applet.split(':');
            QString path = appletList[1];
            QString icon = appletList[2];
            QString title;
            if (appletList.count() == 4) {
                title = appletList[3];
            }

            customExecRadioButton->setChecked(true);
            pathLineEdit->setText(path);
            iconLineEdit->setText(icon);
            titleLineEdit->setText(title);

            entriesListWidget->setVisible(false);

            pathLineEdit->setVisible(true);
            pathLabel->setVisible(true);

            iconLineEdit->setVisible(true);
            iconLabel->setVisible(true);

            titleLineEdit->setVisible(true);
            titleLabel->setVisible(true);
        }
    }
    else {
        appRadioButton->setChecked(true);

        pathLineEdit->setVisible(false);
        pathLabel->setVisible(false);

        iconLineEdit->setVisible(false);
        iconLabel->setVisible(false);

        titleLineEdit->setVisible(false);
        titleLabel->setVisible(false);
    }

    // Making connections
    connect(cancelButton, &QPushButton::clicked, this, [this]() {
        this->hide();
        delete this;
    });

    connect(okButton, &QPushButton::clicked, this, [this, appRadioButton, entriesListWidget,
                                                    pathLineEdit, iconLineEdit, titleLineEdit]() {
        if (!mNewApplet) {
            prepareToSave(appRadioButton, entriesListWidget, pathLineEdit, iconLineEdit, titleLineEdit);
            saveConfig();


        }
        else {
            QListWidgetItem* item = new QListWidgetItem();

            QString applet;
            bool ok = false;
            if (appRadioButton->isChecked()) {
                if (!entriesListWidget->selectedItems().isEmpty()) {
                    QString desktopEntryPath = mEntryByItem[entriesListWidget->currentItem()];
                    applet = QString("launcher:%1").arg(desktopEntryPath.split('/').last());
                    ok = true;

                    DesktopEntry desktopEntry = readDesktopEntry(desktopEntryPath);
                    item->setText(desktopEntry.name);
                    item->setIcon(desktopEntry.icon);
                }
            }
            else {
                QString path = pathLineEdit->text();
                QString iconNameOrPath = iconLineEdit->text();
                QString title = titleLineEdit->text();
                if (!title.isEmpty()) {
                    title = "Launcher";
                }
                applet = QString("launcher:%1:%2:%3").arg(path, iconNameOrPath, title);
                ok = true;

                item->setIcon(resolveIconNameOrPath(iconNameOrPath));
            }

            if (ok) {
                item->setText(applet);
                mAppletsListWidget->addItem(item);
            }
        }
        this->hide();
        delete this;
    });

    connect(appRadioButton, &QRadioButton::clicked, this, [this, entriesListWidget, pathLineEdit,
                                                           pathLabel, iconLineEdit, iconLabel,
                                                           titleLineEdit, titleLabel]() {
        entriesListWidget->setVisible(true);

        pathLineEdit->setVisible(false);
        pathLabel->setVisible(false);

        iconLineEdit->setVisible(false);
        iconLabel->setVisible(false);

        titleLineEdit->setVisible(false);
        titleLabel->setVisible(false);
    });

    connect(customExecRadioButton, &QRadioButton::clicked, this, [this, entriesListWidget, pathLineEdit,
                                                                  pathLabel, iconLineEdit, iconLabel,
                                                                  titleLineEdit, titleLabel]() {
        entriesListWidget->setVisible(false);

        pathLineEdit->setVisible(true);
        pathLabel->setVisible(true);

        iconLineEdit->setVisible(true);
        iconLabel->setVisible(true);

        titleLineEdit->setVisible(true);
        titleLabel->setVisible(true);
    });

    finalizeUI();
    setWindowGeometry();
    setTransparency(this);
}

void LauncherDialog::prepareToSave(QRadioButton* appRadioButton, QListWidget* entriesListWidget,
                                   QLineEdit* pathLineEdit, QLineEdit* iconLineEdit, QLineEdit* titleLineEdit) {
    QString applet;
    bool ok = false;
    if (appRadioButton->isChecked()) {
        if (!entriesListWidget->selectedItems().isEmpty()) {
            QListWidgetItem* currentItem = entriesListWidget->currentItem();
            QString entry = mEntryByItem[currentItem].split('/').last();
            applet = QString("launcher:%1").arg(entry);
            mAppletsListWidget->item(mItem)->setText(applet);
            mAppletsListWidget->item(mItem)->setIcon(currentItem->icon());
            ok = true;
        }
    }
    else {
        QString path = pathLineEdit->text();
        QString iconNameOrPath = iconLineEdit->text();
        QString title = titleLineEdit->text();
        if (title.isEmpty()) {
            title = "Launcher";
        }
        applet = QString("launcher:%1:%2:%3").arg(path, iconNameOrPath, title);
        mAppletsListWidget->item(mItem)->setText(applet);
        mAppletsListWidget->item(mItem)->setIcon(resolveIconNameOrPath(iconNameOrPath));
        ok = true;
    }

    if (ok) {
        QString panelName = QString("panel%1").arg(QString::number(mPanelID));
        QJsonObject panelObj = getConfigValue(panelName).toObject();
        QJsonArray applets = panelObj["applets"].toArray();
        if (mNewApplet) {
            applets.append(applet);
        }
        else {
            applets[mItem] = applet;
        }
        panelObj["applets"] = QJsonValue(applets);
        setEntry(panelName, panelObj);
    }
}

LauncherDialog::LauncherDialog(QJsonObject* cfgObj,
                               int panelID,
                               bool newApplet,
                               int item,
                               QListWidget* appletsListWidget) : Dialog(cfgObj,
                                                  tr("Launcher applet settings"),
                                                  "utilities-terminal") {
    mPanelID = panelID;
    mNewApplet = newApplet;
    mItem = item;
    mAppletsListWidget = appletsListWidget;
}
