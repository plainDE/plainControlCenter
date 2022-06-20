#ifndef DATETIME_H
#define DATETIME_H

#include "../../pane.h"

class DatetimeAppletPane {
public:
    void readConfig();
    void setCurrentSettings(QLineEdit* timeFormatLineEdit,
                            QLineEdit* dateFormatLineEdit);
    void saveSettings(QLineEdit* timeFormatLineEdit,
                      QLineEdit* dateFormatLineEdit);
    QWidget* createUI();
};

#endif // DATETIME_H
