#include "clioutput.h"

QJsonObject cliOutputAppletPaneConfig;

void CLIOutputAppletPane::readConfig() {
    QString configPath = QString("%1/.config/plainDE/config.json").arg(QDir::homePath());
    QFile file(configPath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString data = file.readAll();
    file.close();
    cliOutputAppletPaneConfig = QJsonDocument::fromJson(data.toUtf8()).object();
}

void CLIOutputAppletPane::createUI(Settings* controlCenter) {
    readConfig();

    // UI
    mCLIOutputAppletPaneWidget = new QWidget();
    mCLIOutputAppletPaneWidget->setObjectName("cliOutputAppletPane");
    mCLIOutputAppletPaneWidget->setWindowTitle("plainControlCenter");
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(4, 4, 4, 4);
    mCLIOutputAppletPaneWidget->setLayout(layout);


    // Theme
    QString themeName = cliOutputAppletPaneConfig["theme"].toString();
    QString stylesheetPath = QString("/usr/share/plainDE/styles/%1").arg(themeName);

    QFile stylesheetReader(stylesheetPath);
    stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
    QString data = stylesheetReader.readAll();
    QTextStream styleSheet(&stylesheetReader);
    mCLIOutputAppletPaneWidget->setStyleSheet(styleSheet.readAll());
    stylesheetReader.close();


    // Font
    /*QFont paneFont;
    paneFont.setFamily(cliOutputAppletPaneConfig["fontFamily"].toString());
    paneFont.setPointSize(cliOutputAppletPaneConfig["fontSize"].toInt());
    mCLIOutputAppletPaneWidget->setFont(paneFont);*/


    // Window geometry
    short width = 400, height = 500;
    mCLIOutputAppletPaneWidget->setGeometry(650, 250, width, height);


    // UI
    QPushButton* backPushButton = new QPushButton("Close");
    backPushButton->setIcon(QIcon::fromTheme("go-previous"));
    layout->addWidget(backPushButton);


    QHBoxLayout* nameLayout = new QHBoxLayout();

    QLabel* nameLabel = new QLabel("Name");
    nameLayout->addWidget(nameLabel);

    QLineEdit* nameLineEdit = new QLineEdit();
    nameLayout->addWidget(nameLineEdit);

    QLabel* jsonLabel = new QLabel(".json");
    nameLayout->addWidget(jsonLabel);

    layout->addLayout(nameLayout);


    QLabel* typeLabel = new QLabel("Type");
    layout->addWidget(typeLabel);

    QGroupBox* typeGroupBox = new QGroupBox();

    QVBoxLayout* typeGroupBoxLayout = new QVBoxLayout();
    typeGroupBoxLayout->setContentsMargins(4, 4, 4, 4);

    QRadioButton* stdoutRadioButton = new QRadioButton("stdout");
    typeGroupBoxLayout->addWidget(stdoutRadioButton);

    QRadioButton* conditionRadioButton = new QRadioButton("condition");
    typeGroupBoxLayout->addWidget(conditionRadioButton);

    QRadioButton* dataRadioButton = new QRadioButton("data");
    typeGroupBoxLayout->addWidget(dataRadioButton);

    typeGroupBox->setLayout(typeGroupBoxLayout);

    layout->addWidget(typeGroupBox);


    QHBoxLayout* commandLayout = new QHBoxLayout();

    QLabel* commandLabel = new QLabel("Command");
    commandLayout->addWidget(commandLabel);

    QLineEdit* commandLineEdit = new QLineEdit();
    commandLayout->addWidget(commandLineEdit);

    layout->addLayout(commandLayout);


    QHBoxLayout* intervalLayout = new QHBoxLayout();

    QLabel* intervalLabel = new QLabel("Interval (ms)");
    intervalLayout->addWidget(intervalLabel);

    QSpinBox* intervalSpinBox = new QSpinBox();
    intervalSpinBox->setMinimum(0);
    intervalSpinBox->setMaximum(INT_MAX);
    intervalLayout->addWidget(intervalSpinBox);

    layout->addLayout(intervalLayout);


    QLabel* conditionTypeLabel = new QLabel("Condition type");
    layout->addWidget(conditionTypeLabel);

    QGroupBox* conditionTypeGroupBox = new QGroupBox();

    QVBoxLayout* conditionTypeGroupBoxLayout = new QVBoxLayout();
    conditionTypeGroupBoxLayout->setContentsMargins(4, 4, 4, 4);

    QRadioButton* stdoutCondRadioButton = new QRadioButton("stdout");
    conditionTypeGroupBoxLayout->addWidget(stdoutCondRadioButton);

    QRadioButton* exitcodeCondRadioButton = new QRadioButton("exitcode");
    conditionTypeGroupBoxLayout->addWidget(exitcodeCondRadioButton);

    conditionTypeGroupBox->setLayout(conditionTypeGroupBoxLayout);

    layout->addWidget(conditionTypeGroupBox);


    QLabel* conditionsLabel = new QLabel("Conditions");
    layout->addWidget(conditionsLabel);

    QHBoxLayout* conditionsToolsLayout = new QHBoxLayout();

    QPushButton* addConditionPushButton = new QPushButton("+");
    conditionsToolsLayout->addWidget(addConditionPushButton);

    QPushButton* removeConditionPushButton = new QPushButton("-");
    conditionsToolsLayout->addWidget(removeConditionPushButton);

    layout->addLayout(conditionsToolsLayout);

    QLabel* conditionsTableHint = new QLabel("Gotten data  |  Icon  |  Text");
    layout->addWidget(conditionsTableHint);

    QScrollArea* conditionsScrollArea = new QScrollArea();
    conditionsScrollArea->setWidgetResizable(true);

    QWidget* conditionsWidget = new QWidget();
    conditionsWidget->setObjectName("conditionsWidget");
    conditionsWidget->setStyleSheet(mCLIOutputAppletPaneWidget->styleSheet());

    QVBoxLayout* conditionsWidgetLayout = new QVBoxLayout(conditionsWidget);
    conditionsWidgetLayout->setContentsMargins(2, 2, 2, 2);

    layout->addWidget(conditionsScrollArea);


    QLabel* elseConditionLabel = new QLabel("Else condition");
    layout->addWidget(elseConditionLabel);

    QHBoxLayout* elseIconLayout = new QHBoxLayout();

    QLabel* elseIconLabel = new QLabel("Icon");
    elseIconLayout->addWidget(elseIconLabel);

    QLineEdit* elseIconLineEdit = new QLineEdit();
    elseIconLayout->addWidget(elseIconLineEdit);

    layout->addLayout(elseIconLayout);

    QHBoxLayout* elseTextLayout = new QHBoxLayout();

    QLabel* elseTextLabel = new QLabel("Text");
    elseTextLayout->addWidget(elseTextLabel);

    QLineEdit* elseTextLineEdit = new QLineEdit();
    elseTextLayout->addWidget(elseTextLineEdit);

    layout->addLayout(elseTextLayout);


    QLabel* waitDataLabel = new QLabel("Wait data");
    layout->addWidget(waitDataLabel);

    QHBoxLayout* waitIconLayout = new QHBoxLayout();

    QLabel* waitIconLabel = new QLabel("Icon");
    waitIconLayout->addWidget(waitIconLabel);

    QLineEdit* waitIconLineEdit = new QLineEdit();
    waitIconLayout->addWidget(waitIconLineEdit);

    layout->addLayout(waitIconLayout);

    QHBoxLayout* waitTextLayout = new QHBoxLayout();

    QLabel* waitTextLabel = new QLabel("Text");
    waitTextLayout->addWidget(waitTextLabel);

    QLineEdit* waitTextLineEdit = new QLineEdit();
    waitTextLayout->addWidget(waitTextLineEdit);

    layout->addLayout(waitTextLayout);


    QSpacerItem* spacerItem = new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding);
    layout->addSpacerItem(spacerItem);


    QPushButton* savePushButton = new QPushButton("Create applet");
    layout->addWidget(savePushButton);


    // Make connections
    this->connect(backPushButton,
                  &QPushButton::clicked,
                  this,
                  [this, controlCenter]() {
        controlCenter->mCLIOutputWidgetVisible = false;
        mCLIOutputAppletPaneWidget->hide();
        delete mCLIOutputAppletPaneWidget;
    });


    QList<QWidget*> affectedWidgets = {conditionTypeLabel,
                                       conditionTypeGroupBox,
                                       conditionsLabel,
                                       addConditionPushButton,
                                       removeConditionPushButton,
                                       conditionsTableHint,
                                       conditionsScrollArea,
                                       elseConditionLabel,
                                       elseIconLabel,
                                       elseIconLineEdit,
                                       elseTextLabel,
                                       elseTextLineEdit};
    this->connect(stdoutRadioButton,
                  &QRadioButton::toggled,
                  this,
                  [stdoutRadioButton, affectedWidgets]() {
        if (stdoutRadioButton->isChecked()) {
            foreach (QWidget* widget, affectedWidgets) {
                widget->setVisible(false);
            }
        }
    });

    this->connect(conditionRadioButton,
                  &QRadioButton::toggled,
                  this,
                  [conditionRadioButton, affectedWidgets]() {
        if (conditionRadioButton->isChecked()) {
            foreach (QWidget* widget, affectedWidgets) {
                widget->setVisible(true);
            }
        }
    });

    this->connect(dataRadioButton,
                  &QRadioButton::toggled,
                  this,
                  [dataRadioButton, affectedWidgets]() {
        if (dataRadioButton->isChecked()) {
            foreach (QWidget* widget, affectedWidgets) {
                widget->setVisible(false);
            }
        }
    });

    auto layoutList = new QList<ConditionLine>();
    this->connect(addConditionPushButton,
                  &QPushButton::clicked,
                  this,
                  [layoutList,
                   conditionsWidgetLayout,
                   conditionsScrollArea,
                   conditionsWidget]() {
        QHBoxLayout* conditionLayout = new QHBoxLayout();

        QLineEdit* outputLineEdit = new QLineEdit();
        conditionLayout->addWidget(outputLineEdit);

        QLineEdit* iconLineEdit = new QLineEdit();
        conditionLayout->addWidget(iconLineEdit);

        QLineEdit* textLineEdit = new QLineEdit();
        conditionLayout->addWidget(textLineEdit);

        ConditionLine line;
        line.layout = conditionLayout;
        line.outputLineEdit = outputLineEdit;
        line.iconLineEdit = iconLineEdit;
        line.textLineEdit = textLineEdit;

        layoutList->append(line);
        conditionsWidgetLayout->addLayout(conditionLayout);
        conditionsScrollArea->setWidget(conditionsWidget);
    });

    this->connect(removeConditionPushButton,
                  &QPushButton::clicked,
                  this,
                  [layoutList,
                   conditionsScrollArea,
                   conditionsWidget]() {
        if (!layoutList->isEmpty()) {
          ConditionLine lastLine = layoutList->constLast();
          delete lastLine.outputLineEdit;
          delete lastLine.iconLineEdit;
          delete lastLine.textLineEdit;
          delete lastLine.layout;
          layoutList->removeLast();
          conditionsScrollArea->setWidget(conditionsWidget);
        }
    });

    this->connect(savePushButton,
                  &QPushButton::clicked,
                  this,
                  [this,
                   controlCenter,
                   nameLineEdit,
                   stdoutRadioButton,
                   conditionRadioButton,
                   dataRadioButton,
                   commandLineEdit,
                   intervalSpinBox,
                   stdoutCondRadioButton,
                   exitcodeCondRadioButton,
                   layoutList,
                   elseIconLineEdit,
                   elseTextLineEdit,
                   waitIconLineEdit,
                   waitTextLineEdit]() {
        QJsonObject appletConfig;

        // Type
        QString type = "";
        if (stdoutRadioButton->isChecked()) {
            type = "stdout";
        }
        else if (conditionRadioButton->isChecked()) {
            type = "condition";
        }
        else if (dataRadioButton->isChecked()) {
            type = "data";
        }
        appletConfig["type"] = QJsonValue(type);

        // Command
        appletConfig["command"] = QJsonValue(commandLineEdit->text());

        // Interval
        appletConfig["interval"] = QJsonValue(intervalSpinBox->value());

        // Conditions stuff
        if (conditionRadioButton->isChecked()) {
            // Condition type
            QString condType = "";
            if (stdoutCondRadioButton->isChecked()) {
                condType = "stdout";
            }
            else if (exitcodeCondRadioButton->isChecked()) {
                condType = "exitcode";
            }
            appletConfig["conditionType"] = QJsonValue(condType);

            // Conditions
            QJsonObject conditions;

            for (int i = 0; i < layoutList->count(); ++i) {
                ConditionLine line = layoutList->at(i);
                QString output = line.outputLineEdit->text();
                QString icon = line.iconLineEdit->text();
                QString text = line.textLineEdit->text();
                conditions[output] = QJsonValue(QString("%1;%2").arg(icon, text));
            }
            appletConfig["conditions"] = QJsonValue(conditions);

            // Else condition
            QString elseIcon = elseIconLineEdit->text();
            QString elseText = elseTextLineEdit->text();
            appletConfig["elseCondition"] = QJsonValue(QString("%1;%2").arg(elseIcon, elseText));
        }

        // Wait data
        QString waitIcon = waitIconLineEdit->text();
        QString waitText = waitTextLineEdit->text();
        appletConfig["waitData"] = QJsonValue(QString("%1;%2").arg(waitIcon, waitText));


        // Save to JSON
        QJsonDocument appletDoc(appletConfig);
        QDir dir(QString("%1/.config/plainDE/clioutput-applets").arg(QDir::homePath()));
        if (!dir.exists()) {
            dir.mkdir(QString("%1/.config/plainDE/clioutput-applets").arg(QDir::homePath()));
        }
        QFile appletFile(QString("%1/%2.json").arg(dir.absolutePath(), nameLineEdit->text()));
        appletFile.open(QFile::WriteOnly);
        appletFile.write(appletDoc.toJson(QJsonDocument::Indented));
        appletFile.close();

        QString appletItemText = QString("clioutput:%1").arg(nameLineEdit->text());
        QListWidgetItem* appletItem = new QListWidgetItem(appletItemText);
        appletItem->setIcon(QIcon::fromTheme("terminal"));
        mAppletsListWidget->addItem(appletItem);

        controlCenter->mCLIOutputWidgetVisible = false;
        mCLIOutputAppletPaneWidget->hide();
        delete mCLIOutputAppletPaneWidget;
    });
}

CLIOutputAppletPane::CLIOutputAppletPane(QListWidget* appletsListWidget) {
    mAppletsListWidget = appletsListWidget;
}

CLIOutputAppletPane::~CLIOutputAppletPane() {

}
