#include "autostartpane.h"

#include "../dialogs/addautostartentrydialog.h"

void AutostartPane::setPaneContents() {
    mContentsWidget = new QWidget();
    QVBoxLayout* contentsLayout = new QVBoxLayout(mContentsWidget);
    mContentsWidget->setObjectName("innerPane");

    // Adding widgets
    QLabel* entriesLabel = new QLabel("Autostart Entries");
    entriesLabel->setFont(mTitleFont);
    contentsLayout->addWidget(entriesLabel);

    QHBoxLayout* autostartLayout = new QHBoxLayout();

    mAutostartListWidget = new QListWidget();
    mAutostartListWidget->setFont(mFont);
    QString style = QString("QListView::item:selected { "
                            "background-color: %1; color: #ffffff; "
                            "};").arg(getConfigValue("accent").toString());
    mAutostartListWidget->setStyleSheet(style);
    autostartLayout->addWidget(mAutostartListWidget);

    QVBoxLayout* buttonsLayout = new QVBoxLayout();

    QPushButton* addButton = new QPushButton("+");
    addButton->setFont(mFont);
    buttonsLayout->addWidget(addButton);

    QPushButton* removeButton = new QPushButton("-");
    removeButton->setFont(mFont);
    buttonsLayout->addWidget(removeButton);

    buttonsLayout->addSpacerItem(new QSpacerItem(0, 0,
                                                 QSizePolicy::Ignored,
                                                 QSizePolicy::MinimumExpanding));

    autostartLayout->addLayout(buttonsLayout);

    contentsLayout->addLayout(autostartLayout);


    // Set current settings
    QString path = QString("%1/.config/autostart").arg(QDir::homePath());
    QDir autostartDir(path);
    if (autostartDir.exists()) {
        QStringList autostartEntries = autostartDir.entryList(QDir::Files | QDir::NoDotAndDotDot);
        foreach (QString entry, autostartEntries) {
            QString name, iconName;
            QSettings desktopFileReader(autostartDir.absoluteFilePath(entry),
                                        QSettings::IniFormat);
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
            mAutostartListWidget->addItem(item);
            mEntryByItem[item] = autostartDir.absoluteFilePath(entry);
        }
    }
    else {
        autostartDir.mkpath(path);
    }


    // Making connections
    connect(addButton, &QPushButton::clicked, this, [this]() {
        auto addAutostartEntryDialog = new AddAutostartEntryDialog(mCfgObj, this);
        addAutostartEntryDialog->setPaneContents();
        addAutostartEntryDialog->show();
    });

    connect(removeButton, &QPushButton::clicked, this, [this]() {
        if (!mAutostartListWidget->selectedItems().isEmpty()) {
            QListWidgetItem* item = mAutostartListWidget->selectedItems()[0];
            QFile::remove(mEntryByItem[item]);
            mEntryByItem.remove(item);
            delete item;
        }
    });


    finalizeUI();
}

void AutostartPane::addEntry(QString entry) {
    QString name, iconName;
    QSettings desktopFileReader(entry, QSettings::IniFormat);
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
    mAutostartListWidget->addItem(item);

    QString filename = entry.split('/').last();
    QString path = QString("%1/.config/autostart/%2").arg(QDir::homePath(), filename);

    mEntryByItem[item] = path;
    if (QFile::exists(path)) {
        QFile::remove(path);
    }
    QFile::copy(entry, path);
}

AutostartPane::AutostartPane(QJsonObject* cfgObj) : Pane(nullptr,
                                                         cfgObj,
                                                         tr("Autostart"),
                                                         "cs-general") {

}
