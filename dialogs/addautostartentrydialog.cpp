#include "addautostartentrydialog.h"

void AddAutostartEntryDialog::setPaneContents() {
    mContentsWidget = new QWidget();
    QVBoxLayout* contentsLayout = new QVBoxLayout(mContentsWidget);
    mContentsWidget->setObjectName("innerPane");

    // Adding widgets
    QLabel* selectEntryLabel = new QLabel(tr("Select app from the list:"));
    selectEntryLabel->setFont(mTitleFont);
    contentsLayout->addWidget(selectEntryLabel);

    QListWidget* entriesListWidget = new QListWidget();
    entriesListWidget->setFont(mFont);
    QString style = QString("QListView::item:selected { "
                            "background-color: %1; color: #ffffff; "
                            "};").arg(getConfigValue("accent").toString());
    entriesListWidget->setStyleSheet(style);
    contentsLayout->addWidget(entriesListWidget);

    QHBoxLayout* buttonsLayout = new QHBoxLayout();

    buttonsLayout->addSpacerItem(new QSpacerItem(0, 0,
                                                 QSizePolicy::MinimumExpanding,
                                                 QSizePolicy::Ignored));

    QPushButton* cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setFont(mFont);
    buttonsLayout->addWidget(cancelButton);

    QPushButton* addButton = new QPushButton(tr("Add"));
    addButton->setFont(mFont);
    buttonsLayout->addWidget(addButton);

    contentsLayout->addLayout(buttonsLayout);


    // Misc
    QDir appDir("/usr/share/applications");
    QStringList desktopEntriesList = appDir.entryList(QDir::Files | QDir::NoDotAndDotDot);

    for (int i = 0; i < desktopEntriesList.length(); ++i) {
        QString name, iconName;
        QString desktopEntryPath = appDir.absoluteFilePath(desktopEntriesList[i]);
        if (desktopEntryPath.endsWith(".desktop")) {
            QSettings desktopFileReader(desktopEntryPath, QSettings::IniFormat);
            desktopFileReader.sync();
            desktopFileReader.beginGroup("Desktop Entry");
            name = desktopFileReader.value("Name").toString();
            iconName = desktopFileReader.value("Icon").toString();
            desktopFileReader.endGroup();

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
                QSettings desktopFileReader(desktopEntryPath, QSettings::IniFormat);
                desktopFileReader.sync();
                desktopFileReader.beginGroup("Desktop Entry");
                name = desktopFileReader.value("Name").toString();
                iconName = desktopFileReader.value("Icon").toString();
                desktopFileReader.endGroup();

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
                entriesListWidget->addItem(item);

                mEntryByItem[item] = desktopEntryPath;
            }
        }
    }

    entriesListWidget->sortItems();


    // Making connections
    connect(cancelButton, &QPushButton::clicked, this, [this]() {
        this->hide();
        delete this;
    });

    connect(addButton, &QPushButton::clicked, this, [this, entriesListWidget]() {
        if (!entriesListWidget->selectedItems().isEmpty()) {
            QString entry = mEntryByItem[entriesListWidget->selectedItems()[0]];
            mParent->addEntry(entry);
            this->hide();
            delete this;
        }
    });

    finalizeUI();
    setWindowGeometry();
    setTransparency(this);
}

AddAutostartEntryDialog::AddAutostartEntryDialog(QJsonObject* cfgObj,
                                                 AutostartPane *parent) : Dialog(cfgObj,
                                                                                 tr("Adding Autostart Entry"),
                                                                                 "cs-general") {
    mParent = parent;
}
