#include "datetimedialog.h"

void DateTimeDialog::setPaneContents() {
    mContentsWidget = new QWidget();
    QVBoxLayout* contentsLayout = new QVBoxLayout(mContentsWidget);
    mContentsWidget->setObjectName("innerPane");

    // Adding widgets
    QHBoxLayout* timeFormatLayout = new QHBoxLayout();

    QLabel* timeFormatLabel = new QLabel(tr("Time Format"));
    timeFormatLabel->setFont(mTitleFont);
    timeFormatLayout->addWidget(timeFormatLabel);

    QLineEdit* timeFormatLineEdit = new QLineEdit();
    timeFormatLineEdit->setFont(mFont);
    timeFormatLineEdit->setToolTip("h - 0~23, 1-12\n"
                                   "hh - 00~23, 01-12\n"
                                   "m - 0~59\n"
                                   "mm - 00~59\n"
                                   "s - 0~59\n"
                                   "ss - 00~59\n"
                                   "AP - AM/PM\n"
                                   "t - timezone");
    timeFormatLayout->addWidget(timeFormatLineEdit);

    contentsLayout->addLayout(timeFormatLayout);

    QCheckBox* showDateCheckBox = new QCheckBox(tr("Show date"));
    showDateCheckBox->setFont(mTitleFont);
    contentsLayout->addWidget(showDateCheckBox);

    QHBoxLayout* dateFormatLayout = new QHBoxLayout();

    QLabel* dateFormatLabel = new QLabel(tr("Date Format"));
    dateFormatLabel->setFont(mTitleFont);
    dateFormatLayout->addWidget(dateFormatLabel);

    QLineEdit* dateFormatLineEdit = new QLineEdit();
    dateFormatLineEdit->setFont(mFont);
    dateFormatLineEdit->setToolTip("d - 1~31\n"
                                   "dd - 01~31\n"
                                   "ddd - Mon~Sun\n"
                                   "M - 1~12\n"
                                   "MM 01~12\n"
                                   "MMM - Jan~Dec\n"
                                   "yy - 00~99\n"
                                   "yyyy - 1970-9999");
    dateFormatLayout->addWidget(dateFormatLineEdit);

    contentsLayout->addLayout(dateFormatLayout);

    QCheckBox* weekNumbersCheckBox = new QCheckBox(tr("Show Week Numbers"));
    weekNumbersCheckBox->setFont(mTitleFont);
    contentsLayout->addWidget(weekNumbersCheckBox);

    QLabel* firstDayOfWeekLabel = new QLabel(tr("First Day of Week"));
    firstDayOfWeekLabel->setFont(mTitleFont);
    contentsLayout->addWidget(firstDayOfWeekLabel);

    QRadioButton* mondayRadioButton = new QRadioButton(tr("Monday"));
    mondayRadioButton->setFont(mFont);
    mondayRadioButton->setChecked(true);
    contentsLayout->addWidget(mondayRadioButton);

    QRadioButton* tuesdayRadioButton = new QRadioButton(tr("Tuesday"));
    tuesdayRadioButton->setFont(mFont);
    contentsLayout->addWidget(tuesdayRadioButton);

    QRadioButton* wednesdayRadioButton = new QRadioButton(tr("Wednesday"));
    wednesdayRadioButton->setFont(mFont);
    contentsLayout->addWidget(wednesdayRadioButton);

    QRadioButton* thursdayRadioButton = new QRadioButton(tr("Thursday"));
    thursdayRadioButton->setFont(mFont);
    contentsLayout->addWidget(thursdayRadioButton);

    QRadioButton* fridayRadioButton = new QRadioButton(tr("Friday"));
    fridayRadioButton->setFont(mFont);
    contentsLayout->addWidget(fridayRadioButton);

    QRadioButton* saturdayRadioButton = new QRadioButton(tr("Saturday"));
    saturdayRadioButton->setFont(mFont);
    contentsLayout->addWidget(saturdayRadioButton);

    QRadioButton* sundayRadioButton = new QRadioButton(tr("Sunday"));
    sundayRadioButton->setFont(mFont);
    contentsLayout->addWidget(sundayRadioButton);

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

    // Making connections
    connect(cancelButton, &QPushButton::clicked, this, [this]() {
        this->hide();
        delete this;
    });

    connect(okButton, &QPushButton::clicked, this, [this, timeFormatLineEdit, showDateCheckBox,
                                                    dateFormatLineEdit, weekNumbersCheckBox,
                                                    mondayRadioButton, tuesdayRadioButton,
                                                    wednesdayRadioButton, thursdayRadioButton,
                                                    fridayRadioButton, saturdayRadioButton]() {
        prepareToSave(timeFormatLineEdit, showDateCheckBox,
                      dateFormatLineEdit, weekNumbersCheckBox,
                      mondayRadioButton, tuesdayRadioButton,
                      wednesdayRadioButton, thursdayRadioButton,
                      fridayRadioButton, saturdayRadioButton);
        saveConfig();
        this->hide();
        delete this;
    });

    connect(showDateCheckBox, &QCheckBox::stateChanged, this, [this, showDateCheckBox,
                                                               dateFormatLineEdit, dateFormatLabel]() {
        dateFormatLabel->setVisible(showDateCheckBox->isChecked());
        dateFormatLineEdit->setVisible(showDateCheckBox->isChecked());
    });

    // Setting current settings
    timeFormatLineEdit->setText(getConfigValue("timeFormat").toString());
    showDateCheckBox->setChecked(getConfigValue("showDate").toBool());
    dateFormatLineEdit->setText(getConfigValue("dateFormat").toString());
    weekNumbersCheckBox->setChecked(getConfigValue("showWeekNumbers").toBool());

    int firstDayOfWeek = getConfigValue("firstDayOfWeek").toInt();
    switch (firstDayOfWeek) {
        case 1:
            mondayRadioButton->setChecked(true);
            break;
        case 2:
            tuesdayRadioButton->setChecked(true);
            break;
        case 3:
            wednesdayRadioButton->setChecked(true);
            break;
        case 4:
            thursdayRadioButton->setChecked(true);
            break;
        case 5:
            fridayRadioButton->setChecked(true);
            break;
        case 6:
            saturdayRadioButton->setChecked(true);
            break;
        case 7:
            sundayRadioButton->setChecked(true);
            break;
        default:
            break;
    }

    finalizeUI();
    setWindowGeometry();
    setTransparency(this);
}

