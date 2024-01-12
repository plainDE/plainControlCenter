#include "clioutputdialog.h"

void CLIOutputDialog::readConfig() {
    QString appletName = mAppletsListWidget->item(mItem)->text().split(':').last();
    QString appletPath = QDir::homePath() + "/.config/plainDE/clioutput-applets/" + appletName + ".json";

    QFile file(appletPath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString data = file.readAll();
    file.close();
    mAppletObj = QJsonDocument::fromJson(data.toUtf8()).object();
}

void CLIOutputDialog::newCondition(QBoxLayout* innerConditionsLayout,
                                   QString ifValue,
                                   QString thenIcon,
                                   QString thenText) {
    QVBoxLayout* newConditionLayout = new QVBoxLayout();

    QLabel* ifLabel = new QLabel(tr("If"));
    ifLabel->setFont(mFont);
    newConditionLayout->addWidget(ifLabel);

    QLineEdit* ifLineEdit = new QLineEdit();
    ifLineEdit->setFont(mFont);
    ifLineEdit->setPlaceholderText(tr("Type condition here..."));
    ifLineEdit->setText(ifValue);
    newConditionLayout->addWidget(ifLineEdit);

    QLabel* thenLabel = new QLabel(tr("Then"));
    thenLabel->setFont(mFont);
    newConditionLayout->addWidget(thenLabel);

    QLineEdit* iconLineEdit = new QLineEdit();
    iconLineEdit->setFont(mFont);
    iconLineEdit->setPlaceholderText(tr("Type icon name or path here..."));
    iconLineEdit->setText(thenIcon);
    newConditionLayout->addWidget(iconLineEdit);

    QLineEdit* textLineEdit = new QLineEdit();
    textLineEdit->setFont(mFont);
    textLineEdit->setPlaceholderText(tr("Type text here..."));
    textLineEdit->setText(thenText);
    newConditionLayout->addWidget(textLineEdit);

    QPushButton* rmButton = new QPushButton(tr("Remove"));
    rmButton->setFont(mFont);
    newConditionLayout->addWidget(rmButton);

    innerConditionsLayout->addLayout(newConditionLayout);

    Condition* cond = new Condition();
    cond->i = mConditionList.count();
    cond->ifLabel = ifLabel;
    cond->condLineEdit = ifLineEdit;
    cond->thenLabel = thenLabel;
    cond->iconLineEdit = iconLineEdit;
    cond->textLineEdit = textLineEdit;
    cond->rmButton = rmButton;
    mConditionList.append(cond);

    connect(rmButton, &QPushButton::clicked, this, [this, cond]() {
        qDebug() << mConditionList;
        mConditionList[cond->i] = NULL;
        delete cond->ifLabel;
        delete cond->condLineEdit;
        delete cond->thenLabel;
        delete cond->iconLineEdit;
        delete cond->textLineEdit;
        delete cond->rmButton;
    });
}

void CLIOutputDialog::setPaneContents() {
    mContentsWidget = new QWidget();
    QVBoxLayout* contentsLayout = new QVBoxLayout(mContentsWidget);
    mContentsWidget->setObjectName("innerPane");

    // Adding widgets
    QLabel* appletNameLabel = new QLabel(tr("Applet Name"));
    appletNameLabel->setFont(mTitleFont);
    contentsLayout->addWidget(appletNameLabel);

    QLineEdit* appletNameLineEdit = new QLineEdit();
    appletNameLineEdit->setFont(mFont);
    appletNameLineEdit->setPlaceholderText(tr("Type applet name here..."));
    contentsLayout->addWidget(appletNameLineEdit);

    QGroupBox* appletTypeGroupBox = new QGroupBox(tr("Applet Type"));
    appletTypeGroupBox->setFont(mTitleFont);
    QVBoxLayout* appletTypeLayout = new QVBoxLayout(appletTypeGroupBox);

    QRadioButton* stdoutRadioButton = new QRadioButton("Stdout");
    stdoutRadioButton->setFont(mFont);
    stdoutRadioButton->setChecked(true);
    stdoutRadioButton->setToolTip(tr("Just shows output of a command"));
    appletTypeLayout->addWidget(stdoutRadioButton);

    QRadioButton* conditionRadioButton = new QRadioButton(tr("Condition"));
    conditionRadioButton->setFont(mFont);
    conditionRadioButton->setToolTip(tr("Compares output or exit code of a command with "
                                        "preset options and shows data depending on them"));
    appletTypeLayout->addWidget(conditionRadioButton);

    QRadioButton* dataRadioButton = new QRadioButton(tr("Data"));
    dataRadioButton->setFont(mFont);
    dataRadioButton->setToolTip(tr("Interprets output of a command as data for QPushButton "
                                   "in format IconNameOrPath;Text \n"
                                   "(e.g. /usr/share/icons/thermometer.png;CPU 50°C)"));
    appletTypeLayout->addWidget(dataRadioButton);

    contentsLayout->addWidget(appletTypeGroupBox);

    QLabel* commandLabel = new QLabel(tr("Command"));
    commandLabel->setFont(mTitleFont);
    contentsLayout->addWidget(commandLabel);

    QLineEdit* commandLineEdit = new QLineEdit();
    commandLineEdit->setFont(mFont);
    commandLineEdit->setPlaceholderText(tr("Type your command here..."));
    contentsLayout->addWidget(commandLineEdit);

    QLabel* intervalLabel = new QLabel(tr("Update interval (ms)"));
    intervalLabel->setFont(mTitleFont);
    contentsLayout->addWidget(intervalLabel);

    QSpinBox* intervalSpinBox = new QSpinBox();
    intervalSpinBox->setFont(mFont);
    intervalSpinBox->setMinimum(50);
    intervalSpinBox->setMaximum(INT_MAX);
    contentsLayout->addWidget(intervalSpinBox);

    QGroupBox* conditionTypeGroupBox = new QGroupBox(tr("Condition type"));
    conditionTypeGroupBox->setFont(mTitleFont);
    QVBoxLayout* conditionTypeLayout = new QVBoxLayout(conditionTypeGroupBox);

    QRadioButton* stdoutCondRadioButton = new QRadioButton("Stdout");
    stdoutCondRadioButton->setFont(mFont);
    conditionTypeLayout->addWidget(stdoutCondRadioButton);

    QRadioButton* exitCodeCondRadioButton = new QRadioButton(tr("Exit code"));
    exitCodeCondRadioButton->setFont(mFont);
    conditionTypeLayout->addWidget(exitCodeCondRadioButton);

    contentsLayout->addWidget(conditionTypeGroupBox);

    QGroupBox* conditionsGroupBox = new QGroupBox(tr("Conditions"));
    conditionsGroupBox->setFont(mTitleFont);
    QVBoxLayout* conditionsLayout = new QVBoxLayout(conditionsGroupBox);

    QVBoxLayout* innerConditionsLayout = new QVBoxLayout();
    conditionsLayout->addLayout(innerConditionsLayout);

    QPushButton* newConditionButton = new QPushButton(tr("New condition"));
    newConditionButton->setFont(mFont);
    conditionsLayout->addWidget(newConditionButton);

    contentsLayout->addWidget(conditionsGroupBox);

    QGroupBox* elseGroupBox = new QGroupBox(tr("Else"));
    elseGroupBox->setFont(mTitleFont);

    QVBoxLayout* elseLayout = new QVBoxLayout(elseGroupBox);

    QLineEdit* elseIconLineEdit = new QLineEdit();
    elseIconLineEdit->setFont(mFont);
    elseIconLineEdit->setPlaceholderText(tr("Type icon name or path here..."));
    elseLayout->addWidget(elseIconLineEdit);

    QLineEdit* elseTextLineEdit = new QLineEdit();
    elseTextLineEdit->setFont(mFont);
    elseTextLineEdit->setPlaceholderText(tr("Type text here..."));
    elseLayout->addWidget(elseTextLineEdit);

    contentsLayout->addWidget(elseGroupBox);

    QLabel* waitDataLabel = new QLabel(tr("Data shown while waiting for output:"));
    waitDataLabel->setFont(mTitleFont);
    contentsLayout->addWidget(waitDataLabel);

    QLineEdit* waitIconLineEdit = new QLineEdit();
    waitIconLineEdit->setFont(mFont);
    waitIconLineEdit->setPlaceholderText(tr("Type icon name or path here..."));
    contentsLayout->addWidget(waitIconLineEdit);

    QLineEdit* waitTextLineEdit = new QLineEdit();
    waitTextLineEdit->setFont(mFont);
    waitTextLineEdit->setPlaceholderText(tr("Type text here..."));
    contentsLayout->addWidget(waitTextLineEdit);

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

    // Default
    conditionTypeGroupBox->setVisible(false);
    conditionsGroupBox->setVisible(false);
    elseGroupBox->setVisible(false);

    // Setting current settings
    if (!mNewApplet) {
        // Name
        appletNameLineEdit->setText(mAppletsListWidget->item(mItem)->text().split(':').last());

        // Type
        if (!mAppletObj["type"].toString().compare("stdout")) {
            stdoutRadioButton->setChecked(true);
        }
        else if (!mAppletObj["type"].toString().compare("condition")) {
            conditionRadioButton->setChecked(true);
        }
        else {  // data
            dataRadioButton->setChecked(true);
        }

        // Command
        commandLineEdit->setText(mAppletObj["command"].toString());

        // Interval
        intervalSpinBox->setValue(mAppletObj["interval"].toInt());

        // Condition Type (if applicable)
        if (!mAppletObj["type"].toString().compare("condition")) {
            if (!mAppletObj["conditionType"].toString().compare("stdout")) {
                stdoutCondRadioButton->setChecked(true);
            }
            else {  // exitcode
                exitCodeCondRadioButton->setChecked(true);
            }
        }

        // Conditions (if applicable)
        if (!mAppletObj["type"].toString().compare("condition")) {
            conditionTypeGroupBox->setVisible(true);
            conditionsGroupBox->setVisible(true);
            elseGroupBox->setVisible(true);

            QJsonObject conditions = mAppletObj["conditions"].toObject();
            foreach (QString key, conditions.keys()) {
                QStringList then = conditions[key].toString().split(';');
                QString icon = then[0];
                QString text = then[1];
                newCondition(innerConditionsLayout, key, icon, text);
            }
        }

        // Else Condition (if applicable)
        if (!mAppletObj["type"].toString().compare("condition")) {
            QStringList elseData = mAppletObj["elseCondition"].toString().split(';');
            elseIconLineEdit->setText(elseData[0]);
            elseTextLineEdit->setText(elseData[1]);
        }

        // Wait Data
        QStringList waitData = mAppletObj["waitData"].toString().split(';');
        waitIconLineEdit->setText(waitData[0]);
        waitTextLineEdit->setText(waitData[1]);
    }

    // Making connections
    connect(newConditionButton, &QPushButton::clicked, this, [this, innerConditionsLayout]() {
        newCondition(innerConditionsLayout, "", "", "");
    });

    connect(stdoutRadioButton, &QRadioButton::clicked, this, [this, conditionTypeGroupBox,
                                                              conditionsGroupBox,
                                                              elseGroupBox]() {
        conditionTypeGroupBox->setVisible(false);
        conditionsGroupBox->setVisible(false);
        elseGroupBox->setVisible(false);
    });

    connect(conditionRadioButton, &QRadioButton::clicked, this, [this, conditionTypeGroupBox,
                                                                 conditionsGroupBox,
                                                                 elseGroupBox]() {
        conditionTypeGroupBox->setVisible(true);
        conditionsGroupBox->setVisible(true);
        elseGroupBox->setVisible(true);
    });

    connect(dataRadioButton, &QRadioButton::clicked, this, [this, conditionTypeGroupBox,
                                                            conditionsGroupBox,
                                                            elseGroupBox]() {
        conditionTypeGroupBox->setVisible(false);
        conditionsGroupBox->setVisible(false);
        elseGroupBox->setVisible(false);
    });

    connect(cancelButton, &QPushButton::clicked, this, [this]() {
        this->hide();
    });

    connect(okButton, &QPushButton::clicked, this, [this, appletNameLineEdit, stdoutRadioButton,
                                                    conditionRadioButton, commandLineEdit,
                                                    intervalSpinBox, stdoutCondRadioButton,
                                                    elseIconLineEdit, elseTextLineEdit,
                                                    waitIconLineEdit, waitTextLineEdit]() {
        prepareToSave(appletNameLineEdit, stdoutRadioButton,
             conditionRadioButton, commandLineEdit,
             intervalSpinBox, stdoutCondRadioButton,
             elseIconLineEdit, elseTextLineEdit,
             waitIconLineEdit, waitTextLineEdit);
        saveConfig();
        this->hide();
    });

    finalizeUI();
    setWindowGeometry();
    setTransparency(this);
}

void CLIOutputDialog::prepareToSave(QLineEdit* appletNameLineEdit,
                           QRadioButton* stdoutRadioButton,
                           QRadioButton* conditionRadioButton,
                           QLineEdit* commandLineEdit,
                           QSpinBox* intervalSpinBox,
                           QRadioButton* stdoutCondRadioButton,
                           QLineEdit* elseIconLineEdit,
                           QLineEdit* elseTextLineEdit,
                           QLineEdit* waitIconLineEdit,
                           QLineEdit* waitTextLineEdit) {
    // Type
    QString type;
    if (stdoutRadioButton->isChecked()) {
        type = "stdout";
    }
    else if (conditionRadioButton->isChecked()) {
        type = "condition";
    }
    else {  // dataRadioButton is checked
        type = "data";
    }
    mAppletObj["type"] = QJsonValue(type);

    // Command
    mAppletObj["command"] = QJsonValue(commandLineEdit->text());

    // Interval
    mAppletObj["interval"] = QJsonValue(intervalSpinBox->value());

    // Condition Type (if applicable)
    if (!type.compare("condition")) {
        QString conditionType;
        if (stdoutCondRadioButton->isChecked()) {
            conditionType = "stdout";
        }
        else {  // exitCodeCondRadioButton is checked
            conditionType = "exitcode";
        }
        mAppletObj["conditionType"] = QJsonValue(conditionType);
    }

    // Conditions (if applicable)
    if (!type.compare("condition")) {
        QJsonObject conditions;
        for (int i = 0; i < mConditionList.count(); ++i) {
            if (mConditionList.at(i) != NULL) {
                QString ifValue = mConditionList.at(i)->condLineEdit->text();
                QString thenIcon = mConditionList.at(i)->iconLineEdit->text();
                QString thenText = mConditionList.at(i)->textLineEdit->text();
                QString thenValue = thenIcon + ";" + thenText;
                conditions[ifValue] = QJsonValue(thenValue);
            }
        }
        mAppletObj["conditions"] = QJsonValue(conditions);
    }

    // Else condition (if applicable)
    if (!type.compare("condition")) {
        QString elseIcon = elseIconLineEdit->text();
        QString elseText = elseTextLineEdit->text();
        QString elseValue = elseIcon + ";" + elseText;
        mAppletObj["elseCondition"] = QJsonValue(elseValue);
    }

    // Wait Data
    QString waitIcon = waitIconLineEdit->text();
    QString waitText = waitTextLineEdit->text();
    QString waitData = waitIcon + ";" + waitText;
    mAppletObj["waitData"] = QJsonValue(waitData);


    QString appletsDirPath = QDir::homePath() + "/.config/plainDE/clioutput-applets/";
    QDir appletsDir(appletsDirPath);
    if (!appletsDir.exists()) {
        appletsDir.mkdir(appletsDirPath);
    }
    QString name = appletNameLineEdit->text();
    QString path = appletsDirPath + name + ".json";

    QJsonDocument doc(mAppletObj);
    QFile file(path);
    file.open(QFile::WriteOnly);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    QString applet = "clioutput:" + name;
    if (mNewApplet) {
        QListWidgetItem* item = new QListWidgetItem(QIcon::fromTheme("utilities-terminal"),
                                                    applet);
        mAppletsListWidget->addItem(item);
    }
    else {
        mAppletsListWidget->item(mItem)->setText(applet);
    }
}

CLIOutputDialog::CLIOutputDialog(QJsonObject* cfgObj,
                                 int panelID,
                                 bool newApplet,
                                 int item,
                                 QListWidget* appletsListWidget) : Dialog(cfgObj,
                                                                          tr("CLI Output applet settings"),
                                                                          "utilities-terminal") {
    mNewApplet = newApplet;
    mAppletsListWidget = appletsListWidget;
    mItem = item;
    if (!newApplet) {
        readConfig();
    }
}

CLIOutputDialog::~CLIOutputDialog() {
    foreach (Condition* cond, mConditionList) {
        if (cond != NULL) {
            delete cond;
        }
    }
    mConditionList.clear();
}
