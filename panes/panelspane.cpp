#include "panelspane.h"

#include "../dialogs/addappletdialog.h"
#include "../dialogs/appmenudialog.h"
#include "../dialogs/volumedialog.h"
#include "../dialogs/kbindicatordialog.h"
#include "../dialogs/datetimedialog.h"
#include "../dialogs/usermenudialog.h"
#include "../dialogs/windowlistdialog.h"
#include "../dialogs/localipv4dialog.h"
#include "../dialogs/workspacesdialog.h"
#include "../dialogs/launcherdialog.h"
#include "../dialogs/clioutputdialog.h"

void PanelsPane::setPaneContents() {
    mContentsWidget = new QWidget();
    QVBoxLayout* contentsLayout = new QVBoxLayout(mContentsWidget);
    mContentsWidget->setObjectName("innerPane");

    // Adding widgets
    QWidget* widget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout* panelsListLayout = new QVBoxLayout();

    QListWidget* panelsListWidget = new QListWidget();
    for (int i = 1; i <= getConfigValue("countPanels").toInt(); ++i) {
        QString cfgName = QString("panel%1").arg(QString::number(i));
        if (!getConfigValue(cfgName).isNull()) {
            QString name = QString(" Panel %1").arg(QString::number(i));
            QListWidgetItem* panelItem = new QListWidgetItem(name);
            panelItem->setIcon(QIcon::fromTheme("cs-applets"));
            panelsListWidget->addItem(panelItem);
        }
    }

    QString listWidgetStyle = QString("QListView::item:selected { "
                                      "background-color: %1; color: #ffffff; "
                                      "};").arg(getConfigValue("accent").toString());
    panelsListWidget->setStyleSheet(listWidgetStyle);
    panelsListWidget->setFont(mFont);

    panelsListLayout->addWidget(panelsListWidget);

    QVBoxLayout* listButtonsLayout = new QVBoxLayout();

    QPushButton* newPanelButton = new QPushButton(tr("+"));
    newPanelButton->setFont(mFont);
    listButtonsLayout->addWidget(newPanelButton);

    QPushButton* removePanelButton = new QPushButton(tr("-"));
    removePanelButton->setFont(mFont);
    listButtonsLayout->addWidget(removePanelButton);

    panelsListLayout->addLayout(listButtonsLayout);
    layout->addLayout(panelsListLayout);

    QWidget* currentPanelWidget = new QWidget();
    QVBoxLayout* panelLayout = new QVBoxLayout(currentPanelWidget);
    //panelLayout->setSpacing(10);

    QLabel* appletsLabel = new QLabel(tr("Applets"));
    appletsLabel->setFont(mTitleFont);
    panelLayout->addWidget(appletsLabel);

    QHBoxLayout* appletsLayout = new QHBoxLayout();
    appletsLayout->setContentsMargins(10, 1, 1, 1);

    QListWidget* appletsListWidget = new QListWidget();
    appletsListWidget->setStyleSheet(listWidgetStyle);
    appletsListWidget->setFont(mFont);
    appletsLayout->addWidget(appletsListWidget);

    QVBoxLayout* buttonsLayout = new QVBoxLayout();

    QPushButton* addAppletButton = new QPushButton("+");
    addAppletButton->setMaximumWidth(40);
    buttonsLayout->addWidget(addAppletButton);

    QPushButton* removeAppletButton = new QPushButton("-");
    removeAppletButton->setMaximumWidth(40);
    buttonsLayout->addWidget(removeAppletButton);

    QPushButton* moveUpButton = new QPushButton("⬆️");  // U+2B06 - Up arrow
    moveUpButton->setMaximumWidth(40);
    buttonsLayout->addWidget(moveUpButton);

    QPushButton* moveDownButton = new QPushButton("⬇");  // U+2B07 - Down arrow
    moveDownButton->setMaximumWidth(40);
    buttonsLayout->addWidget(moveDownButton);

    QPushButton* configureButton = new QPushButton();
    configureButton->setIcon(QIcon::fromTheme("preferences-system"));
    configureButton->setMaximumWidth(40);
    buttonsLayout->addWidget(configureButton);

    appletsLayout->addLayout(buttonsLayout);
    panelLayout->addLayout(appletsLayout);

    QLabel* autoHideLabel = new QLabel(tr("Auto Hide"));
    autoHideLabel->setFont(mTitleFont);
    panelLayout->addWidget(autoHideLabel);

    QVBoxLayout* autoHideLayout = new QVBoxLayout();
    autoHideLayout->setContentsMargins(10, 1, 1, 1);

    QCheckBox* enableAutoHideCheckBox = new QCheckBox(tr("Enable Auto Hide"));
    enableAutoHideCheckBox->setFont(mFont);
    autoHideLayout->addWidget(enableAutoHideCheckBox);

    QHBoxLayout* autoHideIntervalLayout = new QHBoxLayout();

    QLabel* autoHideIntervalLabel = new QLabel(tr("Auto Hide Interval"));
    autoHideIntervalLabel->setFont(mTitleFont);
    autoHideIntervalLayout->addWidget(autoHideIntervalLabel);

    QSpinBox* autoHideIntervalSpinBox = new QSpinBox();
    autoHideIntervalSpinBox->setFont(mFont);
    autoHideIntervalSpinBox->setMinimum(100);
    autoHideIntervalSpinBox->setMaximum(32000);
    autoHideIntervalLayout->addWidget(autoHideIntervalSpinBox);

    autoHideLayout->addLayout(autoHideIntervalLayout);

    panelLayout->addLayout(autoHideLayout);

    QLabel* geometryLabel = new QLabel(tr("Geometry"));
    geometryLabel->setFont(mTitleFont);
    panelLayout->addWidget(geometryLabel);

    QVBoxLayout* geometryLayout = new QVBoxLayout();
    geometryLayout->setContentsMargins(10, 1, 1, 1);

    QHBoxLayout* thicknessLayout = new QHBoxLayout();

    QLabel* thicknessLabel = new QLabel(tr("Thickness"));
    thicknessLabel->setFont(mTitleFont);
    thicknessLayout->addWidget(thicknessLabel);

    QSpinBox* thicknessSpinBox = new QSpinBox();
    thicknessSpinBox->setFont(mFont);
    thicknessSpinBox->setMinimum(0);
    thicknessSpinBox->setMaximum(256);
    thicknessLayout->addWidget(thicknessSpinBox);

    geometryLayout->addLayout(thicknessLayout);

    QHBoxLayout* screenLayout = new QHBoxLayout();

    QLabel* screenLabel = new QLabel(tr("Screen"));
    screenLabel->setFont(mTitleFont);
    screenLayout->addWidget(screenLabel);

    QComboBox* screenComboBox = new QComboBox();
    screenComboBox->setFont(mFont);
    screenComboBox->addItem(tr("(primary screen)"));
    foreach (QScreen* screen, qApp->screens()) {
        screenComboBox->addItem(screen->name());
    }
    QString comboBoxStyle = QString("QComboBox::item:selected { "
                                    "background-color: %1; color: #ffffff; "
                                    "};").arg(getConfigValue("accent").toString());
    screenComboBox->setStyleSheet(comboBoxStyle);
    screenLayout->addWidget(screenComboBox);

    geometryLayout->addLayout(screenLayout);

    QCheckBox* stretchCheckBox = new QCheckBox(tr("Stretch to the screen size"));
    stretchCheckBox->setFont(mFont);
    geometryLayout->addWidget(stretchCheckBox);

    QCheckBox* setOnCenterCheckBox = new QCheckBox(tr("Set on the center"));
    setOnCenterCheckBox->setFont(mFont);
    geometryLayout->addWidget(setOnCenterCheckBox);

    QHBoxLayout* axisShiftLayout = new QHBoxLayout();

    QLabel* shiftLabel = new QLabel(tr("Axis shift"));
    shiftLabel->setFont(mTitleFont);
    axisShiftLayout->addWidget(shiftLabel);

    QSpinBox* shiftSpinBox = new QSpinBox();
    shiftSpinBox->setFont(mFont);
    shiftSpinBox->setMinimum(0);
    shiftSpinBox->setMaximum(INT_MAX);
    axisShiftLayout->addWidget(shiftSpinBox);

    geometryLayout->addLayout(axisShiftLayout);

    QHBoxLayout* locationLayout = new QHBoxLayout();

    QLabel* locationLabel = new QLabel(tr("Location"));
    locationLabel->setFont(mTitleFont);
    locationLayout->addWidget(locationLabel);

    QRadioButton* topRadioButton = new QRadioButton(tr("Top"));
    topRadioButton->setFont(mFont);
    locationLayout->addWidget(topRadioButton);

    QRadioButton* bottomRadioButton = new QRadioButton(tr("Bottom"));
    bottomRadioButton->setFont(mFont);
    locationLayout->addWidget(bottomRadioButton);

    QRadioButton* leftRadioButton = new QRadioButton(tr("Left"));
    leftRadioButton->setFont(mFont);
    locationLayout->addWidget(leftRadioButton);

    QRadioButton* rightRadioButton = new QRadioButton(tr("Right"));
    rightRadioButton->setFont(mFont);
    locationLayout->addWidget(rightRadioButton);

    geometryLayout->addLayout(locationLayout);

    QHBoxLayout* launcherIconSizeLayout = new QHBoxLayout();

    QLabel* launcherIconSizeLabel = new QLabel(tr("Launcher Icon Size"));
    launcherIconSizeLabel->setFont(mTitleFont);
    launcherIconSizeLayout->addWidget(launcherIconSizeLabel);

    QSpinBox* launcherIconSizeSpinBox = new QSpinBox();
    launcherIconSizeSpinBox->setMinimum(0);
    launcherIconSizeSpinBox->setMaximum(256);
    launcherIconSizeSpinBox->setFont(mFont);
    launcherIconSizeLayout->addWidget(launcherIconSizeSpinBox);

    geometryLayout->addLayout(launcherIconSizeLayout);

    panelLayout->addLayout(geometryLayout);

    QLabel* bgrImageLabel = new QLabel(tr("Background Image"));
    bgrImageLabel->setFont(mTitleFont);
    panelLayout->addWidget(bgrImageLabel);

    QHBoxLayout* bgrImageLayout = new QHBoxLayout();
    bgrImageLayout->setContentsMargins(10, 1, 1, 1);

    QLineEdit* bgrImagePathLineEdit = new QLineEdit();
    bgrImagePathLineEdit->setFont(mFont);
    bgrImagePathLineEdit->setPlaceholderText(tr("Type path here..."));
    bgrImagePathLineEdit->setClearButtonEnabled(true);
    bgrImageLayout->addWidget(bgrImagePathLineEdit);

    QPushButton* chooseButton = new QPushButton("...");
    bgrImageLayout->addWidget(chooseButton);

    panelLayout->addLayout(bgrImageLayout);

    QLabel* layoutSettingsLabel = new QLabel(tr("Layout Settings"));
    layoutSettingsLabel->setFont(mTitleFont);
    panelLayout->addWidget(layoutSettingsLabel);

    QVBoxLayout* layoutSettingsLayout = new QVBoxLayout();
    layoutSettingsLayout->setContentsMargins(10, 1, 1, 1);

    QHBoxLayout* spacingLayout = new QHBoxLayout();

    QLabel* spacingLabel = new QLabel(tr("Spacing between applets"));
    spacingLabel->setFont(mTitleFont);
    spacingLayout->addWidget(spacingLabel);

    QSpinBox* spacingSpinBox = new QSpinBox();
    spacingSpinBox->setFont(mFont);
    spacingSpinBox->setMinimum(0);
    spacingSpinBox->setMaximum(256);
    spacingLayout->addWidget(spacingSpinBox);

    layoutSettingsLayout->addLayout(spacingLayout);

    QHBoxLayout* marginLayout = new QHBoxLayout();

    QLabel* marginLabel = new QLabel(tr("Margin"));
    marginLabel->setFont(mTitleFont);
    marginLayout->addWidget(marginLabel);

    QSpinBox* marginSpinBox = new QSpinBox();
    marginSpinBox->setFont(mFont);
    marginSpinBox->setMinimum(0);
    marginSpinBox->setMaximum(500);
    marginLayout->addWidget(marginSpinBox);

    layoutSettingsLayout->addLayout(marginLayout);
    panelLayout->addLayout(layoutSettingsLayout);

    QHBoxLayout* opacityLayout = new QHBoxLayout();

    QLabel* opacityLabel = new QLabel(tr("Opacity"));
    opacityLabel->setFont(mTitleFont);
    opacityLayout->addWidget(opacityLabel);

    QDoubleSpinBox* opacitySpinBox = new QDoubleSpinBox();
    opacitySpinBox->setFont(mFont);
    opacitySpinBox->setMinimum(0);
    opacitySpinBox->setMaximum(1);
    opacityLayout->addWidget(opacitySpinBox);

    panelLayout->addLayout(opacityLayout);

    mSaveButton = new QPushButton(tr("Save settings"));
    mSaveButton->setFont(mFont);
    panelLayout->addWidget(mSaveButton);
    mSaveButton->setVisible(false);

    layout->addWidget(currentPanelWidget);

    contentsLayout->addWidget(widget);
    //contentsLayout->setContentsMargins(0, 0, 0, 0);

    // Make connections
    connect(panelsListWidget, &QListWidget::itemSelectionChanged,
            this, [this, panelsListWidget, appletsListWidget, enableAutoHideCheckBox,
                   autoHideIntervalSpinBox, thicknessSpinBox,
                   screenComboBox, stretchCheckBox, setOnCenterCheckBox, shiftSpinBox,
                   topRadioButton, bottomRadioButton, leftRadioButton, rightRadioButton,
                   launcherIconSizeSpinBox, bgrImagePathLineEdit,
                   spacingSpinBox, marginSpinBox, opacitySpinBox]() {
        int id = panelsListWidget->currentItem()->text().split(' ').last().toInt();
        setPanelSettings(id, appletsListWidget, enableAutoHideCheckBox, autoHideIntervalSpinBox,
                         thicknessSpinBox, screenComboBox, stretchCheckBox, setOnCenterCheckBox,
                         shiftSpinBox, topRadioButton, bottomRadioButton, leftRadioButton,
                         rightRadioButton, launcherIconSizeSpinBox, bgrImagePathLineEdit,
                         spacingSpinBox, marginSpinBox, opacitySpinBox);

        QDBusConnection bus = QDBusConnection::sessionBus();
        QDBusMessage req = QDBusMessage::createMethodCall("org.plainDE.plainPanel",
                                                          "/Actions",
                                                          "org.plainDE.actions",
                                                          "highlightPanel");
        QList<QVariant> args = { id };
        req.setArguments(args);
        QDBusMessage resp = bus.call(req);
    });

    connect(moveUpButton, &QPushButton::clicked, this, [appletsListWidget, this]() {
        if (!appletsListWidget->selectedItems().isEmpty()) {
            QListWidgetItem* item = appletsListWidget->selectedItems()[0];
            int currentRow = appletsListWidget->currentRow();
            if (currentRow > 0) {
                appletsListWidget->takeItem(currentRow);
                appletsListWidget->insertItem(currentRow - 1, item);
            }
            appletsListWidget->setCurrentRow(currentRow - 1);
            mSaveButton->setVisible(true);
        }
    });

    connect(moveDownButton, &QPushButton::clicked, this, [appletsListWidget, this]() {
        if (!appletsListWidget->selectedItems().isEmpty()) {
            QListWidgetItem* item = appletsListWidget->selectedItems()[0];
            int currentRow = appletsListWidget->currentRow();
            if (currentRow < appletsListWidget->count() - 1) {
                appletsListWidget->takeItem(currentRow);
                appletsListWidget->insertItem(currentRow + 1, item);
            }
            appletsListWidget->setCurrentRow(currentRow + 1);
            mSaveButton->setVisible(true);
        }
    });

    connect(removeAppletButton, &QPushButton::clicked, this, [appletsListWidget, this]() {
        if (!appletsListWidget->selectedItems().isEmpty()) {
            QListWidgetItem* item = appletsListWidget->currentItem();
            if (item->text().startsWith("clioutput:")) {
                QString appletName = item->text().split(':').last();
                QString path = QDir::homePath() + "/.config/plainDE/clioutput-applets/" + appletName + ".json";
                QMessageBox msgBox(this);
                msgBox.setWindowTitle("Deleting CLI Output Applet");
                QString warning = QString("Do you also want to delete matching config file? (%1)\n"
                                          "This action cannot be undone.").arg(path);
                msgBox.setText(warning);
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                msgBox.setIcon(QMessageBox::Question);
                if (msgBox.exec() == QMessageBox::Yes) {
                    QFile file(path);
                    file.remove();
                }
            }
            delete item;
            mSaveButton->setVisible(true);
        }
    });

    connect(addAppletButton, &QPushButton::clicked, this, [this, appletsListWidget,
                                                           panelsListWidget]() {
        int panelID = panelsListWidget->currentItem()->text().split(' ').last().toInt();
        AddAppletDialog* dialog = new AddAppletDialog(mCfgObj, panelID, appletsListWidget);
        dialog->setPaneContents();
        dialog->show();
    });

    QAbstractItemModel* model = appletsListWidget->model();
    connect(model, &QAbstractItemModel::rowsInserted, this, [this]() {
        mSaveButton->setVisible(true);
    });

    connect(enableAutoHideCheckBox, &QCheckBox::stateChanged, this, [this, enableAutoHideCheckBox,
                                                                     autoHideIntervalSpinBox]() {
        mSaveButton->setVisible(true);
        autoHideIntervalSpinBox->setEnabled(enableAutoHideCheckBox->isChecked());
    });

    connect(autoHideIntervalSpinBox, static_cast<void(QSpinBox::*)(int)>(
                                  &QSpinBox::valueChanged), this, [this]() {
        mSaveButton->setVisible(true);
    });

    connect(thicknessSpinBox, static_cast<void(QSpinBox::*)(int)>(
                                  &QSpinBox::valueChanged), this, [this]() {
        mSaveButton->setVisible(true);
    });

    connect(screenComboBox, static_cast<void(QComboBox::*)(int)>(
                                &QComboBox::currentIndexChanged), this, [this]() {
        mSaveButton->setVisible(true);
    });

    connect(stretchCheckBox, &QCheckBox::stateChanged, this, [this, stretchCheckBox,
                                                              setOnCenterCheckBox]() {
        mSaveButton->setVisible(true);
        if (stretchCheckBox->isChecked()) {
            setOnCenterCheckBox->setEnabled(false);
            setOnCenterCheckBox->setChecked(false);
        }
        else {
            setOnCenterCheckBox->setEnabled(true);
        }
    });

    connect(setOnCenterCheckBox, &QCheckBox::stateChanged, this, [this]() {
        mSaveButton->setVisible(true);
    });

    connect(shiftSpinBox, static_cast<void(QSpinBox::*)(int)>(
                              &QSpinBox::valueChanged), this, [this]() {
        mSaveButton->setVisible(true);
    });

    connect(topRadioButton, &QRadioButton::clicked, this, [this]() {
        mSaveButton->setVisible(true);
    });

    connect(bottomRadioButton, &QRadioButton::clicked, this, [this]() {
        mSaveButton->setVisible(true);
    });

    connect(leftRadioButton, &QRadioButton::clicked, this, [this]() {
        mSaveButton->setVisible(true);
    });

    connect(rightRadioButton, &QRadioButton::clicked, this, [this]() {
        mSaveButton->setVisible(true);
    });

    connect(launcherIconSizeSpinBox, static_cast<void(QSpinBox::*)(int)>(
                                         &QSpinBox::valueChanged), this, [this]() {
        mSaveButton->setVisible(true);
    });

    connect(bgrImagePathLineEdit, &QLineEdit::textChanged, this, [this]() {
        mSaveButton->setVisible(true);
    });

    connect(chooseButton, &QPushButton::clicked, this, [this, bgrImagePathLineEdit]() {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        QDir::homePath(),
                                                        "Images (*.png *.jpg *.jpeg)");
        if (!fileName.isEmpty()) {
            bgrImagePathLineEdit->setText(fileName);
        }
    });

    connect(spacingSpinBox, static_cast<void(QSpinBox::*)(int)>(
                                &QSpinBox::valueChanged), this, [this]() {
        mSaveButton->setVisible(true);
    });

    connect(marginSpinBox, static_cast<void(QSpinBox::*)(int)>(
                               &QSpinBox::valueChanged), this, [this]() {
        mSaveButton->setVisible(true);
    });

    connect(opacitySpinBox, static_cast<void(QDoubleSpinBox::*)(double)>(
                                &QDoubleSpinBox::valueChanged), this, [this]() {
        mSaveButton->setVisible(true);
    });

    connect(mSaveButton, &QPushButton::clicked, this, [this, panelsListWidget, appletsListWidget, enableAutoHideCheckBox,
                                                       autoHideIntervalSpinBox, thicknessSpinBox, screenComboBox, stretchCheckBox,
                                                       setOnCenterCheckBox, shiftSpinBox, topRadioButton, bottomRadioButton,
                                                       leftRadioButton, rightRadioButton, launcherIconSizeSpinBox, bgrImagePathLineEdit,
                                                       spacingSpinBox, marginSpinBox, opacitySpinBox]() {
        int id = panelsListWidget->currentItem()->text().split(' ').last().toInt();
        prepareToSave(id, appletsListWidget, enableAutoHideCheckBox, autoHideIntervalSpinBox,
                      thicknessSpinBox, screenComboBox, stretchCheckBox, setOnCenterCheckBox,
                      shiftSpinBox, topRadioButton, bottomRadioButton, leftRadioButton,
                      rightRadioButton, launcherIconSizeSpinBox, bgrImagePathLineEdit,
                      spacingSpinBox, marginSpinBox, opacitySpinBox);
        saveConfig();
        mSaveButton->setVisible(false);
    });

    connect(configureButton, &QPushButton::clicked, this, [this, appletsListWidget, panelsListWidget]() {
        if (!appletsListWidget->selectedItems().isEmpty()) {
            QListWidgetItem* item = appletsListWidget->currentItem();
            QString text = item->text();
            if (!text.compare("App Menu")) {
                AppMenuDialog* dialog = new AppMenuDialog(mCfgObj);
                dialog->setPaneContents();
                dialog->show();
            }
            else if (!text.compare("Volume")) {
                VolumeDialog* dialog = new VolumeDialog(mCfgObj);
                dialog->setPaneContents();
                dialog->show();
            }
            else if (!text.compare("Keyboard Layout")) {
                KbIndicatorDialog* dialog = new KbIndicatorDialog(mCfgObj);
                dialog->setPaneContents();
                dialog->show();
            }
            else if (!text.compare("Date & Time")) {
                DateTimeDialog* dialog = new DateTimeDialog(mCfgObj);
                dialog->setPaneContents();
                dialog->show();
            }
            else if (!text.compare("User Menu")) {
                UserMenuDialog* dialog = new UserMenuDialog(mCfgObj);
                dialog->setPaneContents();
                dialog->show();
            }
            else if (!text.compare("Window List")) {
                WindowListDialog* dialog = new WindowListDialog(mCfgObj);
                dialog->setPaneContents();
                dialog->show();
            }
            else if (!text.compare("Local IPv4 Address")) {
                LocalIPv4Dialog* dialog = new LocalIPv4Dialog(mCfgObj);
                dialog->setPaneContents();
                dialog->show();
            }
            else if (!text.compare("Workspaces")) {
                WorkspacesDialog* dialog = new WorkspacesDialog(mCfgObj);
                dialog->setPaneContents();
                dialog->show();
            }
            else if (text.startsWith("launcher")) {
                int panelID = panelsListWidget->currentItem()->text().split(' ').last().toInt();
                LauncherDialog* dialog = new LauncherDialog(mCfgObj,
                                                            panelID,
                                                            false,
                                                            appletsListWidget->currentRow(),
                                                            appletsListWidget);
                dialog->setPaneContents();
                dialog->show();
            }
            else if (text.startsWith("clioutput")) {
                int panelID = panelsListWidget->currentItem()->text().split(' ').last().toInt();
                CLIOutputDialog* dialog = new CLIOutputDialog(mCfgObj,
                                                              panelID,
                                                              false,
                                                              appletsListWidget->currentRow(),
                                                              appletsListWidget);
                dialog->setPaneContents();
                dialog->show();
            }
            else {
                QMessageBox msg(this);
                msg.setWindowTitle("Information");
                msg.setText(tr("There are no aditional options for this applet."));
                msg.setStandardButtons(QMessageBox::Ok);
                msg.setIcon(QMessageBox::Information);
                msg.exec();
            }
        }
    });

    connect(newPanelButton, &QPushButton::clicked, this, [this, panelsListWidget]() {
        int lastNumber = 0;
        int final = panelsListWidget->count() + 1;
        bool orderOk = true;
        for (int i = 0; i < panelsListWidget->count(); ++i) {
            int panelID = panelsListWidget->item(i)->text().split(' ').last().toInt();
            if (panelID - lastNumber == 1) {
                lastNumber += 1;
            }
            else {
                final = i;
                orderOk = false;
                break;
            }
        }

        int newPanelID = (!orderOk) ? final + 1 : lastNumber + 1;

        QJsonObject newPanelObj;
        newPanelObj["applets"] = QJsonValue(QJsonArray());
        newPanelObj["thickness"] = QJsonValue(28);
        newPanelObj["screen"] = QJsonValue("");
        newPanelObj["expand"] = QJsonValue(true);
        newPanelObj["shift"] = QJsonValue(0);
        QHash<QString, bool> locations;
        locations["top"] = true;
        locations["bottom"] = true;
        locations["left"] = true;
        locations["right"] = true;
        for (int i = 1; i <= panelsListWidget->count(); ++i) {
            QString panelName = QString("panel%1").arg(QString::number(i));
            QString location = getConfigValue(panelName, "location").toString();
            locations[location] = false;
        }
        QString freeLocation = locations.key(true, "top");
        newPanelObj["location"] = QJsonValue(freeLocation);
        newPanelObj["launcherIconSize"] = QJsonValue(22);
        newPanelObj["spacing"] = QJsonValue(5);
        newPanelObj["margin"] = QJsonValue(5);
        newPanelObj["opacity"] = QJsonValue(0.85);

        QString newPanelName = QString("panel%1").arg(QString::number(newPanelID));
        setEntry(newPanelName, newPanelObj);

        panelsListWidget->clear();
        for (int i = 1; i <= getConfigValue("countPanels").toInt(); ++i) {
            QString cfgName = QString("panel%1").arg(QString::number(i));
            if (!getConfigValue(cfgName).isNull()) {
                QString name = QString(" Panel %1").arg(QString::number(i));
                QListWidgetItem* panelItem = new QListWidgetItem(name);
                panelItem->setIcon(QIcon::fromTheme("cs-applets"));
                panelsListWidget->addItem(panelItem);
            }
        }

        saveConfig();
    });

    connect(removePanelButton, &QPushButton::clicked, this, [this, panelsListWidget]() {
        if (!panelsListWidget->selectedItems().isEmpty() && panelsListWidget->count() > 1) {
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("Removing panel");
            msgBox.setText("Are you sure you want to remove selected panel? "
                           "This action cannot be undone.");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setIcon(QMessageBox::Question);

            if (msgBox.exec() == QMessageBox::Yes) {
                int panelID = panelsListWidget->currentItem()->text().split(' ').last().toInt();
                QString panelName = QString("panel%1").arg(QString::number(panelID));
                setEntry(panelName, QJsonValue::Null);

                panelsListWidget->clear();
                for (int i = 1; i <= getConfigValue("countPanels").toInt(); ++i) {
                    QString cfgName = QString("panel%1").arg(QString::number(i));
                    if (!getConfigValue(cfgName).isNull()) {
                        QString name = QString(" Panel %1").arg(QString::number(i));
                        QListWidgetItem* panelItem = new QListWidgetItem(name);
                        panelItem->setIcon(QIcon::fromTheme("cs-applets"));
                        panelsListWidget->addItem(panelItem);
                    }
                }

                saveConfig();
            }
        }
    });

    finalizeUI();
}

