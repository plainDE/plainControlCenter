#ifndef DATETIMEDIALOG_H
#define DATETIMEDIALOG_H

#include "../dialog.h"

#include <QJsonObject>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QRadioButton>

class DateTimeDialog : public Dialog {
public:
    DateTimeDialog(QJsonObject* cfgObj);
    virtual void setPaneContents() override;
    void prepareToSave(QLineEdit* timeFormatLineEdit,
                       QCheckBox* showDateCheckBox,
                       QLineEdit* dateFormatLineEdit,
                       QCheckBox* weekNumbersCheckBox,
                       QRadioButton* mondayRadioButton,
                       QRadioButton* tuesdayRadioButton,
                       QRadioButton* wednesdayRadioButton,
                       QRadioButton* thursdayRadioButton,
                       QRadioButton* fridayRadioButton,
                       QRadioButton* saturdayRadioButton);
};

#endif // DATETIMEDIALOG_H