void DateTimeDialog::prepareToSave(QLineEdit* timeFormatLineEdit,
                                   QCheckBox* showDateCheckBox,
                                   QLineEdit* dateFormatLineEdit,
                                   QCheckBox* weekNumbersCheckBox,
                                   QRadioButton* mondayRadioButton,
                                   QRadioButton* tuesdayRadioButton,
                                   QRadioButton* wednesdayRadioButton,
                                   QRadioButton* thursdayRadioButton,
                                   QRadioButton* fridayRadioButton,
                                   QRadioButton* saturdayRadioButton) {
    setEntry("timeFormat", timeFormatLineEdit->text());
    setEntry("showDate", showDateCheckBox->isChecked());
    setEntry("dateFormat", dateFormatLineEdit->text());
    setEntry("showWeekNumbers", weekNumbersCheckBox->isChecked());

    int firstDayOfWeek;
    if (mondayRadioButton->isChecked()) {
        firstDayOfWeek = 1;
    }
    else if (tuesdayRadioButton->isChecked()) {
        firstDayOfWeek = 2;
    }
    else if (wednesdayRadioButton->isChecked()) {
        firstDayOfWeek = 3;
    }
    else if (thursdayRadioButton->isChecked()) {
        firstDayOfWeek = 4;
    }
    else if (fridayRadioButton->isChecked()) {
        firstDayOfWeek = 5;
    }
    else if (saturdayRadioButton->isChecked()) {
        firstDayOfWeek = 6;
    }
    else {  // Sunday
        firstDayOfWeek = 7;
    }
    setEntry("firstDayOfWeek", firstDayOfWeek);
}

DateTimeDialog::DateTimeDialog(QJsonObject* cfgObj) : Dialog(cfgObj,
                                                             tr("Date & Time applet settings"),
                                                             "calendar") {

}