void PanelsPane::setPanelSettings(int panelID,
                                  QListWidget* appletsListWidget,
                                  QCheckBox* enableAutoHideCheckBox,
                                  QSpinBox* autoHideIntervalSpinBox,
                                  QSpinBox* thicknessSpinBox,
                                  QComboBox* screenComboBox,
                                  QCheckBox* stretchCheckBox,
                                  QCheckBox* setOnCenterCheckBox,
                                  QSpinBox* shiftSpinBox,
                                  QRadioButton* topRadioButton,
                                  QRadioButton* bottomRadioButton,
                                  QRadioButton* leftRadioButton,
                                  QRadioButton* rightRadioButton,
                                  QSpinBox* launcherIconSize,
                                  QLineEdit* bgrImgPathLineEdit,
                                  QSpinBox* spacingSpinBox,
                                  QSpinBox* marginSpinBox,
                                  QDoubleSpinBox* opacitySpinBox) {
    QString panelName = QString("panel%1").arg(QString::number(panelID));

    // Applets
    appletsListWidget->clear();
    QJsonArray appletsList = getConfigValue(panelName, "applets").toArray();
    foreach (QJsonValue applet, appletsList) {
        if (applet.toString().startsWith("launcher")) {
            QListWidgetItem* item = new QListWidgetItem();

            QString launcherData = applet.toString();
            if (launcherData.endsWith(".desktop")) {
                // Desktop Entry
                QString entry = launcherData.split(':').at(1);

                if (QFile::exists("/usr/share/applications/" + entry)) {
                    entry = "/usr/share/applications/" + entry;
                }
                else {
                    QString homeDir = QDir::homePath();
                    entry = homeDir + "/.local/share/applications/" + entry;
                }

                QString iconPath;

                QSettings desktopFileReader(entry, QSettings::IniFormat);
                desktopFileReader.sync();
                desktopFileReader.beginGroup("Desktop Entry");
                    iconPath = desktopFileReader.value("Icon").toString();
                desktopFileReader.endGroup();

                item->setText(applet.toString());

                if (QIcon::hasThemeIcon(iconPath)) {
                    item->setIcon(QIcon::fromTheme(iconPath));
                }
                else if (QFile::exists(iconPath)) {
                    item->setIcon(QIcon(iconPath));
                }
                else {
                    item->setIcon(QIcon::fromTheme("dialog-question"));
                }
            }
            else {
                // Arbitrary executable
                QString iconPath = launcherData.split(':')[2];
                if (QIcon::hasThemeIcon(iconPath)) {
                    item->setIcon(QIcon::fromTheme(iconPath));
                }
                else if (QFile::exists(iconPath)) {
                    item->setIcon(QIcon(iconPath));
                }
                else {
                    item->setIcon(QIcon::fromTheme("dialog-question"));
                }
                item->setText(applet.toString());
            }

            appletsListWidget->addItem(item);
        }
        else if (applet.toString().startsWith("clioutput")) {
            QListWidgetItem* item = new QListWidgetItem(applet.toString());
            item->setIcon(QIcon::fromTheme(mIconNameByApplet["clioutput"]));
            appletsListWidget->addItem(item);
        }
        else {  // All other applets
            QString text = mAppletHumanNames[applet.toString()];
            QListWidgetItem* item = new QListWidgetItem(text);
            item->setIcon(QIcon::fromTheme(mIconNameByApplet[applet.toString()]));
            appletsListWidget->addItem(item);
        }
    }

    // Auto Hide
    enableAutoHideCheckBox->setChecked(getConfigValue(panelName, "enableAutoHide").toBool());
    autoHideIntervalSpinBox->setValue(getConfigValue(panelName, "autoHideInterval").toInt());
    if (enableAutoHideCheckBox->isChecked()) {
        autoHideIntervalSpinBox->setEnabled(true);
    }
    else {
        autoHideIntervalSpinBox->setEnabled(false);
    }

    // Thickness
    thicknessSpinBox->setValue(getConfigValue(panelName, "thickness").toInt());

    // Screen
    QString screen = getConfigValue(panelName, "screen").toString();
    int index = screenComboBox->findText(screen);
    if (index != -1) {
        screenComboBox->setCurrentIndex(index);
    }
    else {
        screenComboBox->setCurrentIndex(0);
    }

    // Stretch to the screen size
    stretchCheckBox->setChecked(getConfigValue(panelName, "expand").toBool());

    // Set On Center
    setOnCenterCheckBox->setChecked(getConfigValue(panelName, "setOnCenter").toBool());
    if (!stretchCheckBox->isChecked()) {
        setOnCenterCheckBox->setEnabled(true);
    }
    else {
        setOnCenterCheckBox->setEnabled(false);
        setOnCenterCheckBox->setChecked(false);
    }

    // Axis shift
    shiftSpinBox->setValue(getConfigValue(panelName, "shift").toInt());

    // Location
    QString location = getConfigValue(panelName, "location").toString();
    if (!location.compare("top")) {
        topRadioButton->setChecked(true);
    }
    else if (!location.compare("bottom")) {
        bottomRadioButton->setChecked(true);
    }
    else if (!location.compare("left")) {
        leftRadioButton->setChecked(true);
    }
    else if (!location.compare("right")) {
        rightRadioButton->setChecked(true);
    }
    else {  // Unknown option
        topRadioButton->setChecked(true);
    }

    // Launcher Icon Size
    launcherIconSize->setValue(getConfigValue(panelName, "launcherIconSize").toInt());

    // Background Image
    bgrImgPathLineEdit->setText(getConfigValue(panelName, "backgroundImage").toString());

    // Spacing
    spacingSpinBox->setValue(getConfigValue(panelName, "spacing").toInt());

    // Margin
    marginSpinBox->setValue(getConfigValue(panelName, "margin").toInt());

    // Opacity
    opacitySpinBox->setValue(getConfigValue(panelName, "opacity").toDouble());

    mSaveButton->setVisible(false);
}

