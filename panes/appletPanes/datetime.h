#ifndef DATETIME_H
#define DATETIME_H

#include "../../pane.h"
#include "../../settings.h"

class DatetimeAppletPane {
public:
    void readConfig();
    void setCurrentSettings(QLineEdit* timeFormatLineEdit,
                            QLineEdit* dateFormatLineEdit,
                            QCheckBox* showWeekNumbersCheckBox);
    void saveSettings(QLineEdit* timeFormatLineEdit,
                      QLineEdit* dateFormatLineEdit,
                      QCheckBox* showWeekNumbersCheckBox);
    QWidget* createUI(Settings* controlCenter);
};

#endif // DATETIME_H
