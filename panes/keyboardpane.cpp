#include "keyboardpane.h"

void KeyboardPane::setPaneContents() {
    mContentsWidget = new QWidget();
    QVBoxLayout* contentsLayout = new QVBoxLayout(mContentsWidget);
    mContentsWidget->setObjectName("innerPane");

    // Adding widgets
    QLabel* activeLayoutsLabel = new QLabel(tr("Active Keyboard Layouts"));
    activeLayoutsLabel->setFont(mTitleFont);
    contentsLayout->addWidget(activeLayoutsLabel);

    QHBoxLayout* layoutLayout = new QHBoxLayout();

    QListWidget* activeLayoutsListWidget = new QListWidget();
    activeLayoutsListWidget->setFont(mFont);
    QString style = QString("QListView::item:selected { "
                            "background-color: %1; color: #ffffff; "
                            "};").arg(getConfigValue("accent").toString());
    activeLayoutsListWidget->setStyleSheet(style);
    layoutLayout->addWidget(activeLayoutsListWidget);

    QVBoxLayout* manageLayoutsLayout = new QVBoxLayout();

    QPushButton* moveUpButton = new QPushButton("↑");  // U+2191 - Up arrow
    moveUpButton->setFont(mFont);
    manageLayoutsLayout->addWidget(moveUpButton);

    QPushButton* moveDownButton = new QPushButton("↓");  // U+2193 - Down arrow
    moveDownButton->setFont(mFont);
    manageLayoutsLayout->addWidget(moveDownButton);

    QPushButton* addButton = new QPushButton("+");
    addButton->setFont(mFont);
    manageLayoutsLayout->addWidget(addButton);

    QPushButton* removeButton = new QPushButton("-");
    removeButton->setFont(mFont);
    manageLayoutsLayout->addWidget(removeButton);

    layoutLayout->addLayout(manageLayoutsLayout);

    contentsLayout->addLayout(layoutLayout);

    QLabel* toggleMethodLabel = new QLabel("\n" + tr("Layout Toggle Method"));
    toggleMethodLabel->setFont(mTitleFont);
    contentsLayout->addWidget(toggleMethodLabel);

    QListWidget* toggleMethodsListWidget = new QListWidget();
    toggleMethodsListWidget->setFont(mFont);
    toggleMethodsListWidget->setStyleSheet(style);
    contentsLayout->addWidget(toggleMethodsListWidget);

    contentsLayout->addSpacerItem(new QSpacerItem(0, 0,
                                                  QSizePolicy::Ignored,
                                                  QSizePolicy::MinimumExpanding));


    QPushButton* saveButton = new QPushButton(tr("Save settings"));
    saveButton->setFont(mFont);
    contentsLayout->addWidget(saveButton);
    saveButton->setVisible(false);


    // Misc
    QFile layoutsFile("/usr/share/plainDE/layouts.json");
    layoutsFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString layoutsData = layoutsFile.readAll();
    layoutsFile.close();
    mLayoutCodes = QJsonDocument::fromJson(
                       layoutsData.toUtf8()).object().toVariantMap();

    QFile toggleMethodsFile("/usr/share/plainDE/layoutSwitchMethods.json");
    toggleMethodsFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString togglesData = toggleMethodsFile.readAll();
    toggleMethodsFile.close();
    mToggleMethods = QJsonDocument::fromJson(
                         togglesData.toUtf8()).object().toVariantMap();

    foreach (QVariant toggleMethod, mToggleMethods.values()) {
        toggleMethodsListWidget->addItem(toggleMethod.toString());
    }


    // Setting current settings
    foreach (QString layout, getConfigValue("kbLayouts").toString().split(',')) {
        QString flagPath = QString("/usr/share/flags/%1.png").arg(layout);
        QListWidgetItem* layoutItem = new QListWidgetItem(QIcon(flagPath),
                                                          mLayoutCodes.key(layout));
        activeLayoutsListWidget->addItem(layoutItem);
    }

    QString toggleMethod = getConfigValue("kbLayoutToggle").toString();
    for (int i = 0; i < toggleMethodsListWidget->count(); ++i) {
        if (!toggleMethodsListWidget->item(i)->text().compare(mToggleMethods[toggleMethod].toString())) {
            toggleMethodsListWidget->setCurrentRow(i);
            break;
        }
    }


    // Making connections
    QAbstractItemModel* model = activeLayoutsListWidget->model();
    connect(model, &QAbstractItemModel::rowsInserted, saveButton, &QPushButton::show);
    connect(model, &QAbstractItemModel::rowsRemoved, saveButton, &QPushButton::show);

    connect(toggleMethodsListWidget, &QListWidget::itemSelectionChanged,
            saveButton, &QPushButton::show);

    connect(addButton, &QPushButton::clicked, this, [this, activeLayoutsListWidget]() {
        auto layoutAddDialog = new LayoutAddDialog(mCfgObj, activeLayoutsListWidget);
        layoutAddDialog->setPaneContents();
        layoutAddDialog->show();
    });

    connect(removeButton, &QPushButton::clicked, this, [activeLayoutsListWidget]() {
        if (!activeLayoutsListWidget->selectedItems().isEmpty()) {
            delete activeLayoutsListWidget->selectedItems()[0];
        }
    });

    connect(moveUpButton, &QPushButton::clicked, this, [activeLayoutsListWidget]() {
        if (!activeLayoutsListWidget->selectedItems().isEmpty()) {
            int currentPos = activeLayoutsListWidget->currentRow();
            if (currentPos > 0) {
                activeLayoutsListWidget->insertItem(currentPos - 1,
                                                    activeLayoutsListWidget->takeItem(currentPos));
                activeLayoutsListWidget->setCurrentRow(currentPos - 1);
            }
        }
    });

    connect(moveDownButton, &QPushButton::clicked, this, [activeLayoutsListWidget]() {
        if (!activeLayoutsListWidget->selectedItems().isEmpty()) {
            int currentPos = activeLayoutsListWidget->currentRow();
            if (currentPos < activeLayoutsListWidget->count() - 1) {
                activeLayoutsListWidget->insertItem(currentPos + 1,
                                                    activeLayoutsListWidget->takeItem(currentPos));
                activeLayoutsListWidget->setCurrentRow(currentPos + 1);
            }
        }
    });

    connect(saveButton, &QPushButton::clicked, this, [this,
                                                      activeLayoutsListWidget,
                                                      toggleMethodsListWidget,
                                                      saveButton]() {
        prepareToSave(activeLayoutsListWidget, toggleMethodsListWidget);
        saveConfig();
        saveButton->setVisible(false);
    });


    finalizeUI();
}

void KeyboardPane::prepareToSave(QListWidget* activeLayoutsListWidget,
                                 QListWidget* toggleMethodsListWidget) {
    QString kbLayouts;
    for (int i = 0; i < activeLayoutsListWidget->count(); ++i) {
        kbLayouts += mLayoutCodes[activeLayoutsListWidget->item(i)->text()].toString();
        kbLayouts += ",";
    }
    kbLayouts.chop(1);
    setEntry("kbLayouts", kbLayouts);

    if (!toggleMethodsListWidget->selectedItems().isEmpty()) {
        QString toggleMethod = mToggleMethods.key(
            toggleMethodsListWidget->selectedItems()[0]->text());
        setEntry("kbLayoutToggle", toggleMethod);
    }
}

KeyboardPane::KeyboardPane(QJsonObject* cfgObj) : Pane(nullptr,
                                                       cfgObj,
                                                       tr("Keyboard"),
                                                       "cs-keyboard") {

}