void PanelsPane::prepareToSave(int panelID,
                               QListWidget* appletsListWidget,
                               QCheckBox* enableAutoHideCheckBox,
                               QSpinBox* autoHideIntervalSpinBox,
                               QSpinBox* thicknessSpinBox,
                               QComboBox* screenComboBox,
                               QCheckBox* stretchCheckBox,
                               QCheckBox* setOnCenterCheckBox,
                               QSpinBox* shiftSpinBox,
                               QRadioButton* topRadioButton,
                               QRadioButton* bottomRadioButton,
                               QRadioButton* leftRadioButton,
                               QRadioButton* rightRadioButton,
                               QSpinBox* launcherIconSizeSpinBox,
                               QLineEdit* bgrImgPathLineEdit,
                               QSpinBox* spacingSpinBox,
                               QSpinBox* marginSpinBox,
                               QDoubleSpinBox* opacitySpinBox) {
    QJsonObject panelObject;

    // Applets
    QJsonArray applets;
    for (int i = 0; i < appletsListWidget->count(); ++i) {
        QListWidgetItem* item = appletsListWidget->item(i);
        QString itemName = item->text();
        QString finalName;
        if (itemName.startsWith("launcher:") || itemName.startsWith("clioutput:")) {
            finalName = itemName;
        }
        else {  // All other applets
            finalName = mAppletHumanNames.key(itemName);
        }
        applets.append(QJsonValue(finalName));
    }
    panelObject["applets"] = QJsonValue(applets);

    // Auto Hide
    panelObject["enableAutoHide"] = QJsonValue(enableAutoHideCheckBox->isChecked());
    panelObject["autoHideInterval"] = QJsonValue(autoHideIntervalSpinBox->value());

    // Thickness
    panelObject["thickness"] = QJsonValue(thicknessSpinBox->value());

    // Screen
    if (screenComboBox->currentIndex() == 0) {  // (primary screen)
        panelObject["screen"] = QJsonValue("");
    }
    else {
        panelObject["screen"] = QJsonValue(screenComboBox->currentText());
    }

    // Stretch
    panelObject["expand"] = QJsonValue(stretchCheckBox->isChecked());

    // Set On Center
    panelObject["setOnCenter"] = QJsonValue(setOnCenterCheckBox->isChecked());

    // Axis shift
    panelObject["shift"] = QJsonValue(shiftSpinBox->value());

    // Location
    QString location;
    if (topRadioButton->isChecked()) {
        location = "top";
    }
    else if (bottomRadioButton->isChecked()) {
        location = "bottom";
    }
    else if (leftRadioButton->isChecked()) {
        location = "left";
    }
    else if (rightRadioButton->isChecked()) {
        location = "right";
    }
    else {
        location = "top";
    }
    panelObject["location"] = QJsonValue(location);

    // Launcher Icon Size
    panelObject["launcherIconSize"] = QJsonValue(launcherIconSizeSpinBox->value());

    // Background Image
    panelObject["backgroundImage"] = QJsonValue(bgrImgPathLineEdit->text());

    // Spacing
    panelObject["spacing"] = QJsonValue(spacingSpinBox->value());

    // Margin
    panelObject["margin"] = QJsonValue(marginSpinBox->value());

    // Opacity
    panelObject["opacity"] = QJsonValue(opacitySpinBox->value());


    QString panelName = QString("panel%1").arg(QString::number(panelID));
    setEntry(panelName, panelObject);
}

