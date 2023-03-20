#include "datetime.h"

QJsonObject datetimeAppletPaneConfig;


void DatetimeAppletPane::readConfig() {
    QString homeDirectory = getenv("HOME");
    QFile file;
    QString data;

    file.setFileName(homeDirectory + "/.config/plainDE/config.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();
    datetimeAppletPaneConfig = QJsonDocument::fromJson(data.toUtf8()).object();
}

void DatetimeAppletPane::setCurrentSettings(QLineEdit* timeFormatLineEdit,
                                            QLineEdit* dateFormatLineEdit) {
    timeFormatLineEdit->setText(datetimeAppletPaneConfig["timeFormat"].toString());
    dateFormatLineEdit->setText(datetimeAppletPaneConfig["dateFormat"].toString());
}

void DatetimeAppletPane::saveSettings(QLineEdit* timeFormatLineEdit,
                                      QLineEdit* dateFormatLineEdit) {
    datetimeAppletPaneConfig["timeFormat"] = QJsonValue(timeFormatLineEdit->text());

    if (!dateFormatLineEdit->text().isEmpty()) {
        datetimeAppletPaneConfig["showDate"] = QJsonValue(true);
        datetimeAppletPaneConfig["dateFormat"] = QJsonValue(dateFormatLineEdit->text());
    }
    else {
        datetimeAppletPaneConfig["showDate"] = QJsonValue(false);
        datetimeAppletPaneConfig["dateFormat"] = QJsonValue("");
    }

    Pane::saveConfig(datetimeAppletPaneConfig);
}

QWidget* DatetimeAppletPane::createUI(Settings* controlCenter) {
    readConfig();

    // UI
    QWidget* datetimeAppletPane = new QWidget;
    datetimeAppletPane->setObjectName("datetimeAppletPane");
    datetimeAppletPane->setWindowTitle("plainControlCenter");
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(4, 4, 4, 4);
    datetimeAppletPane->setLayout(layout);

    // Theme
    QFile stylesheetReader("/usr/share/plainDE/styles/" + datetimeAppletPaneConfig["theme"].toString());
    stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream styleSheet(&stylesheetReader);
    datetimeAppletPane->setStyleSheet(styleSheet.readAll());
    stylesheetReader.close();

    QFont paneFont;
    paneFont.setFamily(datetimeAppletPaneConfig["fontFamily"].toString());
    paneFont.setPointSize(datetimeAppletPaneConfig["fontSize"].toInt());
    datetimeAppletPane->setFont(paneFont);

    short width = 400, height = 500;
    datetimeAppletPane->setGeometry(650, 250, width, height);

    QPushButton* backPushButton = new QPushButton("Close");
    backPushButton->setIcon(QIcon::fromTheme("go-previous"));
    datetimeAppletPane->layout()->addWidget(backPushButton);

    QLabel* timeFormatLabel = new QLabel("Time format");
    datetimeAppletPane->layout()->addWidget(timeFormatLabel);

    QLineEdit* timeFormatLineEdit = new QLineEdit;
    datetimeAppletPane->layout()->addWidget(timeFormatLineEdit);

    QLabel* timeDescriptionLabel = new QLabel("h - 0-23, 1-12\n"
                                              "hh - 00-23, 01-12\n"
                                              "m - 0-59\n"
                                              "mm - 00-59\n"
                                              "s - 0-59\n"
                                              "ss - 00-59\n"
                                              "AP - AM/PM\n"
                                              "t - timezone");
    datetimeAppletPane->layout()->addWidget(timeDescriptionLabel);

    QLabel* dateFormatLabel = new QLabel("Date format (leave blank to hide)");
    datetimeAppletPane->layout()->addWidget(dateFormatLabel);

    QLineEdit* dateFormatLineEdit = new QLineEdit;
    datetimeAppletPane->layout()->addWidget(dateFormatLineEdit);

    QLabel* dateDescriptionLabel = new QLabel("d - 1-31\n"
                                              "dd - 01-31\n"
                                              "ddd - Mon-Sun\n"
                                              "M - 1-12\n"
                                              "MM - 01-12\n"
                                              "MMM - Jan-Dec\n"
                                              "yy - 00-99\n"
                                              "yyyy - 1970-9999");
    datetimeAppletPane->layout()->addWidget(dateDescriptionLabel);

    QPushButton* revertPushButton = new QPushButton("Revert");
    datetimeAppletPane->layout()->addWidget(revertPushButton);

    QPushButton* savePushButton = new QPushButton("Save");
    datetimeAppletPane->layout()->addWidget(savePushButton);

    setCurrentSettings(timeFormatLineEdit, dateFormatLineEdit);


    // Make connections
    datetimeAppletPane->connect(revertPushButton, &QPushButton::clicked, datetimeAppletPane,
                                [this, timeFormatLineEdit, dateFormatLineEdit]() {
        setCurrentSettings(timeFormatLineEdit, dateFormatLineEdit);
    });

    datetimeAppletPane->connect(savePushButton, &QPushButton::clicked, datetimeAppletPane,
                                [this, timeFormatLineEdit, dateFormatLineEdit]() {
        saveSettings(timeFormatLineEdit, dateFormatLineEdit);
    });

    datetimeAppletPane->connect(backPushButton, &QPushButton::clicked, datetimeAppletPane,
                                [this, datetimeAppletPane, controlCenter]() {
        controlCenter->mDateTimeWidgetVisible = false;
        delete datetimeAppletPane;
        delete this;
    });


    return datetimeAppletPane;
}
