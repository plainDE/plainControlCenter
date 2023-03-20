#ifndef DATETIME_H
#define DATETIME_H

#include "../../pane.h"
#include "../../settings.h"

class DatetimeAppletPane {
public:
    void readConfig();
    void setCurrentSettings(QLineEdit* timeFormatLineEdit,
                            QLineEdit* dateFormatLineEdit);
    void saveSettings(QLineEdit* timeFormatLineEdit,
                      QLineEdit* dateFormatLineEdit);
    QWidget* createUI(Settings* controlCenter);
};

#endif // DATETIME_H
