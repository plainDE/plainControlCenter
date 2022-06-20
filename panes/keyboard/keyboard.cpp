#include "keyboard.h"
#include "layoutdialog.h"

QJsonObject keyboardPaneConfig;
QVariantMap layoutCodes;
QVariantMap layoutSwitchMethods;


void KeyboardPane::readConfig() {
    QString homeDirectory = getenv("HOME");
    QFile file;
    QString data;

    file.setFileName(homeDirectory + "/.config/plainDE/config.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();
    keyboardPaneConfig = QJsonDocument::fromJson(data.toUtf8()).object();
}

void KeyboardPane::addLayout(QString layoutName, QListWidget* layoutList) {
    layoutList->addItem(new QListWidgetItem(QIcon("/usr/share/flags/" + layoutCodes[layoutName].toString() + ".png"),
                                            layoutName));
}

void KeyboardPane::setCurrentSettings(QCheckBox* useCountryFlagCheckBox,
                                      QListWidget* layoutList,
                                      QListWidget* layoutSwitchShortcut) {
    useCountryFlagCheckBox->setChecked(keyboardPaneConfig["useCountryFlag"].toBool());

    layoutList->clear();

    QString toggleMethod = keyboardPaneConfig["kbLayoutToggle"].toString();

    foreach(QString layout, keyboardPaneConfig["kbLayouts"].toString().split(',')) {
        layoutList->addItem(new QListWidgetItem(QIcon("/usr/share/flags/" + layout + ".png"),
                                                layoutCodes.key(layout)));
    }

    // 28 kb layout toggle methods
    for (qint8 i = 0; i < 28; ++i) {
        if (layoutSwitchShortcut->item(i)->text() == layoutSwitchMethods[toggleMethod].toString()) {
            layoutSwitchShortcut->setCurrentRow(i);
            break;
        }
    }
}

void KeyboardPane::saveSettings(QCheckBox* useCountryFlagCheckBox,
                                QListWidget* layoutList,
                                QListWidget* layoutSwitchShortcut) {
    QString activeLayouts = "";
    for (qint8 i = 0; i < layoutList->count(); ++i) {
        activeLayouts += layoutCodes[layoutList->item(i)->text()].toString();
        activeLayouts += ",";
    }
    activeLayouts.chop(1);
    keyboardPaneConfig["kbLayouts"] = QJsonValue(activeLayouts);

    QString toggleMethod = layoutSwitchMethods.key(
                layoutSwitchShortcut->selectedItems()[0]->text());
    keyboardPaneConfig["kbLayoutToggle"] = QJsonValue(toggleMethod);

    keyboardPaneConfig["useCountryFlag"] = QJsonValue(useCountryFlagCheckBox->isChecked());

    Pane::saveConfig(keyboardPaneConfig);
}

void KeyboardPane::prepareUI(QListWidget* layoutSwitchShortcut) {
    QFile layoutsFile;
    QString layoutsData;

    layoutsFile.setFileName("/usr/share/plainDE/layouts.json");
    layoutsFile.open(QIODevice::ReadOnly | QIODevice::Text);
    layoutsData = layoutsFile.readAll();
    layoutsFile.close();
    layoutCodes = QJsonDocument::fromJson(layoutsData.toUtf8()).object().toVariantMap();


    QFile switchMethodsFile;
    QString switchMethodsData;

    switchMethodsFile.setFileName("/usr/share/plainDE/layoutSwitchMethods.json");
    switchMethodsFile.open(QIODevice::ReadOnly | QIODevice::Text);
    switchMethodsData = switchMethodsFile.readAll();
    switchMethodsFile.close();
    layoutSwitchMethods = QJsonDocument::fromJson(switchMethodsData.toUtf8()).object().toVariantMap();

    layoutSwitchShortcut->clear();
    foreach (QVariant toggleMethod, layoutSwitchMethods.values()) {
        layoutSwitchShortcut->addItem(toggleMethod.toString());
    }
}

QWidget* KeyboardPane::createUI(QWidget* controlCenter) {
    readConfig();

    // UI
    QWidget* keyboardPane = new QWidget;
    keyboardPane->setObjectName("keyboardPane");
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(4, 4, 4, 4);
    keyboardPane->setLayout(layout);

    // Theme
    QFile stylesheetReader("/usr/share/plainDE/styles/" + keyboardPaneConfig["theme"].toString());
    stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream styleSheet(&stylesheetReader);
    keyboardPane->setStyleSheet(styleSheet.readAll());

    QFont paneFont;
    paneFont.setFamily(keyboardPaneConfig["fontFamily"].toString());
    paneFont.setPointSize(keyboardPaneConfig["fontSize"].toInt());
    keyboardPane->setFont(paneFont);

    short width = 400, height = 500;
    keyboardPane->setGeometry(250, 250, width, height);

    QPushButton* backPushButton = new QPushButton("Back");
    backPushButton->setIcon(QIcon::fromTheme("go-previous"));
    keyboardPane->layout()->addWidget(backPushButton);

    QCheckBox* useCountryFlagCheckBox = new QCheckBox("Use country flag instead of layout code");
    layout->addWidget(useCountryFlagCheckBox);

    QHBoxLayout* manageLanguagesLayout = new QHBoxLayout;
    QVBoxLayout* buttonsLayout = new QVBoxLayout;

    QListWidget* activeLayoutsListWidget = new QListWidget;
    manageLanguagesLayout->addWidget(activeLayoutsListWidget);

    QPushButton* moveUpPushButton = new QPushButton("↑");  // up arrow
    QPushButton* moveDownPushButton = new QPushButton("↓");  // down arrow
    QPushButton* addLayoutPushButton = new QPushButton("+");
    QPushButton* removeLayoutPushButton = new QPushButton("-");

    buttonsLayout->addWidget(moveUpPushButton);
    buttonsLayout->addWidget(moveDownPushButton);
    buttonsLayout->addWidget(addLayoutPushButton);
    buttonsLayout->addWidget(removeLayoutPushButton);

    manageLanguagesLayout->addLayout(buttonsLayout);
    layout->addLayout(manageLanguagesLayout);

    QListWidget* toggleMethodsListWidget = new QListWidget;
    keyboardPane->layout()->addWidget(toggleMethodsListWidget);

    keyboardPane->layout()->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));

    QPushButton* revertPushButton = new QPushButton("Revert");
    keyboardPane->layout()->addWidget(revertPushButton);

    QPushButton* savePushButton = new QPushButton("Save");
    keyboardPane->layout()->addWidget(savePushButton);

    prepareUI(toggleMethodsListWidget);
    setCurrentSettings(useCountryFlagCheckBox, activeLayoutsListWidget,
                       toggleMethodsListWidget);


    LayoutDialog* layoutDg = new LayoutDialog;
    QWidget* layoutDgWidget = layoutDg->createUI(this, activeLayoutsListWidget);


    // Make connections
    keyboardPane->connect(savePushButton, &QPushButton::clicked, keyboardPane,
                  [this, useCountryFlagCheckBox,
                   activeLayoutsListWidget, toggleMethodsListWidget]() {
        saveSettings(useCountryFlagCheckBox, activeLayoutsListWidget,
                     toggleMethodsListWidget);
    });

    keyboardPane->connect(revertPushButton, &QPushButton::clicked, keyboardPane,
                  [this, useCountryFlagCheckBox, activeLayoutsListWidget,
                          toggleMethodsListWidget]() {
        setCurrentSettings(useCountryFlagCheckBox, activeLayoutsListWidget,
                           toggleMethodsListWidget);
    });

    keyboardPane->connect(backPushButton, &QPushButton::clicked, keyboardPane,
                  [keyboardPane, layoutDg, controlCenter]() {
        controlCenter->show();
        keyboardPane->hide();
        delete layoutDg;
        delete keyboardPane;
    });

    keyboardPane->connect(moveUpPushButton, &QPushButton::clicked, keyboardPane,
                          [activeLayoutsListWidget]() {
        if (!activeLayoutsListWidget->selectedItems().isEmpty()) {
            qint8 currentLayoutPosition = activeLayoutsListWidget->currentRow();
            if (currentLayoutPosition > 0) {
                activeLayoutsListWidget->insertItem(currentLayoutPosition - 1,
                                                    activeLayoutsListWidget->takeItem(currentLayoutPosition));
            }
        }
    });

    keyboardPane->connect(moveDownPushButton, &QPushButton::clicked, keyboardPane,
                          [activeLayoutsListWidget]() {
        if (!activeLayoutsListWidget->selectedItems().isEmpty()) {
            qint8 currentLayoutPosition = activeLayoutsListWidget->currentRow();
            if (currentLayoutPosition < (activeLayoutsListWidget->count() - 1)) {
                activeLayoutsListWidget->insertItem(currentLayoutPosition + 1,
                                                    activeLayoutsListWidget->takeItem(currentLayoutPosition));
            }
        }
    });

    keyboardPane->connect(addLayoutPushButton, &QPushButton::clicked, keyboardPane,
                          [layoutDgWidget]() {
        layoutDgWidget->show();
    });

    keyboardPane->connect(removeLayoutPushButton, &QPushButton::clicked, keyboardPane,
                          [activeLayoutsListWidget]() {
        if (!activeLayoutsListWidget->selectedItems().isEmpty()) {
            delete activeLayoutsListWidget->currentItem();
        }
    });

    return keyboardPane;
}