PanelsPane::PanelsPane(QJsonObject* cfgObj) : Pane(nullptr,
                                                   cfgObj,
                                                   tr("Panels"),
                                                   "cs-applets") {
    mAppletHumanNames["appmenu"] = tr("App Menu");
    mAppletHumanNames["datetime"] = tr("Date & Time");
    mAppletHumanNames["kblayout"] = tr("Keyboard Layout");
    mAppletHumanNames["localipv4"] = tr("Local IPv4 Address");
    mAppletHumanNames["battery"] = tr("Battery");
    mAppletHumanNames["sni"] = tr("Tray (StatusNotifierItem)");
    mAppletHumanNames["mpris"] = tr("Playback Control (MPRIS)");
    mAppletHumanNames["usermenu"] = tr("User Menu");
    mAppletHumanNames["volume"] = tr("Volume");
    mAppletHumanNames["windowlist"] = tr("Window List");
    mAppletHumanNames["workspaces"] = tr("Workspaces");
    mAppletHumanNames["spacer"] = tr("Spacer");
    mAppletHumanNames["splitter"] = tr("Splitter");

    mIconNameByApplet["appmenu"] = "app-launcher";
    mIconNameByApplet["datetime"] = "calendar";
    mIconNameByApplet["kblayout"] = "keyboard";
    mIconNameByApplet["localipv4"] = "cs-network";
    mIconNameByApplet["battery"] = "extensions";
    mIconNameByApplet["sni"] = "extensions";
    mIconNameByApplet["mpris"] = "preferences-system-sound";
    mIconNameByApplet["usermenu"] = "preferences-desktop-user";
    mIconNameByApplet["volume"] = "preferences-system-sound";
    mIconNameByApplet["windowlist"] = "preferences-system-windows";
    mIconNameByApplet["workspaces"] = "workspace-switcher-top-left";
    mIconNameByApplet["spacer"] = "extensions";
    mIconNameByApplet["splitter"] = "extensions";
    mIconNameByApplet["launcher"] = "utilities-terminal";
    mIconNameByApplet["clioutput"] = "utilities-terminal";
}
